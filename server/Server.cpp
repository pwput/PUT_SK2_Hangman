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
#include "Message.h"

void logMessage(int clientFd, string prefix, int value);
void logMessage(int clientFd,Message m);

Server::Server(int port) : port(port) {}

void Server::prepareServerSock() {
    std::string p = std::to_string(this->port);
    // resolve addrinfo for given name
    addrinfo hints{.ai_flags=AI_PASSIVE, .ai_protocol = IPPROTO_TCP};
    addrinfo *resolved;
    if (int err = getaddrinfo("127.0.0.1", p.c_str(), &hints, &resolved)) {
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
    };
}

void Server::prepareClientSock() {
    // prepare placeholders for client address
    sockaddr_in clientAddr{};
    socklen_t clientAddrSize = sizeof(clientAddr);

    // accept new connection
    this->clientFd = accept(serverSock, (sockaddr *) &clientAddr, &clientAddrSize);
    if (this->clientFd == -1) {
        fprintf(stderr, "Failed to accept: %s\n", strerror(errno));
        exit(1);
    }
    // add client to all clients set
    {
        std::unique_lock<std::mutex> lock(clientFdsLock);
        Player tmp = Player(this->clientFd);
        this->addPlayer(tmp);
    }
    // tell who has connected
    printf("new connection from: %s:%hu (fd: %d)\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port),
           this->clientFd);

}

void Server::shoutDown() {
    std::unique_lock<std::mutex> lock(clientFdsLock);
    for (auto p: players_list) {
        shutdown(p.clientFd, SHUT_RDWR);
        close(p.clientFd);
    }
    close(serverSock);
    printf("Closing server\n");
    exit(0);
}

void Server::runPlayerLoop() {
    char buf[256], *eol;
    int pos{0};


    while (true) {
        // dane z sieci zapisz do bufora, zaczynając od miejsca za wcześniej zapisanymi danymi
        int bytesRead = read(clientFd, buf+pos, 255-pos);
        if (bytesRead <= 0)
            break;
        // zaktualizuj ile łącznie danych jest w buforze
        pos+=bytesRead;
        // zapisz znak '\0' na końcu danych
        buf[pos] = 0;


        // dopóki w danych jest znak nowej linii
        while(nullptr != (eol = strchr(buf, '\n'))){

            sendTo(clientFd,buf,bytesRead);

            // przeczytaj komendę

            char cmd[256] {};
            int value {-1};
            sscanf(buf, "%s%d", cmd,&value);

            // usuń komendę z bufora

            // (pomocnicze) wylicz długość komendy
            int cmdLen = (eol-buf)+1;
            // przesuń pozostałe dane i znak '\0' na początek bufora
            memmove(buf, eol+1, pos-cmdLen+1);
            // zaktualizuj zmienną pamiętającą ile danych jest w buforze
            pos -= cmdLen;

            Message receved =  Message{string(cmd),to_string(value)};


            logMessage(clientFd,receved);

        }
        // jeżeli w 255 znakach nie ma '\n', wyjdź.
        if(pos == 255)
            break;

    }
}

void Server::sendToAllBut(int fd, char *buffer, int count) {
    int res;
    std::unique_lock<std::mutex> lock(clientFdsLock);
    for (auto p: players_list) {
        if (p.clientFd == fd) continue;
        res = send(p.clientFd, buffer, count, MSG_DONTWAIT);
        if (res != count)
            helpClientFds.insert(p.clientFd);
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
}

void Server::runServerLoop() {
    prepareServerSock();
    while (true){
        prepareClientSock();
        std::thread(&Server::runPlayerLoop, this).detach();
    }
}

void Server::sendTo(int fd, char *buffer, int count) {
    int res;
    std::unique_lock<std::mutex> lock(clientFdsLock);
    for (auto p: players_list) {
        if (p.clientFd != fd) continue;
        res = send(p.clientFd, buffer, count, MSG_DONTWAIT);
        if (res != count)
            helpClientFds.insert(p.clientFd);
    }
    for (int clientFd: helpClientFds) {
        printf("removing %d\n", clientFd);
        erasePlayer(clientFd);
        close(clientFd);
    }
}

void Server::sendTo(int fd, Message m) {

}

void Server::processMessage(int playerFd, Message m) {

}

void logMessage(int fd, Message m){
    cout<<"From :"<<fd<<", Prefix :"<<m.command<<", Value :"<<m.content<<endl;
}

void logMessage(int clientFd, string prefix, int value){
    cout<<"From :"<<clientFd<<", Prefix :"<<prefix<<", Value :"<<value<<endl;
}

ServerState Server::getRoomState() {
    return roomState;
}

bool Server::isEnoughPlayers() {
    if (this->players_list.size() == GAME_MIN_SIZE)
        return true;
    else
        return false;
}

string Server::addPlayer(Player player) {
    players_list.push_back(player);
    return player.nick;
}

bool Server::startGame(int playerFd) {
    if (!this->isEnoughPlayers())
        return false;
    else return true;
}


void Server::erasePlayer(int clientFd) {
    this->players_list.erase(std::remove_if(this->players_list.begin(), this->players_list.end(),
                                            [clientFd](Player const &p) { return p.clientFd == clientFd; }), this->players_list.end());
}

