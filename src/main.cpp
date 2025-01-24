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
const char *baseURL = "";
const char *token = "";

//APIClient apiClient(baseURL, token);
MotorManager motorManager;
RFIDManager rfidManager;
WifiManager wifiManager;
WebConfigServer webserver;


// WIFI
String ssid = "";
String password = "";

APIClient apiClient = APIClient(baseURL, token);
void setup()
{
  auto cfg = M5.config();
  cfg.external_spk = true;
  Serial.begin(115200);
  M5.begin(cfg);
  M5.Power.begin();
  
  // Init des moteurs servo et step
  motorManager.initMotor();

  // Init du RFID
  rfidManager.initMFRC522();

  // Init du Wifi
  wifiManager.loadWiFiCredentials(ssid, password);
  if(ssid == "" || password == ""){
    wifiManager.startAPMode();
  } else {
    wifiManager.connectToWiFi(ssid, password);
    wifiManager.waitForWiFi();

    if (!isWiFiConnected) {
      wifiManager.startAPMode();
    }
  }
  webserver.begin(); 
}

void loop()
{
  M5.update();
  if(isWiFiConnected){
    motorManager.startStepMotor(); 
    String test = rfidManager.readCardData();
    if(test != ""){
      Serial.println(test);
    }
    String id = "1";//rfidManager.readCardData();
    if(id != ""){
      String data = apiClient.getRequest("/products/", id);
      Serial.println(data);
      String name = getJsonValue(data, "ref");
      Serial.println(name);
    }
  }
  
  //motorManager.defineAngleForServoMotor(1);
  
  // if(id != ""){
  //   String data = apiClient.getRequest("thirdparties", "ref="+id);
  //   String name = getJsonValue(data, "name");
  //   Serial.println(name);
  // }

}
