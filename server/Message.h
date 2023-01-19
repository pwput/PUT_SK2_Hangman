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
    Message(string command, string content){
        this->content = content;
        this->command = command;
    }
    string command;
    string content;
    string toString(){
        return command + MESSAGE_SEPARATOR +content + MESSAGE_END;
    }
};


#endif //SERVER_MESSAGE_H
