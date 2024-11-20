#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gmail_auth.h"
#include "gmail_client.h"
#include "gmail_messages.h"
#include "gmail_utils.h"

int main() {
    const char *CLIENT_ID = "YOUR CLIENT_ID";
    const char *CLIENT_SECRET = "YOUR CLIENT_SECRET";
    const char *REFRESH_TOKEN = "YOUR REFRESH_TOKEN";

    if (!CLIENT_ID || !CLIENT_SECRET || !REFRESH_TOKEN) {
        fprintf(stderr, "Error: Environment variables for Gmail credentials are not set.\n");
        return 1;
    }

    gmail_auth_set_credentials(CLIENT_ID, CLIENT_SECRET, REFRESH_TOKEN, NULL);

    const char *to = "w.luiscm@gmail.com";
    const char *subject = "Este é um email de teste";
    const char *body = "Hello! I'm testing the Gmail C Library.\n\nAttentiously,\nWellington.";

    if (gmail_send_email(to, subject, body) == 0) {
        printf("Email sent successfully.\n");
    } else {
        fprintf(stderr, "Failed to send email.\n");
    }

    gmail_auth_cleanup();
    return 0;
}
