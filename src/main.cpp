#include <M5Unified.h>
#include "wifi/WifiManager.h"
#include "api/APIClient.h"
#include "json/JsonParser.h"
#include "rfid/reader/MFRC522_I2C.h"

#define MFRC522_I2C_ADDR 0x28

const char *ssid = "La Geogeo Team";
const char *password = "LaGeogeoTeam";
const char *baseURL = "http://146.59.232.167:8081/api/index.php/";
const char *token = "c3iwyi9dcQ2s";

APIClient apiClient(baseURL, token);

MFRC522 _MFRC522 = MFRC522(MFRC522_I2C_ADDR);

String getCardUID() {
    // Vérifie si une nouvelle carte est présente
    if (!_MFRC522.PICC_IsNewCardPresent() || !_MFRC522.PICC_ReadCardSerial()) {
        return ""; // Pas de carte détectée
    }

    // Construire l'UID sous forme de chaîne de caractères
    String uid = "";
    for (byte i = 0; i < _MFRC522.uid.size; i++) {
        if (_MFRC522.uid.uidByte[i] < 0x10) {
            uid += "0"; // Ajout d'un zéro pour les valeurs hexadécimales < 10
        }
        uid += String(_MFRC522.uid.uidByte[i], HEX); // Conversion en hexadécimal
    }

    uid.toUpperCase(); // Convertir la chaîne en majuscules (modifie en place)
    return uid; // Retourner l'UID
}



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

  // M5.Lcd.println("MFRC522 Test");
  _MFRC522.PCD_Init();  // Init _MFRC522
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