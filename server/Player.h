#ifndef SERVER_PLAYER_H
#define SERVER_PLAYER_H

#include <string>
#include <chrono>

#define PLAYER_LIFES 10

using namespace std;

class Player {
public:
    Player();

    explicit Player(int playerFd);
    Player(int playerFd, time_t joinTime);
    string playerToString();

public:
    int playerFd;
    time_t joinTime;
    int score = 0;
    int lives = PLAYER_LIFES;
};


#endif //SERVER_PLAYER_H
