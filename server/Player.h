#ifndef SERVER_PLAYER_H
#define SERVER_PLAYER_H

#include <string>
#include <chrono>

#define PLAYER_LIFES 10

using namespace std;

class Player {
public:
    string nick;
    time_t joinTime;
    int score;
    int lives = PLAYER_LIFES;
};


#endif //SERVER_PLAYER_H
