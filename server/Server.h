#ifndef SERVER_SERVER_H
#define SERVER_SERVER_H


#include <mutex>
#include <unordered_set>
#include "Room.h"

class Server {
public:
    explicit Server(int port);
    explicit Server(char * port);

public:
    Room room;
    int port;
    int serverSock;
    int clientFd;
    std::mutex clientFdsLock;
    std::unordered_set<int> helpClientFds;
    void runServerLoop();

private:
    void sendToAllBut(int fd, char *buffer, int count);
    void shoutDown();
    void prepareServerSock();
    void prepareClientSock();
    void runPlayerLoop();
};


#endif //SERVER_SERVER_H
