
# Gmail C Library

This C library provides a simple interface for interacting with the Gmail API using OAuth2 for authentication. It includes functions for authenticating with the Gmail API, sending and reading emails, and querying messages.

## Features

- Authenticate using OAuth2.
- Send and receive emails via the Gmail API.
- List messages from the inbox.
- Retrieve message details and parse them.

## Table of Contents

1. [Installation](#installation)
2. [Authentication](#authentication)
3. [Examples](#examples)
4. [Usage](#usage)
5. [Cleaning Up](#cleaning-up)

## Installation

To use this library, you'll need to set up a Google OAuth2 application and get your credentials (client ID, client secret, and refresh token).

### Prerequisites

- `gcc` (GNU Compiler Collection)
- `libcurl` (for HTTP requests)
- `libcjson` (for JSON parsing)
- `libssl` and `libcrypto` (for SSL and encryption)
- `jq` (for parsing JSON in the shell script)
- `nc` (for receiving the authorization code via netcat)

### Build the Library

Clone this repository and navigate to the project directory:

```bash
cd gmail-c-library
```

Run the `make` command to compile the source code for the examples:

```bash
make
```

This will place the compiled examples in the `build` directory.

### Authentication Helper Script

To authenticate with the Gmail API, use the provided shell script `scripts/auth_helper.sh`:

```bash
./scripts/auth_helper.sh
```

This will guide you through the OAuth2 authentication process and create a `token.json` file containing your OAuth tokens.

## Authentication

To authenticate with the Gmail API, you need to provide your `CLIENT_ID`, `CLIENT_SECRET`, and `REFRESH_TOKEN`. You can get these values by setting up a project in the [Google Developer Console](https://console.developers.google.com/).

In the examples, replace the placeholder values with your actual credentials:

```c
const char *CLIENT_ID = "YOUR_CLIENT_ID";
const char *CLIENT_SECRET = "YOUR_CLIENT_SECRET";
const char *REFRESH_TOKEN = "YOUR_REFRESH_TOKEN";
```

Once you have the credentials, you can call the `gmail_auth_set_credentials` function to authenticate:

```c
gmail_auth_set_credentials(CLIENT_ID, CLIENT_SECRET, REFRESH_TOKEN, NULL);
```

## Examples

Here are a few examples demonstrating how to use the library:

### 1. Get Access Token (`get_token.c`)

This example demonstrates how to retrieve the access token using the provided refresh token.

```bash
make get_token
./build/get_token
```

### 2. List Messages (`get_request.c`)

This example sends a `GET` request to the Gmail API to list messages in the inbox.

```bash
make get_request
./build/get_request
```

### 3. Read Email (`read_email.c`)

This example retrieves details of a specific email from Gmail.

```bash
make read_email
./build/read_email
```

### 4. Send Email (`send_email.c`)

This example sends an email via Gmail's API.

```bash
make send_email
./build/send_email
```

## Usage

Once the library is compiled, you can link it with your own applications to interact with the Gmail API. 

In your application, include the necessary headers:

```c
#include "gmail_auth.h"
#include "gmail_client.h"
#include "gmail_messages.h"
#include "gmail_utils.h"
```

Use the provided functions for authentication and making API requests, such as:

- `gmail_auth_set_credentials()`: Set up authentication credentials.
- `gmail_request()`: Send a `GET` request to the Gmail API.
- `gmail_post_request()`: Send a `POST` request to the Gmail API.
- `gmail_list_messages()`: List Gmail messages.
- `gmail_get_message()`: Get details of a specific message.
- `gmail_send_email()`: Send an email using Gmail.

Example for listing messages:

```c
const char *query = "is:unread";
int max_results = 5;
char *response = gmail_list_messages(query, max_results);

if (response) {
    printf("Response from Gmail API:
%s
", response);
    free(response);
} else {
    fprintf(stderr, "Failed to list messages.
");
}
```

## Cleaning Up

After you're done using the library, don't forget to clean up authentication credentials:

```c
gmail_auth_cleanup();
```

This will free any allocated memory and reset authentication credentials.
