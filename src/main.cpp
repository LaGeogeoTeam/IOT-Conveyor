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

const char *baseURL = "";
const char *token = "";

MotorManager motorManager;
RFIDManager rfidManager;
WifiManager wifiManager;
WebConfigServer webserver;

String ssid = "";
String password = "";

APIClient apiClient = APIClient(baseURL, token);

bool isWriting = false;
String dataToWrite = "";
String currentUid = "";
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
  rfidManager.loadRFIDPrefs();

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
    String uid = rfidManager.getCardUID();
  
    if (uid != "")
    {
      if (isWriting) {
        if(dataToWrite == ""){
          rfidManager.loadRFIDPrefs();
        }
        uid.trim();
        currentUid.trim();
        if (uid != currentUid){
          Serial.println("Nouvelle carte détectée, écriture en cours...");
                byte *byteArray = (byte*) dataToWrite.c_str();
                rfidManager.writeMifare1k(byteArray); // Écrire sur la carte
                currentUid = uid;    // Mettre à jour l'UID courant
                delay(1000);
        } else {
          Serial.println("Même carte détectée, lecture en cours...");
          rfidManager.readMifare1K(); // Lire les données sur la carte
          delay(1000);
        }
      } else {
        if(uid != ""){
          String dataRead = rfidManager.readMifare1K();
          Serial.println("dataRead: " + dataRead);
          String response = apiClient.getRequest("/products/ref/", dataRead);
          Serial.println(response);

          String warehouseId = getJsonValue(response, "fk_default_warehouse");
          String id = getJsonValue(response, "id");
          Serial.print("[Main] Warehouse Id: ");
          Serial.println(warehouseId);
          motorManager.defineAngleForServoMotor(warehouseId.toInt());
          String payload = "{";
                payload += "\"product_id\":\"" + id + "\",";
                payload += "\"warehouse_id\":\"" + warehouseId + "\",";
                payload += "\"qty\":1";
                payload += "}";

          String postResponse = apiClient.postRequest("/stockmovements", payload);
        }
      } 
    }      
  }
  //
}
