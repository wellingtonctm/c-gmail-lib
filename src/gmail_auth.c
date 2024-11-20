#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <cjson/cJSON.h>
#include "gmail_auth.h"

typedef struct
{
    char *client_id;
    char *client_secret;
    char *refresh_token;
    char *access_token;
} GmailAuthConfig;

static GmailAuthConfig auth_config = {0};

typedef struct
{
    char *data;
    size_t size;
} MemoryBuffer;

static size_t write_callback(void *ptr, size_t size, size_t nmemb, void *userdata)
{
    size_t total_size = size * nmemb;
    MemoryBuffer *mem = (MemoryBuffer *)userdata;

    char *temp = realloc(mem->data, mem->size + total_size + 1);
    if (!temp)
    {
        fprintf(stderr, "Failed to allocate memory.\n");
        return 0;
    }

    mem->data = temp;
    memcpy(&(mem->data[mem->size]), ptr, total_size);
    mem->size += total_size;
    mem->data[mem->size] = '\0';

    return total_size;
}

void gmail_auth_set_credentials(const char *client_id, const char *client_secret, const char *refresh_token, const char *access_token)
{
    if (client_id)
        auth_config.client_id = strdup(client_id);

    if (client_secret)
        auth_config.client_secret = strdup(client_secret);

    if (refresh_token)
        auth_config.refresh_token = strdup(refresh_token);

    if (access_token)
        auth_config.access_token = strdup(access_token);
}

char *gmail_auth_get_client_id()
{
    if (!auth_config.client_id) {
        fprintf(stderr, "No client ID provided.\n");
        return NULL;
    }

    return strdup(auth_config.client_id);
}

char *gmail_auth_get_client_secret()
{
    if (!auth_config.client_secret) {
        fprintf(stderr, "No client secret provided.\n");
        return NULL;
    }

    return strdup(auth_config.client_secret);
}

char *gmail_auth_get_refresh_token()
{
    if (!auth_config.refresh_token) {
        fprintf(stderr, "No refresh provided.\n");
        return NULL;
    }

    return strdup(auth_config.refresh_token);
}

char *gmail_auth_get_access_token()
{
    if (auth_config.access_token)
        return strdup(auth_config.access_token);

    char *refresh_token = gmail_auth_get_refresh_token();

    if (!refresh_token)
    {
        fprintf(stderr, "Refresh token is required for requesting an access token.\n");
        return NULL;
    }

    char *client_id = gmail_auth_get_client_id();

    if (!client_id)
    {
        fprintf(stderr, "Client ID is required for requesting an access token.\n");
        return NULL;
    }

    char *client_secret = gmail_auth_get_client_secret();

    if (!client_secret)
    {
        fprintf(stderr, "Client secret is required for requesting an access token.\n");
        return NULL;
    }

    CURL *curl;
    CURLcode res;
    MemoryBuffer response = {NULL, 0};
    char *access_token = NULL;

    curl = curl_easy_init();
    if (!curl)
    {
        fprintf(stderr, "Failed to initialize CURL.\n");
        return NULL;
    }

    const char *url = "https://accounts.google.com/o/oauth2/token";
    char post_fields[512];
    snprintf(post_fields, sizeof(post_fields),
             "client_id=%s&client_secret=%s&refresh_token=%s&grant_type=refresh_token",
             client_id, client_secret, refresh_token);

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_fields);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    res = curl_easy_perform(curl);
    if (res != CURLE_OK)
    {
        fprintf(stderr, "CURL error: %s\n", curl_easy_strerror(res));
        curl_easy_cleanup(curl);
        free(response.data);
        return NULL;
    }

    cJSON *json = cJSON_Parse(response.data);
    if (json)
    {
        cJSON *token = cJSON_GetObjectItem(json, "access_token");
        if (token && cJSON_IsString(token))
        {
            access_token = strdup(token->valuestring);
        }
        else
        {
            fprintf(stderr, "Failed to parse access token.\n");
        }
        cJSON_Delete(json);
    }
    else
    {
        fprintf(stderr, "Failed to parse JSON response.\n");
    }

    curl_easy_cleanup(curl);
    free(response.data);
    return access_token;
}

// Limpeza de credenciais
void gmail_auth_cleanup()
{
    free(auth_config.client_id);
    free(auth_config.client_secret);
    free(auth_config.refresh_token);
    free(auth_config.access_token);
    auth_config.client_id = auth_config.client_secret = auth_config.refresh_token = auth_config.access_token = NULL;
}
