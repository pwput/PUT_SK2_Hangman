#ifndef SERVER_SERVER_H
#define SERVER_SERVER_H


#include <mutex>
#include <unordered_set>

class Server {
public:
    explicit Server(int port);

public:
    int port;
    int serverSock;
    std::mutex clientFdsLock;
    std::unordered_set<int> helpClientFds;
private:
};


#endif //SERVER_SERVER_H
