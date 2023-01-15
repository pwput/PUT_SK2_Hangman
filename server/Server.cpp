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
#include <thread>
#include <iostream>
#include <functional>

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
        this->room.addPlayer(tmp);
    }
    // tell who has connected
    printf("new connection from: %s:%hu (fd: %d)\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port),
           this->clientFd);

}

void Server::shoutDown() {
    std::unique_lock<std::mutex> lock(clientFdsLock);
    for (auto p: room.players_list) {
        shutdown(p.clientFd, SHUT_RDWR);
        close(p.clientFd);
    }
    close(serverSock);
    printf("Closing server\n");
    exit(0);
}

void Server::runPlayerLoop() {
    char buffer[255];

    while (true) {
        // read a message
        int count = read(clientFd, buffer, 255);

        if (count < 1) {
            printf("removing %d\n", clientFd);
            {
                std::unique_lock<std::mutex> lock(clientFdsLock);
                room.erasePlayer(clientFd);
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

void Server::sendToAllBut(int fd, char *buffer, int count) {
    int res;
    std::unique_lock<std::mutex> lock(clientFdsLock);
    for (auto p: room.players_list) {
        if (p.clientFd == fd) continue;
        res = send(p.clientFd, buffer, count, MSG_DONTWAIT);
        if (res != count)
            helpClientFds.insert(p.clientFd);
    }
    for (int clientFd: helpClientFds) {
        printf("removing %d\n", clientFd);
        room.erasePlayer(clientFd);
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


