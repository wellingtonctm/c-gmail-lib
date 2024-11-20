#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "gmail_client.h"
#include "gmail_auth.h"

#define GMAIL_BASE_URL "https://www.googleapis.com/gmail/v1/users/me/"

typedef struct {
    char *data;
    size_t size;
} MemoryBuffer;

static size_t write_callback(void *ptr, size_t size, size_t nmemb, void *userdata) {
    size_t total_size = size * nmemb;
    MemoryBuffer *mem = (MemoryBuffer *)userdata;

    char *temp = realloc(mem->data, mem->size + total_size + 1);
    if (!temp) {
        fprintf(stderr, "Failed to allocate memory.\n");
        return 0;
    }

    mem->data = temp;
    memcpy(&(mem->data[mem->size]), ptr, total_size);
    mem->size += total_size;
    mem->data[mem->size] = '\0';

    return total_size;
}

char *gmail_request(const char *endpoint) {
    CURL *curl;
    CURLcode res;
    MemoryBuffer response = {NULL, 0};

    if (!endpoint) {
        fprintf(stderr, "Endpoint is required for GET request.\n");
        return NULL;
    }

    char *access_token = gmail_auth_get_access_token();

    if (!access_token) {
        fprintf(stderr, "Couldn't get an access token.\n");
        return NULL;
    }

    char url[512];
    snprintf(url, sizeof(url), "%s%s", GMAIL_BASE_URL, endpoint);

    curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "Failed to initialize CURL.\n");
        free(access_token);
        return NULL;
    }

    struct curl_slist *headers = NULL;
    char auth_header[512];
    snprintf(auth_header, sizeof(auth_header), "Authorization: Bearer %s", access_token);
    headers = curl_slist_append(headers, auth_header);

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        fprintf(stderr, "CURL error: %s\n", curl_easy_strerror(res));
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
        free(response.data);
        free(access_token);
        return NULL;
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    free(access_token);

    return response.data;
}

char *gmail_post_request(const char *endpoint, const char *payload) {
    CURL *curl;
    CURLcode res;
    MemoryBuffer response = {NULL, 0};
    
    if (!endpoint) {
        fprintf(stderr, "Endpoint is required for POST request.\n");
        return NULL;
    }

    char *access_token = gmail_auth_get_access_token();
    if (!access_token) {
        fprintf(stderr, "No access token provided.\n");
        return NULL;
    }

    if (!payload) {
        fprintf(stderr, "Payload is required for POST request.\n");
        return NULL;
    }

    char url[512];
    snprintf(url, sizeof(url), "https://www.googleapis.com/gmail/v1/users/me/%s", endpoint);

    curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "Failed to initialize CURL.\n");
        return NULL;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");

    char auth_header[256];
    snprintf(auth_header, sizeof(auth_header), "Authorization: Bearer %s", access_token);
    headers = curl_slist_append(headers, auth_header);

    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        fprintf(stderr, "CURL POST request failed: %s\n", curl_easy_strerror(res));
        free(response.data);
        response.data = NULL;
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    return response.data;
}