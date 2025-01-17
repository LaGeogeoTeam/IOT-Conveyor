#include <M5Unified.h>
#include "wifi/WifiManager.h"
#include "api/APIClient.h"
#include "json/JsonParser.h"
#include "rfid/RFIDManager.h"
#include "motor/MotorManager.h"
#include <stdio.h>
#include <math.h>
#include <Preferences.h>
Preferences preferences;
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

String currentId;
String warehouseId;

void setup()
{
  auto cfg = M5.config();
  cfg.external_spk = true;
  Serial.begin(115200);
  M5.begin(cfg);
  M5.Power.begin();
  // Init des moteurs servo et step
  // motorManager.initMotor();

  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setBrightness(100);

  // Connection to wifi
  // connectToWiFi(ssid, password);

  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setBrightness(100);
  M5.Lcd.setCursor(20, 20);
  // Init _MFRC522
  rfidManager.initMFRC522();
  M5.Lcd.println("Please put the card\n\nUID:");
  preferences.begin("rfid", false);
  // preferences.putBool("rwMode", false);
}

void loop()
{
  // get the rfid mode from nvs
  bool rfidMode = preferences.getBool("rwMode");
  M5.Lcd.setCursor(20, 150);
  M5.Lcd.println("RFID current mode: ");
  M5.Lcd.setCursor(240, 150);
  M5.Lcd.println(rfidMode);
  M5.update();
  // motorManager.stepMotor();
  // reconnectWiFi(ssid, password);
  rfidManager.rfidConveyor();
}
