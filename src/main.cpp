#include <M5Unified.h>
#include "wifi/WiFiManager.h"
#include "webserver/WebConfigServer.h"
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

// DOLIBARR
const char *baseURL = "http://146.59.232.167:8081/api/index.php/";
const char *token = "c3iwyi9dcQ2s";

APIClient apiClient(baseURL, token);
MotorManager motorManager;
RFIDManager rfidManager;
WifiManager wifiManager;
WebConfigServer webserver;


// WIFI
String ssid = "";
String password = "";

void setup()
{
  auto cfg = M5.config();
  cfg.external_spk = true;
  Serial.begin(115200);
  M5.begin(cfg);
  M5.Power.begin();

  // WIFI and WebServer
  wifiManager.loadWiFiCredentials(ssid, password);

  if (ssid == "" || password == "")
  {
    Serial.println("Pas de WiFi config - Lancement AP Mode (premiere mise en place)...");
    wifiManager.startAPMode();
  }
  else
  {
    wifiManager.connectToWiFi(ssid, password);
    wifiManager.waitForWiFi();

    if (!isWiFiConnected)
    {
      wifiManager.startAPMode();
    }
  }
  webserver.begin();
  // Init des moteurs servo et step
  motorManager.initMotor();

  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setBrightness(100);

  // Init RFID
  rfidManager.initMFRC522();
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
  motorManager.startStepMotor();
  rfidManager.rfidConveyor(baseURL, token);

  wifiManager.reconnectWiFi(ssid, password);
}
