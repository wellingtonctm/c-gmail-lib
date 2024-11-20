#!/bin/bash

CLIENT_ID='YOUR CLIENT_ID'
CLIENT_SECRET='YOUR CLIENT_SECRET'
ACCESS_TYPE='offline'
REDIRECT_URI='http://localhost:5003'
SCOPE='https://www.googleapis.com/auth/gmail.send https://www.googleapis.com/auth/gmail.modify'

echo -n "https://accounts.google.com/o/oauth2/v2/auth?"
echo -n "scope=${SCOPE// /%20}&"
echo -n "access_type=${ACCESS_TYPE}&"
echo -n "response_type=code&"
echo -n "redirect_uri=${REDIRECT_URI}&"
echo -n "client_id=${CLIENT_ID}&"
echo -n "prompt=consent"
echo -e "\n"

echo "Waiting for the authorization code at $REDIRECT_URI..."

RESPONSE="HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 2\r\n\r\nOK"
CODE=$(echo -e "$RESPONSE" | nc -l -p 5003 | while read line; do
  grep -Po 'code=\K[^\s&]+' <<< $line
done)

if [ -z "$CODE" ]; then
  echo "No authorization code received. Exiting."
  exit 1
fi

echo -e "Authorization code received: $CODE\n"

curl -sSf -X POST "https://oauth2.googleapis.com/token" \
  -H "Content-Type: application/x-www-form-urlencoded" \
  -d "code=${CODE}" \
  -d "client_id=${CLIENT_ID}" \
  -d "client_secret=${CLIENT_SECRET}" \
  -d "redirect_uri=${REDIRECT_URI}" \
  -d "grant_type=authorization_code" > token.json

cat token.json | jq

exit 0
