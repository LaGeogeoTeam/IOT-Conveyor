#ifndef APICLIENT_H
#define APICLIENT_H

#include <Arduino.h>
#include <HTTPClient.h>
#include "wifi/WifiManager.h"

class APIClient {
public:
    APIClient(const char* baseURL = "", const char* token = "");

    String getRequest(const char *endpoint, const String data = "");
    String postRequest(const char *endpoint, const String &payload);

    String getBaseURL() const { return this->baseURL; }
    String getToken() const   { return this->token;  }

private:
    void loadApiFromPreferences();
    void saveApiToPreferences();

    String baseURL;
    String token;
    WifiManager wifiManager;
};

#endif
