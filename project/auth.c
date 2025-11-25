#include "auth.h"
#include <string.h>
#include <stdio.h>

// Verify staff login by matching password input with stored password
int staffLogin() {
    char password[32];
    printf("Enter staff password: ");
    scanf("%31s", password);  // Limit input to avoid overflow

    if (strcmp(password, "nas2025") == 0)
        return 1;  // Correct password, login successful
    else
        return 0;  // Incorrect password
}
