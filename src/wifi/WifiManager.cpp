#include "WiFiManager.h"

bool isWiFiConnected = false;

void connectToWiFi(const char *ssid, const char *password) {
    M5.Lcd.println("Connexion au WiFi...");
    WiFi.begin(ssid, password);

    int retryCount = 0;
    while (WiFi.status() != WL_CONNECTED && retryCount < 20) {
        delay(500);
        M5.Lcd.print(".");
        retryCount++;
    }

    if (WiFi.status() == WL_CONNECTED) {
        isWiFiConnected = true;
        M5.Lcd.clear();
        M5.Lcd.setCursor(0, 0);
        M5.Lcd.println("\nConnecte !");
        M5.Lcd.print("Adresse IP : ");
        M5.Lcd.println(WiFi.localIP());
    } else {
        isWiFiConnected = false;
        M5.Lcd.println("\nÉchec de connexion.");
    }
}

void reconnectWiFi(const char *ssid, const char *password) {
    if (WiFi.status() != WL_CONNECTED) {
        if (isWiFiConnected) {
            M5.Lcd.println("\nDeconnexion detectee. Tentative de reconnexion...");
            isWiFiConnected = false;
        }
        connectToWiFi(ssid, password);
    } else {
        if (!isWiFiConnected) {
            M5.Lcd.clear();
            M5.Lcd.setCursor(0, 0);
            M5.Lcd.println("Reconnecte !");
            isWiFiConnected = true;
        }
    }
}

void waitForWiFi() {
    int retryCount = 0;
    while (WiFi.status() != WL_CONNECTED && retryCount < 20) {
        M5.Lcd.println("Attente de la connexion WiFi...");
        delay(500);
        retryCount++;
    }
    if (WiFi.status() != WL_CONNECTED) {
        M5.Lcd.clear();
        M5.Lcd.setCursor(0, 0);
        M5.Lcd.println("Connexion WiFi échouée !");
    }
}
