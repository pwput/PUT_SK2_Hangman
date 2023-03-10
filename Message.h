//
// Created by piotr on 15.01.23.
//

#ifndef SERVER_MESSAGE_H
#define SERVER_MESSAGE_H

#include <string>
#include "config.h"

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
        return "Message:" + command + MESSAGE_SEPARATOR +content + MESSAGE_END;
    }
    string toSend(){
        return command + " " + content + "\n";
    }
};


//message


#endif //SERVER_MESSAGE_H
