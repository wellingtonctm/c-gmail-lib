#ifndef GMAIL_AUTH_H
#define GMAIL_AUTH_H

/**
 * Set authentication credentials.
 * 
 * @param client_id The client ID for the application (optional for access token only use).
 * @param client_secret The client secret for the application (optional for access token only use).
 * @param refresh_token The refresh token obtained during the OAuth flow (optional if access token is provided).
 * @param access_token The access token to be used directly for authenticated requests (optional).
 */
void gmail_auth_set_credentials(const char *client_id, const char *client_secret, 
                                const char *refresh_token, const char *access_token);

/**
 * Get the client ID.
 * 
 * @return A newly allocated string containing the client ID, or NULL if not set.
 *         Caller is responsible for freeing the returned string.
 */
char *gmail_auth_get_client_id();

/**
 * Get the client secret.
 * 
 * @return A newly allocated string containing the client secret, or NULL if not set.
 *         Caller is responsible for freeing the returned string.
 */
char *gmail_auth_get_client_secret();

/**
 * Get the refresh token.
 * 
 * @return A newly allocated string containing the refresh token, or NULL if not set.
 *         Caller is responsible for freeing the returned string.
 */
char *gmail_auth_get_refresh_token();

/**
 * Get the access token.
 * 
 * If an access token is already set, it will be returned. If no access token is
 * set, it attempts to obtain a new one using the refresh token flow.
 * 
 * @return A newly allocated string containing the access token, or NULL on failure.
 *         Caller is responsible for freeing the returned string.
 */
char *gmail_auth_get_access_token();

/**
 * Clean up all allocated authentication credentials.
 * Frees memory associated with client ID, client secret, refresh token,
 * and access token, and resets them to NULL.
 */
void gmail_auth_cleanup();

#endif // GMAIL_AUTH_H
