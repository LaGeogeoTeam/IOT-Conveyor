#include <M5Unified.h>
#include "wifi/WifiManager.h"
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

int cpt = 0;

void setup()
{
  auto cfg = M5.config();
  cfg.external_spk = true;
  Serial.begin(115200);
  M5.begin(cfg);
  M5.Power.begin();
  
  // Init des moteurs servo et step
  motorManager.initMotor();
}

void loop()
{
  M5.update();
  motorManager.startStepMotor();
}
