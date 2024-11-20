#include <stdio.h>
#include <stdlib.h>
#include "gmail_auth.h"
#include "gmail_client.h"

int main() {
    const char *CLIENT_ID = "YOUR CLIENT_ID";
    const char *CLIENT_SECRET = "YOUR CLIENT_SECRET";
    const char *REFRESH_TOKEN = "YOUR REFRESH_TOKEN";

    gmail_auth_set_credentials(CLIENT_ID, CLIENT_SECRET, REFRESH_TOKEN, NULL);

    const char *endpoint = "messages?q=is:inbox&maxResults=1";
    char *response = gmail_request(endpoint);

    if (response) {
        printf("Response from Gmail API:\n%s\n", response);
        free(response);
    } else {
        fprintf(stderr, "Failed to fetch messages.\n");
    }

    gmail_auth_cleanup();
    return 0;
}
