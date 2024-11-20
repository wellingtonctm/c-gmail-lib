#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cjson/cJSON.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include "gmail_utils.h"

int gmail_parse_message_details(const char *message_json, GmailMessageDetails *details) {
    if (!message_json || !details) {
        fprintf(stderr, "Invalid input to gmail_parse_message_details.\n");
        return -1;
    }

    cJSON *json = cJSON_Parse(message_json);
    if (!json) {
        fprintf(stderr, "Failed to parse JSON: %s\n", cJSON_GetErrorPtr());
        return -1;
    }

    details->from = NULL;
    details->subject = NULL;
    details->snippet = NULL;

    cJSON *payload = cJSON_GetObjectItem(json, "payload");
    if (!payload) {
        fprintf(stderr, "Payload not found in JSON.\n");
        cJSON_Delete(json);
        return -1;
    }

    cJSON *headers = cJSON_GetObjectItem(payload, "headers");
    if (!headers || !cJSON_IsArray(headers)) {
        fprintf(stderr, "Headers not found or invalid in JSON.\n");
        cJSON_Delete(json);
        return -1;
    }

    cJSON *header = NULL;
    cJSON_ArrayForEach(header, headers) {
        cJSON *name = cJSON_GetObjectItem(header, "name");
        cJSON *value = cJSON_GetObjectItem(header, "value");

        if (name && value && strcmp(name->valuestring, "From") == 0) {
            details->from = strdup(value->valuestring);
        }

        if (name && value && strcmp(name->valuestring, "Subject") == 0) {
            details->subject = strdup(value->valuestring);
        }
    }

    cJSON *snippet = cJSON_GetObjectItem(json, "snippet");
    if (snippet && cJSON_IsString(snippet)) {
        details->snippet = strdup(snippet->valuestring);
    }

    cJSON_Delete(json);

    if (!details->from || !details->subject || !details->snippet) {
        fprintf(stderr, "Failed to extract required fields.\n");
        free(details->from);
        free(details->subject);
        free(details->snippet);
        return -1;
    }

    return 0;
}

char* gmail_base64_encode(const unsigned char *input, size_t length) {
    BIO *bio, *b64;
    BUF_MEM *bufferPtr;

    b64 = BIO_new(BIO_f_base64());
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
    bio = BIO_new(BIO_s_mem());
    bio = BIO_push(b64, bio);

    BIO_write(bio, input, length);
    BIO_flush(bio);

    BIO_get_mem_ptr(bio, &bufferPtr);

    char *encoded = (char*)malloc(bufferPtr->length + 1);
    if (!encoded) {
        BIO_free_all(bio);
        return NULL;
    }

    memcpy(encoded, bufferPtr->data, bufferPtr->length);
    encoded[bufferPtr->length] = '\0';

    BIO_free_all(bio);

    return encoded;
}