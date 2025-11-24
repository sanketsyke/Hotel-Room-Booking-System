#ifndef FEEDBACK_H
#define FEEDBACK_H
#include "constants.h"

typedef struct Feedback {
    char username[NAME_LEN];
    char text[FEEDBACK_LEN];
    struct Feedback* next;
} Feedback;

void addFeedback(const char* username);
void listFeedbacks();

#endif
