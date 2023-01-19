#include "database_reader.h"
#include "server_config.h"
#include <cstring>
#include <iostream>
#include "Player.h"
#include "Server.h"

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


void checkPort(char *p) {
    char *ptr;
    auto port = strtol(p, &ptr, 10);
    if (*ptr != 0 || port < 1024 || port > 65535) {
        fprintf(stderr, "Not valiable port number: %s.\n"
                        "Use number between 1024 and 65535\n", p);
        exit(0);
    }

}

void checkArgument(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage:\n%s <port>\n", argv[0]);
        exit(0);
    }
    checkPort(argv[1]);
}

int main(int argc, char **argv) {

    checkArgument(argc, argv);

    Server server = Server(argv[1]);
    server.runServerLoop();

}

