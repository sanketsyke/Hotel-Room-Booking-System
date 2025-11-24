#include "guest.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

Guest *headGuest = NULL;

int isValidPhone(const char *phone) {
    // Must be exactly 10 digits, all numeric
    if (strlen(phone) != 10) return 0;
    for (int i = 0; i < 10; ++i) {
        if (!isdigit(phone[i])) return 0;
    }
    return 1;
}

void addGuest() {
    Guest *g = (Guest*)malloc(sizeof(Guest));
    printf("Guest name: ");
    fgets(g->name, NAME_LEN, stdin);
    g->name[strcspn(g->name,"\n")] = 0;

    char phoneInput[PHONE_LEN];
    int validPhone = 0;
    do {
        printf("Phone number (10 digits): ");
        fgets(phoneInput, PHONE_LEN, stdin);
        phoneInput[strcspn(phoneInput,"\n")] = 0;
        validPhone = isValidPhone(phoneInput);
        if (!validPhone) printf("Invalid phone number! Please enter exactly 10 digits.\n");
    } while (!validPhone);
    strcpy(g->phone, phoneInput);

    printf("Room number: ");
    scanf("%d", &g->roomNumber); getchar();

    g->next = headGuest; headGuest = g;
    printf("Guest added: %s, Phone: %s, Room: %d\n", g->name, g->phone, g->roomNumber);
}

void listGuests() {
    Guest *g = headGuest;
    printf("--- Guests ---\n");
    while(g) {
        printf("Name: %s | Phone: %s | Room: %d\n", g->name, g->phone, g->roomNumber);
        g = g->next;
    }
}

