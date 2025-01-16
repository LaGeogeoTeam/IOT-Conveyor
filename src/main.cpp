#include <M5Unified.h>
#include "wifi/WifiManager.h"
#include "api/APIClient.h"
#include "json/JsonParser.h"
#include "rfid/RFIDManager.h"
#include "motor/MotorManager.h"
#include <stdio.h>
#include <math.h>
#include <M5BurnerNVS.h>

using namespace std;

// WIFI
const char *ssid = "La Geogeo Team";
const char *password = "LaGeogeoTeam";

// DOLIBARR
const char *baseURL = "http://146.59.232.167:8081/api/index.php/";
const char *token = "c3iwyi9dcQ2s";

APIClient apiClient(baseURL, token);
MotorManager motorManager;
RFIDManager rfidManager;
M5BurnerNVS burner_nvs;

void setup()
{
  Serial.begin(115200);
  M5.begin();
  M5.Power.begin();
  // Init des moteurs servo et step
  motorManager.initMotor();

  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setBrightness(100);

  auto cfg = M5.config();
  cfg.external_spk = true;

  // Connection to wifi
  connectToWiFi(ssid, password);

  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setBrightness(100);
  M5.Lcd.setCursor(20, 20);
  // Init _MFRC522
  rfidManager.initMFRC522();
  M5.Lcd.println("Please put the card\n\nUID:");

  burner_nvs.init();
  burner_nvs.listen();

  burner_nvs.set("key", "prout");
}

void loop()
{
  M5.update();
  // motorManager.stepMotor();

  reconnectWiFi(ssid, password);

  Serial.println(burner_nvs.get("key"));
  String uid = rfidManager.getCardUID();
  if (uid != "")
  { // Si une carte est détectée
    M5.Lcd.clear();
    M5.Lcd.println("Card Detected!");
    M5.Lcd.println("uid : " + uid);
    String response = apiClient.getRequest("products/ref/", uid);

    String warehouseId = getJsonValue(response, "fk_default_warehouse");
    M5.Lcd.clear();
    M5.Lcd.setCursor(20, 20);
    M5.Lcd.println("Warehouse Id: " + warehouseId);

    motorManager.defineAngleForServoMotor(warehouseId.toInt());

    delay(1000); // Pause pour éviter la lecture continue de la même carte
  }
}