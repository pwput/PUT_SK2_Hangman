#ifndef SERVER_SERVER_H
#define SERVER_SERVER_H


#include <mutex>
#include <unordered_set>
#include <vector>
#include "../Message.h"
#include "ServerState.h"
#include "Player.h"

class Server {
public:
    explicit Server(char * port);

public:
    int port;
    int serverSock{};
    std::mutex clientFdsLock;
    std::mutex serverLock;
    std::unordered_set<int> helpClientFds;
    void runServerLoop();

private:
    string word;
    ServerState state;
    vector<Player> players_list;
    void erasePlayer(int clientFd);
    bool startGame(int playerFd);
    string addPlayer(Player player);
    bool isMessageValid(Message m);
    bool isEnoughPlayers();
    void sendTo(int fd, Message m);
    bool processMessage(int playerFd, Message m);
    void shoutDown();
    void prepareServerSock();
    int prepareClientSock(int clinetFd);
    void runPlayerLoop(int clinetFd);
    void sendToAll_noLock(Message m);
    bool guessLetter(int playerFd, Message m);
    string getScoreBoard();
    void sendGameEndMessage(int playerFd);

    void checkIfGameEnded();

    void sendToAllScoreboard_noLock();

    void sendToAllStartGame_noLock();
};


#endif //SERVER_SERVER_H
