#include <stdio.h>
#include <stdlib.h>
#include "gmail_auth.h"
#include "gmail_client.h"
#include "gmail_messages.h"
#include "gmail_utils.h"

int main() {
    const char *CLIENT_ID = "YOUR CLIENT_ID";
    const char *CLIENT_SECRET = "YOUR CLIENT_SECRET";
    const char *REFRESH_TOKEN = "YOUR REFRESH_TOKEN";

    gmail_auth_set_credentials(CLIENT_ID, CLIENT_SECRET, REFRESH_TOKEN, NULL);

    const char *query = "is:unread";
    int max_results = 5;

    char *response = gmail_list_messages(query, max_results);

    if (response) {
        printf("Response from Gmail API:\n%s\n", response);
        free(response);
    } else {
        fprintf(stderr, "Failed to list messages.\n");
    }

    const char *message_id = "1934168443b17e45";

    response = gmail_get_message(message_id);
    if (!response) {
        fprintf(stderr, "Failed to fetch message details.\n");
        return 1;
    }

    GmailMessageDetails details;
    if (gmail_parse_message_details(response, &details) == 0) {
        printf("Message Details:\n");
        printf("From: %s\n", details.from);
        printf("Subject: %s\n", details.subject);
        printf("Snippet: %s\n", details.snippet);

        free(details.from);
        free(details.subject);
        free(details.snippet);
    } else {
        fprintf(stderr, "Failed to parse message details.\n");
    }

    free(response);
    gmail_auth_cleanup();
    return 0;
}
