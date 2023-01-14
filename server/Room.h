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
    vector<Player> players_list;
    string getRandomName();
    string getNewRandomName();
    bool isNameTaken(string name);
public:
    bool startGame();
    string addPlayer(Player player);
    RoomStates getRoomState();
    void setRoomState(RoomStates roomState);
    bool isEnoughPlayers();
};


#endif //SERVER_ROOM_H
