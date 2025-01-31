#ifndef WEBCONFIGSERVER_H
#define WEBCONFIGSERVER_H

#include <ESPAsyncWebServer.h>
#include <Preferences.h>
#include <Arduino.h>
#include "rfid/RFIDManager.h"

class WebConfigServer {
public:
    WebConfigServer();
    void begin();

    void setApiURL(const String &url) { apiURL = url; }
    void setToken(const String &tkn) { token = tkn; }

private:
    void setupServer();
    void savePreferences();
    void resetPreferences();
    String generateConfigPage(const String &ssid, const String &password, const String &apiURL, const String &apiToken);

    AsyncWebServer server; 
    Preferences preferences;    

    String ssid;
    String password;
    String apiURL;
    String token;
};

#endif // WEBCONFIGSERVER_H
