#ifndef SERVER_PLAYER_H
#define SERVER_PLAYER_H

#include <string>
#include <chrono>
#include "PlayerState.h"
#include "server_config.h"


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
    int lives;
    PlayerState state;

    void reward();

    void punch();

    void setPlayerFd(int playerFd);

    void setJoinTime(time_t joinTime);

    void setScore(int score);

    void setLives(int lives);

    void setState(PlayerState state);

    int getPlayerFd() const;

    time_t getJoinTime() const;

    int getScore() const;

    int getLives() const;

    PlayerState getState() const;


};


#endif //SERVER_PLAYER_H
