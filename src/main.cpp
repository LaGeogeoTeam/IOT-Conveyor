#include <M5Unified.h>
#include "wifi/WifiManager.h"
#include "api/APIClient.h"
#include "json/JsonParser.h"
#include "rfid/RFIDManager.h"

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

  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setBrightness(100);
  M5.Lcd.setCursor(20, 20);
  initMFRC522();  // Init _MFRC522
  M5.Lcd.println("Please put the card\n\nUID:");
}

void loop() {
  M5.update();
  reconnectWiFi(ssid, password);

  String uid = getCardUID();
  if (uid != "") { // Si une carte est détectée
      M5.Lcd.clear();
      M5.Lcd.println("Card Detected!");
      String response = apiClient.getRequest("products/ref/", uid);

      String warehouseId = getJsonValue(response, "fk_default_warehouse");
      M5.Lcd.clear();
      M5.Lcd.setCursor(20, 20);
      M5.Lcd.println("Warehouse Id: " + warehouseId);        
      delay(200); // Pause pour éviter la lecture continue de la même carte 
  }
}