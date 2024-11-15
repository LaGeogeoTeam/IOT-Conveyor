#include <M5Unified.h>
#include "wifi/WifiManager.h"
#include "api/APIClient.h"


const char *ssid = "La Geogeo Team";
const char *password = "LaGeogeoTeam";
const char *baseURL = "http://146.59.232.167:8081/api/index.php/";
const char *token = "c3iwyi9dcQ2s";

APIClient apiClient(baseURL, token);

void setup() {
  auto cfg = M5.config();
  cfg.external_spk = true;
  M5.begin(cfg);
  Serial.begin(115200);

  connectToWiFi(ssid, password);

  // Exemple d'appel GET
  String response = apiClient.getRequest("products");
  M5.Lcd.println("Réponse GET :");
  M5.Lcd.println(response);

  // Exemple d'appel POST
  // String payload = "{\"ref\":\"test\",\"label\":\"Produit de test\"}";
  // response = apiClient.postRequest("products", payload);
  // M5.Lcd.println("Réponse POST :");
  // M5.Lcd.println(response);
}

void loop() {
    M5.update();
    reconnectWiFi(ssid, password);
    if (M5.BtnB.wasPressed()) {
        M5.Lcd.clear();
        M5.Lcd.println("Bouton central pressé, nouvelle requête GET...");

        // Effectuez une nouvelle requête GET
        String response = apiClient.getRequest("products");
        M5.Lcd.println("Réponse GET :");
        M5.Lcd.println(response);
    }
    delay(100);
}