
#include "Player.h"

Player::Player() {}


Player::Player(int clientFd) : playerFd(clientFd){}



string Player::playerToString() {
    return "fd:" + to_string(this->playerFd) + "";
}

Player::Player(int playerFd, time_t joinTime) {
    this->playerFd = playerFd;
    this->joinTime = joinTime;
    this->lives = 0;
}
