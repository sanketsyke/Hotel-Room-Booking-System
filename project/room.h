#ifndef ROOM_H
#define ROOM_H
#include "constants.h"

typedef struct Room {
    int roomNumber;
    int isAvailable;
    int ac;
    int beds;
    struct Room *next;
} Room;

extern Room *headRoom;

void initializeRooms();
void listRoomsTable();
void updateRoom();
void checkoutRoomStaff();
#endif
