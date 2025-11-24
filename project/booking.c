// Including header files
#include "booking.h"
#include "feedback.h"
#include "guest.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define global pointers to the head of linked lists
Booking *headBooking = NULL;                    
PastBooking *headPastBooking = NULL;

// Persistence files
#define BOOKING_FILE "bookings.dat"
#define PAST_BOOKING_FILE "past_bookings.dat"
#define BOOKING_ID_FILE "booking_id_counter.dat"

// Reads last saved booking ID
int getLastBookingID() {
    FILE *fp = fopen(BOOKING_ID_FILE, "rb");        // Opening file in binary read mode
    int lastId = 1000;                              // default base booking ID
    if (fp) {                                       // Checks if the file is opened successfully
        fread(&lastId, sizeof(int), 1, fp);         // Reads one int from file into lastId variable
        fclose(fp);                                 
    }
    return lastId;
}

// Saves the given last ID
void saveLastBookingID(int lastId) {                
    FILE *fp = fopen(BOOKING_ID_FILE, "wb");        // Opening file in binary write mode
    if (fp) {
        fwrite(&lastId, sizeof(int), 1, fp);        // Writes the int lastId to the file
        fclose(fp);
    }
}

// Generating new booking ID
int generateBookingID() {
    static int count = -1;                          // we use static variable to initialise count only once and it is initialised 
                                                    // to -1 because it shows no booking Id has been initialised yet
    if (count == -1) {                              // This runs only for the first call
        count = getLastBookingID();                 
    }
    count++;                                        
    saveLastBookingID(count);                       // Persisting new count value
    return count;                                   
}

// Adding new booking for certain person
void addBooking(const char *username) {
    Booking *newBooking = (Booking*) malloc(sizeof(Booking));       // Memory allocation
    if (!newBooking) {                                              // Checking allocation
        printf("Memory allocation failed!\n");
        return;
    }
    newBooking->bookingID = generateBookingID();                    // Giving new Booking ID
    strncpy(newBooking->customerName, username, NAME_LEN);          // Copies username to customerName
    printf("\nRooms available:\n"); 
    listRoomsTable();                                               // Display available rooms
    printf("Enter room number to book: ");
    scanf("%d", &newBooking->roomNumber);   

    // Check if room exists and is available
    Room *r = headRoom;                                             // Start itereating rooms from headRoom
    int valid = 0;                                                  // Flag whether the room is valid and available
    while (r) {                                                     // Iterating over Room linked list
        if (r->roomNumber == newBooking->roomNumber && r->isAvailable) {    // Checking if avialable
            r->isAvailable = 0;                                     
            valid = 1;
            break;
        }
        r = r->next;                                                // Move to next room in the list
    }
    if (!valid) {
        printf("Invalid room number or already booked.\n");
        free(newBooking);
        return;
    }
    // Inserting the new booking at the front of the linked list
    newBooking->next = headBooking;
    headBooking = newBooking;
    printf("Booking successful! Your booking ID: %d\n", newBooking->bookingID);
}

// Cancelling or Checking Out from a room owned by user of a certain name
void cancelBookingForUser(const char *username) {
    int id, found = 0;                                       // Declaring Booking Id to remove and a found flag
    printf("Enter booking ID to checkout/cancel: ");        
    scanf("%d", &id);        
    Booking *curr = headBooking, *prev = NULL;
    // Looping through bookings
    while(curr) {
        // If booking matches the id and belongs to that user, mark the room available again
        if(curr->bookingID == id && strcmp(curr->customerName, username) == 0) {
            Room *r = headRoom;
            // Searching for the room and making it available
            while(r) {
                if(r->roomNumber == curr->roomNumber) {
                    r->isAvailable = 1;
                    break;
                }
                r = r->next;
            }
            moveBookingToHistory(curr);         // Move this booking to past bookings list
            if(prev)                            // IF prev exists, curr is not head node
                prev->next = curr->next;                // prev->next  is pointed to curr->next unlinking curr from list
            else                                // Else curr is head node
                headBooking = curr->next;               // head is pointed to curr->next unlinking curr
            free(curr);
            printf("You have checked out. Booking cancelled and room is now available.\n");
            found = 1;
            break;
        }
        prev = curr;
        curr = curr->next;
    }
    if(!found)
        printf("Booking ID not found or not your booking.\n");
}

// Move active booking to past booking
void moveBookingToHistory(Booking *b) {
    PastBooking *pb = (PastBooking*)malloc(sizeof(PastBooking));    // Allocate PastBooking node
    // Copying bookingId, roomnum, name
    pb->bookingID = b->bookingID;                                              
    pb->roomNumber = b->roomNumber;
    strncpy(pb->customerName, b->customerName, NAME_LEN);
    pb->next = headPastBooking;             // Insert new past booking at head
    headPastBooking = pb;
}

// Displaying rooms to user
void searchRooms() {
    printf("\nRooms at NAS Hotels:\n");
    listRoomsTable();
}

// Show active and past bookings for given name
void viewUserBookings(const char *username) {
    // Looping Active bookings
    Booking *curr = headBooking; int found = 0;
    printf("--- Active Bookings ---\n");
    while(curr) {
        if(strcmp(curr->customerName, username)==0) {
            printf("BookingID: %d | Room: %d\n", curr->bookingID, curr->roomNumber);
            found = 1;
        }
        curr = curr->next;
    }
    if(!found) printf("No current bookings for %s\n", username);

    // Looping Past bookings
    PastBooking *pbc = headPastBooking; found = 0;
    printf("--- Past Bookings ---\n");
    while(pbc) {
        if(strcmp(pbc->customerName, username)==0) {
            printf("BookingID: %d | Room: %d\n", pbc->bookingID, pbc->roomNumber);
            found = 1;
        }
        pbc = pbc->next;
    }
    if(!found) printf("No past bookings for %s\n", username);
}

// For staff menu
void viewUserBookingsStaff() {
    char username[NAME_LEN];
    printf("Enter user name to view bookings: ");
    fgets(username, NAME_LEN, stdin); username[strcspn(username, "\n")] = 0;
    viewUserBookings(username);
}

// Marking rooms unavailable for active bookings
void applyBookingToRooms() {
    // MAtching active booking room numbers with the hotel room numbers
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

// Writing active bookings to a file
void saveBookingData() {
    FILE *fp = fopen(BOOKING_FILE, "wb");
    if (!fp) { 
        printf("ERROR: Cannot open file for booking save!\n"); 
        return; 
    }
    Booking *curr = headBooking;
    while(curr) {
        fwrite(curr, sizeof(Booking), 1, fp);
        curr = curr->next;
    }
    fclose(fp);
}

// Reading bookings back from file to linked list
void loadBookingData() {
    FILE *fp = fopen(BOOKING_FILE, "rb");
    if (!fp) { 
        return; 
    }
    Booking temp;
    while (fread(&temp, sizeof(Booking), 1, fp)) {
        Booking *b = (Booking*) malloc(sizeof(Booking));
        *b = temp;
        b->next = headBooking;
        headBooking = b;
    }
    fclose(fp);
}

// Writing past bookings to a file
void savePastBookingData() {
    FILE *fp = fopen(PAST_BOOKING_FILE, "wb");
    if (!fp) { printf("ERROR: Cannot open file for past booking save!\n"); return; }
    PastBooking *curr = headPastBooking;
    while(curr) {
        fwrite(curr, sizeof(PastBooking), 1, fp);
        curr = curr->next;
    }
    fclose(fp);
}

// Reading past bookings back from file to linked list
void loadPastBookingData() {
    FILE *fp = fopen(PAST_BOOKING_FILE, "rb");
    if (!fp) { return; }
    PastBooking temp;
    while (fread(&temp, sizeof(PastBooking), 1, fp)) {
        PastBooking *b = (PastBooking*) malloc(sizeof(PastBooking));
        *b = temp;
        b->next = headPastBooking;
        headPastBooking = b;
    }
    fclose(fp);
}

void viewAllCurrentGuests() {
    printf("\n--- Current Guests ---\n");
    printf("%-8s %-20s\n", "Room No", "Guest Name");   // -8s and -20s are used to left-shift the strings of 8 and 20 char spaces
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

// User features
void userMenu() {
    int choice;
    char username[NAME_LEN];
    printf("Enter your name for login/view: ");
    fgets(username, NAME_LEN, stdin); username[strcspn(username, "\n")] = 0;
    do {
        printf("\n[User: %s] - NAS Hotels\n"
               "1. Search rooms\n"
               "2. Book room\n"
               "3. Checkout (Cancel booking)\n"
               "4. View my bookings (current & past)\n"
               "5. Leave feedback\n"
               "6. Exit\nChoice: ", username);
        scanf("%d", &choice); getchar();
        if(choice==1) searchRooms();
        else if(choice==2) addBooking(username);
        else if(choice==3) cancelBookingForUser(username);
        else if(choice==4) viewUserBookings(username);
        else if(choice==5) addFeedback(username);
        else if(choice==6) break;
        else printf("Invalid input. Please enter a number between 1 and 6.\n");
    } while(choice!=6);
}

// Staff features
void staffMenu() {
    int choice;
    do {
        printf("\n[Staff] - NAS Hotels\n"
               "1. Update Room\n"
               "2. Checkout Room (remove guest from room)\n"
               "3. View Rooms\n"
               "4. Record Guest\n"
               "5. View Current Bookings\n"
               "6. View Feedback\n"
               "7. View User Bookings (current & past)\n"
               "8. Exit\nChoice: ");
        scanf("%d", &choice); 
        if(choice==1) updateRoom();
        else if(choice==2) checkoutRoomStaff();
        else if(choice==3) listRoomsTable();
        else if(choice==4) addGuest();
        else if(choice==5) viewAllCurrentGuests();
        else if(choice==6) listFeedbacks();
        else if(choice==7) viewUserBookingsStaff();
        else if(choice==8) break;
        else printf("Invalid input. Please enter a number between 1 and 8.\n");
    } while(choice!=8);
}





