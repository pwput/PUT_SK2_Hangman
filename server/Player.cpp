
#include "Player.h"

Player::Player() {}


Player::Player(int clientFd) : clientFd(clientFd){}

string Player::playerToString() {
    return "fd:"+to_string(this->clientFd) + ", name:"+ this->nick ;
}
