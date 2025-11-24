#ifndef GUEST_H
#define GUEST_H
#include "constants.h"

typedef struct Guest {
    char name[NAME_LEN];
    char phone[PHONE_LEN];
    int roomNumber; // Store room number
    struct Guest* next;
} Guest;


void addGuest();
void listGuests();
int isValidPhone(const char *phone);

#endif
