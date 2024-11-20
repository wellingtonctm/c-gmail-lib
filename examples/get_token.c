#include <stdio.h>
#include <stdlib.h>
#include "gmail_auth.h"

int main() {
    const char *CLIENT_ID = "YOUR CLIENT_ID";
    const char *CLIENT_SECRET = "YOUR CLIENT_SECRET";
    const char *REFRESH_TOKEN = "YOUR REFRESH_TOKEN";

    gmail_auth_set_credentials(CLIENT_ID, CLIENT_SECRET, REFRESH_TOKEN, NULL);

    char *access_token = gmail_auth_get_access_token();

    if (access_token) {
        printf("Access Token: %s\n", access_token);
        free(access_token);
    } else {
        fprintf(stderr, "Failed to get access token.\n");
    }

    gmail_auth_cleanup();
    return 0;
}
