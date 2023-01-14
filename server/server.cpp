#include "database_reader.h"
#include "server_config.h"
#include "server_define.h"
#include "Room.h"
#include <string.h>
#include <iostream>

#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <csignal>
#include <netdb.h>

int serverSock;



void createServer(char * port) {
    addrinfo hints {.ai_flags=AI_PASSIVE, .ai_protocol = IPPROTO_TCP};
    addrinfo *resolved;
    if(int err = getaddrinfo(nullptr, port, &hints, &resolved)) {
        fprintf(stderr, "Resolving address failed: %s\n", gai_strerror(err));
        exit(1);
    }
    serverSock = socket(resolved->ai_family, resolved->ai_socktype, resolved->ai_protocol);
    const int one = 1;
    setsockopt(serverSock, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));
    if(bind(serverSock, resolved->ai_addr, resolved->ai_addrlen)) {
        fprintf(stderr, "Failed to bind: %s\n", strerror(errno));
        exit(1);
    }
    freeaddrinfo(resolved);
    listen(serverSock, 1);
    clientSock = accept(serverSock, nullptr, nullptr);
    close(serverSock);
    if(clientSock==-1) {
        fprintf(stderr, "Failed to accept: %s\n", strerror(errno));
        exit(1);
    }
}



int main(int argc, char** argv){
        if(argc!=2) {
            fprintf(stderr, "Usage:\n%s <port>\n", argv[0]);
            return 1;
        }

    createServer(argv[1]);

    signal(SIGINT, ctrl_c);

    long ctrs[4] {};

    char buf[256], *eol;
    int pos{0};

    printf(" |%18ld|%18ld|%18ld|%18ld|\r", ctrs[0], ctrs[1], ctrs[2], ctrs[3]);
    fflush(stdout);


}
