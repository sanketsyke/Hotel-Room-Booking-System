// Including header files
#include "booking.h"
#include "feedback.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Global pointers to the head of linked lists
Booking *headBooking = NULL;
PastBooking *headPastBooking = NULL;

// Persistence files
#define BOOKING_FILE "bookings.dat"
#define PAST_BOOKING_FILE "past_bookings.dat"
#define BOOKING_ID_FILE "booking_id_counter.dat"

// Read last saved booking ID (default base is 1000 if file does not exist)
int getLastBookingID() {
    FILE *fp = fopen(BOOKING_ID_FILE, "rb");
    int lastId = 1000;
    if (fp) {
        fread(&lastId, sizeof(int), 1, fp);
        fclose(fp);
    }
    return lastId;
}
// Save the last used booking ID
void saveLastBookingID(int lastId) {
    FILE *fp = fopen(BOOKING_ID_FILE, "wb");
    if (fp) {
        fwrite(&lastId, sizeof(int), 1, fp);
        fclose(fp);
    }
}
// Generate a new booking ID and persist it
int generateBookingID() {
    static int count = -1;
    if (count == -1) {
        count = getLastBookingID();
    }
    count++;
    saveLastBookingID(count);
    return count;
}
// Add a new booking for a given user
void addBooking(const char *username) {
    Booking *newBooking = (Booking*) malloc(sizeof(Booking));
    if (!newBooking) {
        printf("Memory allocation failed!\n");
        return;
    }
    newBooking->bookingID = generateBookingID();
    strncpy(newBooking->customerName, username, NAME_LEN);
    printf("\nRooms available:\n");
    listRoomsTable();
    printf("Enter room number to book: ");
    scanf("%d", &newBooking->roomNumber);
    // Validate room number and availability
    Room *r = headRoom;
    int valid = 0;
    while (r) {
        if (r->roomNumber == newBooking->roomNumber && r->isAvailable) {
            r->isAvailable = 0;
            valid = 1;
            break;
        }
        r = r->next;
    }
    if (!valid) {
        printf("Invalid room number or already booked.\n");
        free(newBooking);
        return;
    }
    // Insert new booking at the front of the list
    newBooking->next = headBooking;
    headBooking = newBooking;
    printf("Booking successful! Your booking ID: %d\n", newBooking->bookingID);
}
// Cancel or checkout a booking for the given user
void cancelBookingForUser(const char *username) {
    int id, found = 0;
    printf("Enter booking ID to checkout/cancel: ");
    scanf("%d", &id);
    Booking *curr = headBooking, *prev = NULL;
    while (curr) {
        if (curr->bookingID == id && strcmp(curr->customerName, username) == 0) {
            // Mark corresponding room as available again
            Room *r = headRoom;
            while (r) {
                if (r->roomNumber == curr->roomNumber) {
                    r->isAvailable = 1;
                    break;
                }
                r = r->next;
            }
            moveBookingToHistory(curr);
            // Unlink node from active bookings list
            if (prev)
                prev->next = curr->next;
            else
                headBooking = curr->next;
            free(curr);
            printf("You have checked out. Booking cancelled and room is now available.\n");
            found = 1;
            break;
        }
        prev = curr;
        curr = curr->next;
    }
    if (!found)
        printf("Booking ID not found or not your booking.\n");
}
// Move active booking to past bookings list
void moveBookingToHistory(Booking *b) {
    PastBooking *pb = (PastBooking*) malloc(sizeof(PastBooking));
    if (!pb) {
        printf("Memory allocation failed!\n");
        return;
    }
    pb->bookingID = b->bookingID;
    pb->roomNumber = b->roomNumber;
    strncpy(pb->customerName, b->customerName, NAME_LEN);
    pb->next = headPastBooking;
    headPastBooking = pb;
}
// Display all rooms to the user
void searchRooms() {
    printf("\nRooms at NAS Hotels:\n");
    listRoomsTable();
}
// Show active and past bookings for a given user
void viewUserBookings(const char *username) {
    Booking *curr = headBooking;
    int found = 0;
    printf("--- Active Bookings ---\n");
    while (curr) {
        if (strcmp(curr->customerName, username) == 0) {
            printf("BookingID: %d | Room: %d\n", curr->bookingID, curr->roomNumber);
            found = 1;
        }
        curr = curr->next;
    }
    if (!found)
        printf("No current bookings for %s\n", username);
    PastBooking *pbc = headPastBooking;
    found = 0;
    printf("--- Past Bookings ---\n");
    while (pbc) {
        if (strcmp(pbc->customerName, username) == 0) {
            printf("BookingID: %d | Room: %d\n", pbc->bookingID, pbc->roomNumber);
            found = 1;
        }
        pbc = pbc->next;
    }
    if (!found)
        printf("No past bookings for %s\n", username);
}
// Staff menu: view bookings of a specific user
void viewUserBookingsStaff() {
    char username[NAME_LEN];
    getchar();
    printf("Enter user name to view bookings: ");
    fgets(username, NAME_LEN, stdin);
    username[strcspn(username, "\n")] = 0;
    viewUserBookings(username);
}
// Apply active bookings to room availability at startup
void applyBookingToRooms() {
    Booking *curr = headBooking;
    while (curr) {
        Room *r = headRoom;
        while (r) {
            if (r->roomNumber == curr->roomNumber) {
                r->isAvailable = 0;
                break;
            }
            r = r->next;
        }
        curr = curr->next;
    }
}
// Save active bookings to file
void saveBookingData() {
    FILE *fp = fopen(BOOKING_FILE, "wb");
    if (!fp) {
        printf("ERROR: Cannot open file for booking save!\n");
        return;
    }
    Booking *curr = headBooking;
    while (curr) {
        fwrite(curr, sizeof(Booking), 1, fp);
        curr = curr->next;
    }
    fclose(fp);
}
// Load active bookings from file
void loadBookingData() {
    FILE *fp = fopen(BOOKING_FILE, "rb");
    if (!fp) {
        return;
    }
    Booking temp;
    while (fread(&temp, sizeof(Booking), 1, fp)) {
        Booking *b = (Booking*) malloc(sizeof(Booking));
        if (!b) {
            printf("Memory allocation failed while loading bookings!\n");
            fclose(fp);
            return;
        }
        *b = temp;
        b->next = headBooking;
        headBooking = b;
    }
    fclose(fp);
}
// Save past bookings to file
void savePastBookingData() {
    FILE *fp = fopen(PAST_BOOKING_FILE, "wb");
    if (!fp) {
        printf("ERROR: Cannot open file for past booking save!\n");
        return;
    }
    PastBooking *curr = headPastBooking;
    while (curr) {
        fwrite(curr, sizeof(PastBooking), 1, fp);
        curr = curr->next;
    }
    fclose(fp);
}
// Load past bookings from file
void loadPastBookingData() {
    FILE *fp = fopen(PAST_BOOKING_FILE, "rb");
    if (!fp) {
        return;
    }
    PastBooking temp;
    while (fread(&temp, sizeof(PastBooking), 1, fp)) {
        PastBooking *b = (PastBooking*) malloc(sizeof(PastBooking));
        if (!b) {
            printf("Memory allocation failed while loading past bookings!\n");
            fclose(fp);
            return;
        }
        *b = temp;
        b->next = headPastBooking;
        headPastBooking = b;
    }
    fclose(fp);
}
// Show all current guests (room view for staff)
void viewAllCurrentGuests() {
    printf("\n--- Current Guests ---\n");
    printf("%-8s %-20s\n", "Room No", "Guest Name");
    printf("------------------------------\n");
    Booking *curr = headBooking;
    int found = 0;
    while (curr) {
        printf("%-8d %-20s\n", curr->roomNumber, curr->customerName);
        found = 1;
        curr = curr->next;
    }
    if (!found)
        printf("No current guests.\n");
}
// User menu
void userMenu() {
    int choice;
    char username[NAME_LEN];
    printf("Enter your name for login/view: ");
    fgets(username, NAME_LEN, stdin);
    username[strcspn(username, "\n")] = 0;
    do {
        printf("\n[User: %s] - NAS Hotels\n"
               "1. Search rooms\n"
               "2. Book room\n"
               "3. Checkout (Cancel booking)\n"
               "4. View my bookings (current & past)\n"
               "5. Leave feedback\n"
               "6. Exit\nChoice: ", username);
        scanf("%d", &choice);
        getchar();
        if (choice == 1) searchRooms();
        else if (choice == 2) addBooking(username);
        else if (choice == 3) cancelBookingForUser(username);
        else if (choice == 4) viewUserBookings(username);
        else if (choice == 5) addFeedback(username);
        else if (choice == 6) break;
        else printf("Invalid input. Please enter a number between 1 and 6.\n");
    } while (choice != 6);
}
// Staff menu
void staffMenu() {
    int choice;
    do {
        printf("\n[Staff] - NAS Hotels\n"
               "1. Checkout Room (remove guest from room)\n"
               "2. View Rooms\n"
               "3. View Current Bookings\n"
               "4. View Feedback\n"
               "5. View User Bookings (current & past)\n"
               "6. Exit\nChoice: ");
        scanf("%d", &choice);
        if (choice == 1) checkoutRoomStaff();
        else if (choice == 2) listRoomsTable();
        else if (choice == 3) viewAllCurrentGuests();
        else if (choice == 4) listFeedbacks();
        else if (choice == 5) viewUserBookingsStaff();
        else if (choice == 6) break;
        else printf("Invalid input. Please enter a number between 1 and 6.\n");
    } while (choice != 6);
}
