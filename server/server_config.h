#pragma once

#define PORT_NUMBER 8081
#define BUFFER_SIZE 1024
#define QUEUE_SIZE 4
#define GAME_MIN_SIZE 3

#define MESSAGE_SEPARATOR " "
#define MESSAGE_END "\n"
#define MESSAGE_MAX_LENGTH 258

#define stringify( name ) #name


enum Commands{
    START_GAME = 0,
    END_GAME,
    SEND_LETTER,
    COMMANDS_LAST_ITEM
};

static const char* Commands[] = {
        stringify(START_GAME),
        stringify(END_GAME),
        stringify(SEND_LETTER)
};



#define RESOLUT_SUCCESS 0
#define RESOLUT_FAILURE 1
