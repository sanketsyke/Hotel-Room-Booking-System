#include "room.h"
#include "booking.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Room *headRoom = NULL;

void initializeRooms() {
    // Free existing list if any
    Room *curr = headRoom;
    while (curr) {
        Room *tmp = curr;
        curr = curr->next;
        free(tmp);
    }
    headRoom = NULL;

    Room *tail = NULL;
    for (int floor = 1; floor <= 5; ++floor) {
        for (int num = 1; num <= 10; ++num) {
            Room *newRoom = (Room*) malloc(sizeof(Room));
            if (!newRoom) {
                printf("Memory allocation failed!\n");
                return;
            }
            newRoom->roomNumber = floor * 100 + num;
            newRoom->isAvailable = 1;

            if (num >= 1 && num <= 4) {
                newRoom->ac = 1;
                newRoom->beds = 1;
            } else if (num >= 5 && num <= 7) {
                newRoom->ac = 0;
                newRoom->beds = 2;
            } else {
                newRoom->ac = 1;
                newRoom->beds = 2;
            }
            newRoom->next = NULL;

            if (headRoom == NULL) {
                headRoom = tail = newRoom;
            } else {
                tail->next = newRoom;
                tail = newRoom;
            }
        }
    }
}

void listRoomsTable() {
    Room *r = headRoom;
    printf("\n%-8s %-6s %-6s %-14s %s\n", "Room No", "Beds", "AC", "Availability", "Guest");
    printf("---------------------------------------------------------------\n");

    while (r) {
        char guestName[NAME_LEN] = "";
        if (!r->isAvailable) {
            Booking *b = headBooking;
            while (b) {
                if (b->roomNumber == r->roomNumber) {
                    strncpy(guestName, b->customerName, NAME_LEN);
                    guestName[NAME_LEN - 1] = '\0';
                    break;
                }
                b = b->next;
            }
        }
        printf("%-8d %-6d %-6s %-14s %s\n",
               r->roomNumber,
               r->beds,
               r->ac ? "Yes" : "No",
               r->isAvailable ? "Available" : "Not Available",
               r->isAvailable ? "" : guestName);
        r = r->next;
    }
}

// Allows staff to checkout guest by room number
void checkoutRoomStaff() {
    int rn;
    printf("Room to checkout (guest leaving): ");
    scanf("%d", &rn);
    getchar();

    Room *currRoom = headRoom;
    int found = 0;
    while (currRoom) {
        if (currRoom->roomNumber == rn) {
            currRoom->isAvailable = 1;
            found = 1;
            break;
        }
        currRoom = currRoom->next;
    }
    if (!found) {
        printf("Room not found.\n");
        return;
    }

    Booking *currBooking = headBooking, *prevBooking = NULL;
    while (currBooking) {
        if (currBooking->roomNumber == rn) {
            moveBookingToHistory(currBooking);
            Booking *toFree = currBooking;
            if (prevBooking)
                prevBooking->next = currBooking->next;
            else
                headBooking = currBooking->next;
            currBooking = currBooking->next;
            free(toFree);
            printf("Guest has checked out, room now available.\n");
            return;
        }
        prevBooking = currBooking;
        currBooking = currBooking->next;
    }
    printf("No booking found for this room (it was already free).\n");
}
