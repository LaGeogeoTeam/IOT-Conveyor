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

bool isWriting = false;
String dataToWrite = "";

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
    // Démarrer le moteur en continu
    motorManager.startStepMotor();

    // On essaye de lire la carte pour détecter sa présence
    // readCardData() renvoie par ex. l'UID ou une chaîne vide si pas de carte
    String cardData = rfidManager.readCardData();

    if (isWriting) {
      // Si on est en mode écriture, on n'écrit que si une carte est détectée
      if (cardData != "") {
        Serial.print("[Main] Carte detectee. Ecriture RFID: ");
        Serial.println(dataToWrite);

        // Conversion du String dataToWrite en octets (16 max)
        rfidManager.writeMifare1k((byte*) dataToWrite.c_str());

        Serial.println("[Main] Ecriture RFID terminee!");
        // Optionnel: Remettre isWriting à false si on ne veut qu'une écriture unique
        // isWriting = false;
      }
    } else {
      // Mode lecture : on lit uniquement si carte détectée
      if(cardData != ""){
        Serial.println("[Main] Lecture RFID: " + cardData);

        // Exemple: Requete Dolibarr
        String data = apiClient.getRequest("/products/", cardData);
        Serial.println(data);

        String name = getJsonValue(data, "ref");
        Serial.print("[Main] Nom produit: ");
        Serial.println(name);
      }
    }
  }
  //motorManager.defineAngleForServoMotor(1);
}
