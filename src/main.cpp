#include <M5Unified.h>
#include "wifi/WiFiManager.h"

const char *ssid = "La Geogeo Team";
const char *password = "LaGeogeoTeam";

void setup() {
    auto cfg = M5.config();
    cfg.external_spk = true; // Active le haut-parleur externe
    M5.begin(cfg);
    Serial.begin(115200);

    // Appeler la fonction de connexion
    connectToWiFi(ssid, password);
}

void loop() {
    M5.update();

    // Appeler la fonction de reconnexion dans la boucle principale
    reconnectWiFi(ssid, password);

    // Afficher la puissance du signal si connecté
    if (WiFi.status() == WL_CONNECTED) {
        M5.Lcd.setCursor(0, 40);
        M5.Lcd.printf("Signal WiFi : %d dBm", WiFi.RSSI());
    }

    delay(1000);
}