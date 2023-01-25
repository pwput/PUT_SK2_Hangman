//
// Created by piotr on 15.01.23.
//

#include <netdb.h>
#include <cstring>
#include "Server.h"
#include "Player.h"
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <iostream>
#include <functional>
#include "../Message.h"
#include "database_reader.h"

void logMessage(int clientFd, Message m);

void Server::prepareServerSock() {
    std::string p = std::to_string(this->port);
    addrinfo hints{.ai_flags=AI_PASSIVE, .ai_protocol = IPPROTO_TCP};
    addrinfo *resolved;
    if (int err = getaddrinfo(SERVER_ADDR, p.c_str(), &hints, &resolved)) {
        fprintf(stderr, "Resolving address failed: %s\n", gai_strerror(err));
        exit(1);
    }
    serverSock = socket(resolved->ai_family, resolved->ai_socktype, resolved->ai_protocol);
    const int one = 1;
    setsockopt(serverSock, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));
    if (bind(serverSock, resolved->ai_addr, resolved->ai_addrlen)) {
        fprintf(stderr, "Failed to bind: %s\n", strerror(errno));
        exit(1);
    }
    freeaddrinfo(resolved);

    if (int res = listen(serverSock, 1)) {
        fprintf(stderr, "Failed to listen %s\n", strerror(errno));
        exit(1);
    }
}

int Server::prepareClientSock(int clientFd) {
    // prepare placeholders for client address
    sockaddr_in clientAddr{};
    socklen_t clientAddrSize = sizeof(clientAddr);

    // accept new connection
    clientFd = accept(serverSock, (sockaddr *) &clientAddr, &clientAddrSize);
    if (clientFd == -1) {
        fprintf(stderr, "Failed to accept: %s\n", strerror(errno));
        exit(1);
    }
    // add client to all clients set
    {
        std::unique_lock<std::mutex> lock(clientFdsLock);
        auto const now = std::chrono::system_clock::now();
        this->addPlayer(Player(clientFd, chrono::system_clock::to_time_t(now)));
        sendToAllScoreboard_noLock();
    }
    // tell who has connected
    printf("New Player from: %s:%hu (fd: %d)\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port),
           clientFd);
    return clientFd;

}

void Server::shoutDown() {
    std::unique_lock<std::mutex> lock(clientFdsLock);
    for (auto p: players_list) {
        shutdown(p.playerFd, SHUT_RDWR);
        close(p.playerFd);
    }
    close(serverSock);
    printf("Closing server\n");
    exit(0);
}

void Server::runPlayerLoop(int clientFd) {
    char buf[256], *eol;
    int pos{0};

    while (true) {
        // dane z sieci zapisz do bufora, zaczynając od miejsca za wcześniej zapisanymi danymi
        int bytesRead = read(clientFd, buf + pos, 255 - pos);
        if (bytesRead <= 0){
            cout<<"Removing player: "<<clientFd<<endl;
            {
                unique_lock<mutex> rmcl(clientFdsLock);
                erasePlayer(clientFd);
                sendToAllScoreboard_noLock();
            }
            shutdown(clientFd,SHUT_RDWR);
            close(clientFd);
            checkIfGameEnded();
            return;
        }
        // zaktualizuj ile łącznie danych jest w buforze
        pos += bytesRead;
        // zapisz znak '\0' na końcu danych
        buf[pos] = 0;

        // dopóki w danych jest znak nowej linii
        while (nullptr != (eol = strchr(buf, '\n'))) {

            // przeczytaj komendę
            char cmd[256]{};
            int value{-1};
            sscanf(buf, "%s%d", cmd, &value);

            // usuń komendę z bufora
            // długość komendy
            int cmdLen = (eol - buf) + 1;
            // przesuń pozostałe dane i znak '\0' na początek bufora
            memmove(buf, eol + 1, pos - cmdLen + 1);
            // zaktualizuj zmienną pamiętającą ile danych jest w buforze
            pos -= cmdLen;

            Message receved = Message{string(cmd), to_string(value)};

            // sendTo(clientFd, receved);

            logMessage(clientFd, receved);
            processMessage(clientFd, receved);

        }
        // jeżeli w 255 znakach nie ma '\n', wyjdź.
        if (pos == 255)
            break;

    }
}

void Server::sendToAll_noLock(Message m) {
    string mess = m.command + MESSAGE_SEPARATOR + m.content + MESSAGE_END;
    const char *mm = mess.c_str();
    int res;
    for (auto p: players_list) {
        res = send(p.playerFd, mm, mess.length(), MSG_DONTWAIT);
        if (res != mess.length())
            helpClientFds.insert(p.playerFd);
    }
    for (int clientFd: helpClientFds) {
        printf("removing %d\n", clientFd);
        erasePlayer(clientFd);
        close(clientFd);
    }
}

Server::Server(char *port) {
    char *ptr;
    this->port = strtol(port, &ptr, 10);
    this->state = WaitingForPlayers;
    this->word = "";
}

void Server::runServerLoop() {
    prepareServerSock();
    while (true) {
        int clientFd = prepareClientSock(clientFd);
        sendTo(clientFd,Message(Commands[SCOREBOARD],getScoreBoard()));
        std::thread([this, clientFd] { runPlayerLoop(clientFd); }).detach();
    }
}


void Server::sendTo(int fd, Message m) {
    string mess = m.command + MESSAGE_SEPARATOR + m.content + MESSAGE_END;
    const char *mm = mess.c_str();
    int res;
    for (auto &p: players_list) {
        if (p.playerFd != fd) continue;
        res = send(p.playerFd, mm, mess.length(), MSG_DONTWAIT);
        if (res != mess.length())
            helpClientFds.insert(p.playerFd);
    }
    for (int clientFd: helpClientFds) {
        printf("removing %d\n", clientFd);
        erasePlayer(clientFd);
        close(clientFd);
    }
}

bool Server::processMessage(int playerFd, Message m) {
    if (!isMessageValid(m))
        return false;
    if (m.command == Commands[START_GAME]){
        bool started = startGame(playerFd);
        if (!started)
            sendTo(playerFd,Message(Commands[START_GAME],to_string(RESOLUT_FAILURE)));
        return started;
    }
    if (m.command == Commands[SEND_LETTER]){
        bool res =  guessLetter(playerFd, m);
        checkIfGameEnded();
        return res;
    }

    return false;
}

void logMessage(int fd, Message m) {
    cout << "From :" << fd << ", Prefix :" << m.command << ", Value :" << m.content << endl;
}


bool Server::isEnoughPlayers() {
    if (this->players_list.size() >= GAME_MIN_SIZE)
        return true;
    else
        return false;
}

string Server::addPlayer(Player player) {
    cout << "Player has joned!" << endl;
    cout << player.playerToString() << endl;
    players_list.push_back(player);
    return to_string(player.playerFd);
}


bool Server::startGame(int playerFd) {
    unique_lock<std::mutex> lock1(serverLock);
    unique_lock<std::mutex> lock2(clientFdsLock);

    if (!this->isEnoughPlayers())
        return false;
    if (this->state != WaitingForPlayers)
        return false;

    this->state = GameInProgress;

    for (auto &p: players_list) {
        p.lives = PLAYER_LIFES;
        p.state = InGmame;
    }

    this->word = get_random_word();
    cout << "New word:" << this->word << endl;

    for (auto &p: players_list) {
        p.word = string(word.length(),'?');
        sendTo(p.playerFd,Message(Commands[SEND_LETTER],p.word));
    }


    sendToAllStartGame_noLock();

    sendToAllScoreboard_noLock();

    cout << "New GAME starting" << endl;

    return true;
}


void Server::erasePlayer(int clientFd) {
    this->players_list.erase(std::remove_if(this->players_list.begin(), this->players_list.end(),
                                            [clientFd](Player const &p) { return p.playerFd == clientFd; }),
                             this->players_list.end());
}

bool Server::isMessageValid(Message m) {
    for (auto &command: Commands) {
        if (m.command == command && m.content != to_string(-1)) {
            cout << "Valid message" << endl;
            return true;
        }
    }
    cout << "Invalid message" << endl;
    return false;
}

bool Server::guessLetter(int playerFd, Message m) {
    unique_lock<std::mutex> lock1(serverLock);

    if (state != GameInProgress)
        return false;

    char letter = (char) stoi(m.content);

    cout <<"Letter: "<< letter<<endl;

    unique_lock<std::mutex> lock2(clientFdsLock);

    for (auto &p: this->players_list) {

        // find player
        if (p.playerFd == playerFd) {

            // check if he already guess this letter
            if (p.word.find(letter) != string::npos){
                sendTo(p.playerFd,Message(Commands[SEND_LETTER],p.word));
                return false;
            }

            // if guess wrong then punch and send scoreboard
            if (word.find(letter) == string::npos){
                p.punch();
                sendTo(p.playerFd,Message(Commands[SEND_LETTER],p.word));
                sendToAll_noLock(Message(Commands[SCOREBOARD], getScoreBoard()));
                if (p.lives == 0) {
                    cout << "Player lost" << playerFd << endl;
                }
                return false;
            }

            //guess right
            for (int i = 0; i < word.length(); ++i) {
                if(word[i]==letter)
                    p.word[i]=letter;
            }

            p.reward();
            sendTo(p.playerFd,Message(Commands[SEND_LETTER],p.word));
            sendToAllScoreboard_noLock();
            return true;
        }
    }

    return false;
}

string Server::getScoreBoard() {
    string res;
    for (auto p: players_list) {
        res += p.playerToString() + ";";
    }
    return res;
}

void Server::sendToAllStartGame_noLock(){
    sendToAll_noLock(Message(Commands[START_GAME], RESOLUT_SUCCESS));
}

void Server::sendToAllScoreboard_noLock(){
    sendToAll_noLock(Message(Commands[SCOREBOARD], getScoreBoard()));
}

void Server::sendGameEndMessage(int playerFd) {
    sendTo(playerFd,Message(Commands[END_GAME], to_string(RESOLUT_SUCCESS)));
}

void Server::checkIfGameEnded() {
    unique_lock<mutex> seg(serverLock);
    unique_lock<mutex> plge(clientFdsLock);

    if (state == WaitingForPlayers){
        return;
    }

    if(players_list.empty()){
        state == WaitingForPlayers;
    }

    //reset for players that lost
    for (auto &p : players_list) {
        if (p.lives == 0){
            p.state = WaitingForGame;
            p.lives = PLAYER_LIFES;
            sendGameEndMessage(p.playerFd);
        }
    }

    //restet for player that won
    for (auto &p : players_list) {
        if (p.word.find('?') == string::npos){
            p.state = WaitingForGame;
            p.lives = PLAYER_LIFES;
            p.score += REWARD_WIN;
            sendGameEndMessage(p.playerFd);
        }
    }


    //if is someone playing then return
    for (auto  &p:players_list){
        if (p.state == InGmame){
            return;
        }
    }

    //if everyone lost

    state = WaitingForPlayers;

}


