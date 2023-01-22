
#include "Player.h"
#include "server_config.h"

Player::Player() {}


Player::Player(int clientFd) : playerFd(clientFd){
    this->state = WaitingForGame;
}



string Player::playerToString() {
    return to_string(this->playerFd) + "," + to_string(this->score) + "," + to_string(this->lives) + "," + to_string(this->state);
}

Player::Player(int playerFd, time_t joinTime) {
    this->playerFd = playerFd;
    this->joinTime = joinTime;
    this->lives =PLAYER_LIFES;
    this->state = WaitingForGame;
}

int Player::getPlayerFd() const {
    return playerFd;
}

time_t Player::getJoinTime() const {
    return joinTime;
}

int Player::getScore() const {
    return score;
}

int Player::getLives() const {
    return lives;
}

PlayerState Player::getState() const {
    return state;
}

void Player::setPlayerFd(int playerFd) {
    Player::playerFd = playerFd;
}

void Player::setJoinTime(time_t joinTime) {
    Player::joinTime = joinTime;
}

void Player::setScore(int score) {
    Player::score = score;
}

void Player::setLives(int lives) {
    Player::lives = lives;
}

void Player::setState(PlayerState state) {
    Player::state = state;
}

void Player::punch() {
    this->lives -= 1;
}

void Player::reward() {
    this->score += REWARD;
}
