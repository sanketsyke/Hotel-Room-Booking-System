#ifndef BOOKING_H
#define BOOKING_H

#include "constants.h"
#include "room.h"

// Structure representing an active booking
typedef struct Booking {
    int bookingID;
    int roomNumber;
    char customerName[NAME_LEN];
    struct Booking *next;
} Booking;

extern Booking *headBooking;

// Structure representing a past booking (history)
typedef struct PastBooking {
    int bookingID;
    int roomNumber;
    char customerName[NAME_LEN];
    struct PastBooking *next;
} PastBooking;

extern PastBooking *headPastBooking;

// User and staff menu
void userMenu();
void staffMenu();

// Booking data persistence
void loadBookingData();
void saveBookingData();
void loadPastBookingData();
void savePastBookingData();

// Booking operations
void addBooking(const char *username);
void cancelBookingForUser(const char *username);
void moveBookingToHistory(Booking *b);
void applyBookingToRooms();

// Room and booking queries
void searchRooms();
void viewUserBookings(const char *username);
void viewUserBookingsStaff();
void viewAllCurrentGuests();

#endif  // BOOKING_H
