#ifndef SERVER_SERVER_H
#define SERVER_SERVER_H


#include <mutex>
#include <unordered_set>
#include <vector>
#include "Message.h"
#include "ServerState.h"
#include "Player.h"

class Server {
public:
    explicit Server(int port);
    explicit Server(char * port);

public:
    int port;
    int serverSock{};
    int clientFd{};
    std::mutex clientFdsLock;
    std::unordered_set<int> helpClientFds;
    void runServerLoop();

private:
    ServerState roomState;
    vector<Player> players_list;
    void erasePlayer(int clientFd);
    bool startGame(int playerFd);
    string addPlayer(Player player);
    ServerState getRoomState();
    bool isEnoughPlayers();
    void sendToAllBut(int fd, char *buffer, int count);
    void sendTo(int fd, char *buffer, int count);
    void sendTo(int fd, Message m);
    void processMessage(int playerFd, Message m);
    void shoutDown();
    void prepareServerSock();
    void prepareClientSock();
    void runPlayerLoop();
};


#endif //SERVER_SERVER_H
