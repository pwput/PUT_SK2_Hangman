#include "database_reader.h"
#include "server_config.h"
#include "Room.h"
#include <cstring>
#include <iostream>
#include "Player.h"

#include <unistd.h>
#include <sys/socket.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <netdb.h>
#include <thread>
#include <mutex>
#include <unordered_set>
#include <csignal>
#include <cerrno>
#include <error.h>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <error.h>
#include <netdb.h>
#include <thread>
#include <mutex>
#include <unordered_set>
#include <algorithm>


int serverSock;
// client sockets
std::mutex clientFdsLock;
std::vector<Player> players;

std::unordered_set<int> helpClientFds;


void checkPort(char *p) {
    char *ptr;
    auto port = strtol(p, &ptr, 10);
    if (*ptr != 0 || port < 1024 || port > 65535)
        fprintf(stderr, "Not valiable port number: %s.\n"
                        "Use number between 1024 and 65535\n", p);
    exit(0);
}


void ctrl_c(int) {
    std::unique_lock<std::mutex> lock(clientFdsLock);
    for (auto p: players) {
        shutdown(p.clientFd, SHUT_RDWR);
        close(p.clientFd);
    }
    close(serverSock);
    printf("Control + c pressed."
           "Closing server\n");
    exit(0);
}

void sendToAllBut(int fd, char *buffer, int count) {
    int res;
    std::unique_lock<std::mutex> lock(clientFdsLock);
    for (auto p: players) {
        if (p.clientFd == fd) continue;
        res = send(p.clientFd, buffer, count, MSG_DONTWAIT);
        if (res != count)
            helpClientFds.insert(p.clientFd);
    }
    for (int clientFd: helpClientFds) {
        printf("removing %d\n", clientFd);
        players.erase(std::remove_if(players.begin(), players.end(),
                      [clientFd](Player const &p) { return p.clientFd == clientFd; }), players.end());
        close(clientFd);
    }
}


void createServer(char *port) {
    // resolve addrinfo for given name
    addrinfo hints{.ai_flags=AI_PASSIVE, .ai_protocol = IPPROTO_TCP};
    addrinfo *resolved;
    if (int err = getaddrinfo("127.0.0.1", port, &hints, &resolved)) {
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

void createClients(int *clientFd) {
    // prepare placeholders for client address
    sockaddr_in clientAddr{};
    socklen_t clientAddrSize = sizeof(clientAddr);

    // accept new connection
    *clientFd = accept(serverSock, (sockaddr *) &clientAddr, &clientAddrSize);
    if (*clientFd == -1) error(1, errno, "accept failed");

    // add client to all clients set
    {
        std::unique_lock<std::mutex> lock(clientFdsLock);
        Player tmp = Player(*clientFd);
        players.push_back(tmp);
    }

    // tell who has connected
    printf("new connection from: %s:%hu (fd: %d)\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port),
           *clientFd);

}

    void clientLoop(int clientFd){
        char buffer[255];

        while (true) {
            // read a message
            int count = read(clientFd, buffer, 255);

            if (count < 1) {
                printf("removing %d\n", clientFd);
                {
                    std::unique_lock<std::mutex> lock(clientFdsLock);
                    players.erase(std::remove_if(players.begin(), players.end(),
                                                 [clientFd](Player const &p) { return p.clientFd == clientFd; }), players.end());
                    close(clientFd);
                }
                shutdown(clientFd, SHUT_RDWR);
                close(clientFd);
                break;
            } else {
                // broadcast the message
                sendToAllBut(clientFd, buffer, count);
            }
        }
}


int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage:\n%s <port>\n", argv[0]);
        return 1;
    }
    players.clear();

    createServer(argv[1]);
    while (true) {
        int clientFd;
        createClients(&clientFd);

        std::thread(clientLoop, std::ref(clientFd)).detach();
    }

}

