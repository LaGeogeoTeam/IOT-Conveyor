#include "APIClient.h"
#include "wifi/WifiManager.h"   // Pour wifiManager.waitForWiFi()
#include <M5Unified.h>          // Pour afficher "API not configured"
#include <Preferences.h>

static Preferences apiPrefs;    // Preferences pour l'API

HTTPClient http;

APIClient::APIClient(const char *baseURL, const char *token)
{
    this->baseURL = (baseURL != nullptr) ? baseURL : "";
    this->token   = (token   != nullptr) ? token   : "";

    if (this->baseURL == "" || this->token == "") {
        loadApiFromPreferences();
    }
    if (this->baseURL == "" || this->token == "") {
        M5.Lcd.clear();
        M5.Lcd.setCursor(0, 0);
        M5.Lcd.println("API not configured");
    } else {
        saveApiToPreferences();
    }
}

// --------------------- GET REQUEST ---------------------
String APIClient::getRequest(const char *endpoint, const String data) {
    wifiManager.waitForWiFi();

    // Vérifier la connexion
    if (WiFi.status() != WL_CONNECTED) {
        return "Erreur : WiFi non connecté";
    }

    // Vérifier si API configurée
    if (baseURL == "" || token == "") {
        try
        {
           loadApiFromPreferences();
        }
        catch(const std::exception& e)
        {
            return e.what();
        }        
    }

    String url = String(baseURL) + String(endpoint) + data;
    http.begin(url);
    http.addHeader("DOLAPIKEY", token);
    http.addHeader("User-Agent", "ESP32Client");
    http.addHeader("Connection", "keep-alive");
    http.addHeader("Accept", "*/*");

    int httpResponseCode = http.GET();
    if (httpResponseCode > 0) {
        String response = http.getString();
        http.end();
        return response;
    } else {
        String error = http.errorToString(httpResponseCode).c_str();
        http.end();
        return "Erreur : " + String(httpResponseCode) + " - " + error;
    }
}

// --------------------- POST REQUEST ---------------------
String APIClient::postRequest(const char *endpoint, const String &payload) {
    wifiManager.waitForWiFi();

    // Vérifier si API configurée
    if (baseURL == "" || token == "") {
        return "Erreur : API non configuree";
    }

    String url = String(baseURL) + String(endpoint);
    http.begin(url);
    http.addHeader("DOLAPIKEY", token);
    http.addHeader("Content-Type", "application/json");
    int httpResponseCode = http.POST(payload);
    
    if (httpResponseCode > 0) {
        String response = http.getString();
        http.end();
        return response;
    } else {
        http.end();
        return "Erreur : " + String(httpResponseCode);
    }
}

// --------------------- Charger depuis Preferences ---------------------
void APIClient::loadApiFromPreferences() {
    apiPrefs.begin("ApiConfig", true); // true = lecture
    String savedURL = apiPrefs.getString("apiURL", "");
    String savedToken = apiPrefs.getString("token", "");
    apiPrefs.end();

    if (savedURL != "")  this->baseURL = savedURL;
    if (savedToken != "") this->token   = savedToken;

    Serial.println("[APIClient] loadApiFromPreferences:");
    Serial.println(" baseURL = " + baseURL);
    Serial.println(" token   = " + token);
}

// --------------------- Sauvegarder dans Preferences ---------------------
void APIClient::saveApiToPreferences() {
    apiPrefs.begin("ApiConfig", false); // false = écriture
    apiPrefs.putString("apiURL", baseURL);
    apiPrefs.putString("token",   token);
    apiPrefs.end();

    Serial.println("[APIClient] saveApiToPreferences:");
    Serial.println(" baseURL = " + baseURL);
    Serial.println(" token   = " + token);
}
