//
// Created by piotr on 15.01.23.
//

#ifndef SERVER_MESSAGE_H
#define SERVER_MESSAGE_H

#include <string>
#include "server_config.h"

using namespace std;

class Message {
public:
    string command;
    string content;
    string messageToString(){
        return "";
    }
};


#endif //SERVER_MESSAGE_H
