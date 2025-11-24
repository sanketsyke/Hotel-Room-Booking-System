#include "auth.h"
#include <string.h>
#include <stdio.h>
int staffLogin() {
    char password[32];                      
    printf("Enter staff password: ");
    scanf("%s", password);    
    if(strcmp(password, "nas2025") == 0)    // Comparing our entered password and the correct password
        return 1;
    else
        return 0;
}
