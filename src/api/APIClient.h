#ifndef APICLIENT_H
#define APICLIENT_H

#include <Arduino.h>
#include <HTTPClient.h>
#include "wifi/WifiManager.h"

class APIClient {
public:
    // Constructeur : on peut fournir baseURL/token ou laisser vides
    APIClient(const char* baseURL = "", const char* token = "");

    // Méthodes pour faire les requêtes
    String getRequest(const char *endpoint, const String data = "");
    String postRequest(const char *endpoint, const String &payload);

    // Getters
    String getBaseURL() const { return this->baseURL; }
    String getToken() const   { return this->token;  }

private:
    // Chargement et sauvegarde dans Preferences
    void loadApiFromPreferences();
    void saveApiToPreferences();

    // Membres
    String baseURL;
    String token;
    WifiManager wifiManager;
};

#endif
