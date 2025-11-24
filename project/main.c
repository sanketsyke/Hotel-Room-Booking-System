#include <stdio.h>
#include "booking.h"
#include "room.h"
#include "feedback.h"
#include "auth.h"

int main() {
    printf("\n======== NAS Hotels Management System ========\n");
    initializeRooms();
    loadBookingData();
    loadPastBookingData();
    applyBookingToRooms();

int role;
do {
    printf("\nAre you?\n1. User\n2. Hotel Staff\n3. Exit\nYour choice: ");
    scanf("%d", &role); getchar();
    if(role==1) userMenu();
    else if(role==2) {
        if(staffLogin()) staffMenu();
        else printf("Access denied!\n");
    }
    else if(role==3) break;
    else printf("Invalid input. Please enter 1, 2, or 3.\n");
} while(role!=3);


    saveBookingData();
    savePastBookingData();
    printf("Thank you for using NAS Hotels System!\n");
    return 0;
}
