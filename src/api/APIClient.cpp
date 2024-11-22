#include "APIClient.h"
#include "wifi/WifiManager.h"

HTTPClient http;

APIClient::APIClient(const char *baseURL, const char *token) {
    this->baseURL = baseURL;
    this->token = token;
}

String APIClient::getRequest(const char *endpoint, const String data) {
    waitForWiFi();

    if (WiFi.status() != WL_CONNECTED) {
        return "Erreur : WiFi non connecté";
    }

    String url = String(baseURL) + String(endpoint);
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


String APIClient::postRequest(const char *endpoint, const String &payload) {
    waitForWiFi();
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
