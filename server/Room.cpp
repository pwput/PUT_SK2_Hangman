#include "Room.h"

RoomStates Room::getRoomState() {
    return roomState;
}

void Room::setRoomState(RoomStates roomState) {
    Room::roomState = roomState;
}

bool Room::isEnoughPlayers() {
    if (this->players_list.size() == GAME_MIN_SIZE)
        return true;
    else
        return false;
}

string Room::addPlayer(Player player) {
    players_list.push_back(player);
    return player.nick;
}

bool Room::startGame() {
    return false;
}

string Room::getRandomName() {
    static const string numbers = "0123456789";
    string newName;
    newName.reserve(DEFAULT_NICK_LENGTH);
    srand((unsigned )time(NULL));
    for (int i = 0; i < newName.length(); ++i) {
        newName[i] = numbers[   rand() % numbers.length()];
    }
    return newName;
}

string Room::getNewRandomName() {
    string tmp = getRandomName();
    while (isNameTaken(tmp))
        tmp = getRandomName();
    return tmp;
}

bool Room::isNameTaken(string name) {
    for (Player p: this->players_list)
        if (p.nick == name)
            return true;
    return false;
}
