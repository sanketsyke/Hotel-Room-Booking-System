#include "feedback.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Feedback *headFeedback = NULL;

// Add new feedback from a user
void addFeedback(const char* username) {
    Feedback *f = (Feedback*) malloc(sizeof(Feedback));
    if (!f) {
        printf("Memory allocation failed!\n");
        return;
    }

    strncpy(f->username, username, NAME_LEN);
    f->username[NAME_LEN - 1] = '\0';  // Ensure null termination

    printf("Enter your feedback: ");
    if (fgets(f->text, FEEDBACK_LEN, stdin) != NULL) {
        f->text[strcspn(f->text, "\n")] = '\0';  // Remove trailing newline
    } else {
        f->text[0] = '\0';  // Empty feedback if input fails
    }

    f->next = headFeedback;  // Insert at head of list
    headFeedback = f;

    printf("Thank you for your feedback!\n");
}

// List all feedback entries
void listFeedbacks() {
    Feedback *f = headFeedback;
    printf("--- Feedbacks ---\n");
    if (!f) {
        printf("No feedbacks yet.\n");
        return;
    }

    while (f) {
        printf("%s: %s\n", f->username, f->text);
        f = f->next;
    }
}
