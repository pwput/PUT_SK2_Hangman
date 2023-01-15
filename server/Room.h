#ifndef SERVER_ROOM_H
#define SERVER_ROOM_H

#include <vector>

#include "Player.h"
#include "room_states.h"

#define GAME_MIN_SIZE  3
#define DEFAULT_NICK_LENGTH  6


using namespace std;

class Room {
private:
    RoomStates roomState;
    string getRandomName();
    string getNewRandomName();
    bool isNameTaken(string name);
public:
    vector<Player> players_list;
    void erasePlayer(int clientFd);
    bool startGame();
    string addPlayer(Player player);
    RoomStates getRoomState();
    void setRoomState(RoomStates roomState);
    bool isEnoughPlayers();
};


#endif //SERVER_ROOM_H
