#ifndef BOOKING_H
#define BOOKING_H
#include "constants.h"
#include "room.h"

typedef struct Booking {
    int bookingID;
    int roomNumber;
    char customerName[NAME_LEN];
    struct Booking* next;
} Booking;

extern Booking *headBooking;

typedef struct PastBooking {
    int bookingID;
    int roomNumber;
    char customerName[NAME_LEN];
    struct PastBooking *next;
} PastBooking;

extern PastBooking *headPastBooking;

void userMenu();
void staffMenu();
void loadBookingData();
void saveBookingData();
void addBooking(const char *username);
void cancelBookingForUser(const char *username);
void searchRooms();
void viewUserBookings(const char *username);
void viewUserBookingsStaff();
void applyBookingToRooms();
void moveBookingToHistory(Booking *b);
void savePastBookingData();
void loadPastBookingData();
void viewAllCurrentGuests();
#endif
