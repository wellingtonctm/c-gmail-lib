#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gmail_client.h"
#include "gmail_utils.h"
#include "gmail_messages.h"

char *gmail_list_messages(const char *query, int max_results) {
    char endpoint[512];
    if (query && max_results > 0) {
        snprintf(endpoint, sizeof(endpoint), "messages?q=%s&maxResults=%d", query, max_results);
    } else if (query) {
        snprintf(endpoint, sizeof(endpoint), "messages?q=%s", query);
    } else if (max_results > 0) {
        snprintf(endpoint, sizeof(endpoint), "messages?maxResults=%d", max_results);
    } else {
        snprintf(endpoint, sizeof(endpoint), "messages");
    }

    char *response = gmail_request(endpoint);
    if (!response) {
        fprintf(stderr, "Failed to fetch messages.\n");
        return NULL;
    }

    return response;
}

char *gmail_get_message(const char *message_id) {
    if (!message_id) {
        fprintf(stderr, "Message ID is required.\n");
        return NULL;
    }

    char endpoint[512];
    snprintf(endpoint, sizeof(endpoint), "messages/%s", message_id);

    char *response = gmail_request(endpoint);
    if (!response) {
        fprintf(stderr, "Failed to fetch message details.\n");
        return NULL;
    }

    return response;
}

int gmail_send_email(const char *to, const char *subject, const char *body) {
    if (!to || !subject || !body) {
        fprintf(stderr, "All fields are required to send an email.\n");
        return -1;
    }

    char mime_message[2048];
    snprintf(mime_message, sizeof(mime_message),
             "From: me\r\n"
             "To: %s\r\n"
             "Subject: %s\r\n\r\n"
             "%s",
             to, subject, body);

    char *encoded_message = gmail_base64_encode((unsigned char *)mime_message, strlen(mime_message));
    if (!encoded_message) {
        fprintf(stderr, "Failed to encode email message.\n");
        return -1;
    }

    char payload[4096];
    snprintf(payload, sizeof(payload),
             "{\"raw\": \"%s\"}",
             encoded_message);
    free(encoded_message);

    char *response = gmail_post_request("messages/send", payload);
    if (!response) {
        fprintf(stderr, "Failed to send email.\n");
        return -1;
    }

    printf("Email sent successfully: %s\n", response);
    free(response);

    return 0;
}