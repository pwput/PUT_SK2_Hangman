#pragma once

#define PORT_NUMBER 8081
#define BUFFER_SIZE 1024
#define QUEUE_SIZE 4

//message
#define MESSAGE_SEPARATOR ";"
#define MESSAGE_END "\n"
#define MESSAGE_MAX_LENGTH 258


enum Commands {
    CREATE_USER,
    START_GAME,
    END_GAME,
    SEND_LETTER,
};


#define RESOLUT_SUCCESS "0"
#define RESOLUT_FAILURE "1"
