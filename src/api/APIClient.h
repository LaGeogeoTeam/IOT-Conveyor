#ifndef API_CLIENT_H
#define API_CLIENT_H

#include <HTTPClient.h>
#include <ArduinoJson.h>

class APIClient {
public:
    APIClient(const char *baseURL, const char *token);
    String getRequest(const char *endpoint, const String data);
    String postRequest(const char *endpoint, const String &payload);

private:
    const char *baseURL;
    const char *token;
    HTTPClient http;
};

#endif // API_CLIENT_H
