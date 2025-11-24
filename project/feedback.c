#include "feedback.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
Feedback *headFeedback = NULL;

void addFeedback(const char* username) {
    Feedback *f = (Feedback*)malloc(sizeof(Feedback));
    strcpy(f->username, username);
    printf("Enter your feedback: ");
    fgets(f->text, FEEDBACK_LEN, stdin);
    f->text[strcspn(f->text, "\n")] = 0;
    f->next = headFeedback;
    headFeedback = f;
    printf("Thank you for your feedback!\n");
}

void listFeedbacks() {
    Feedback *f = headFeedback;
    printf("--- Feedbacks ---\n");
    while(f) {
        printf("%s: %s\n", f->username, f->text);
        f=f->next;
    }
}
