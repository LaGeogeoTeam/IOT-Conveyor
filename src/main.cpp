#include <M5Unified.h>
#include "wifi/WifiManager.h"
#include "api/APIClient.h"
#include "json/JsonParser.h"
#include "rfid/RFIDManager.h"
#include "motor/MotorManager.h"
#include <stdio.h>
#include <math.h>
using namespace std;

// WIFI
const char *ssid = "La Geogeo Team";
const char *password = "LaGeogeoTeam";

//DOLIBARR
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
  //Init des moteurs servo et step
  //motorManager.initMotor();

  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setBrightness(100);
  

  //Connection to wifi
  //connectToWiFi(ssid, password);

  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setBrightness(100);
  M5.Lcd.setCursor(20, 20);
  // Init _MFRC522
  rfidManager.initMFRC522();
  M5.Lcd.println("Please put the card\n\nUID:");


}

String currentUid = "";

void loop()
{
  M5.update();
  //motorManager.stepMotor(); 
  //reconnectWiFi(ssid, password);

  // Récupérer l'UID de la carte
  String uid = rfidManager.getCardUID();

  if (uid != "") { // Si une carte est détectée
    if (uid != currentUid) { // Vérifier si c'est une nouvelle carte
      Serial.println("Nouvelle carte détectée, écriture en cours...");
      rfidManager.writeMifare1k(); // Écrire sur la carte
      currentUid = uid; // Mettre à jour l'UID courant
      delay(1000);
    } else { // Si c'est la même carte qu'avant
      Serial.println("Même carte détectée, lecture en cours...");
      rfidManager.readMifare1K(); // Lire les données sur la carte
      currentUid = ""; // Réinitialiser l'UID courant
      delay(1000);
    }
  } else {
    delay(500); // Réduire la fréquence de boucle pour économiser les ressources
  }
  
  // if (uid != "")
  // { // Si une carte est détectée
  //   if (!currentId.isEmpty()) {
  //     String payload = "{";
  //       payload += "\"product_id\":\"" + currentId + "\",";
  //       payload += "\"warehouse_id\":\"" + warehouseId + "\",";
  //       payload += "\"qty\":1";
  //       payload += "}";

  //       // Effectuer la requête POST avec le payload
  //       String postResponse = apiClient.postRequest("stockmovements", payload);
  //   }
  //   M5.Lcd.clear();
  //   M5.Lcd.println("Card Detected!");
  //   M5.Lcd.println("uid : " + uid);
  //   String response = apiClient.getRequest("products/ref/", uid);

  //   warehouseId = getJsonValue(response, "fk_default_warehouse");
  //   currentId = getJsonValue(response, "id");

  //   M5.Lcd.clear();
  //   M5.Lcd.setCursor(20, 20);
  //   M5.Lcd.println("Warehouse Id: " + warehouseId);
    
  //   motorManager.defineAngleForServoMotor(warehouseId.toInt());


  //   delay(200); // Pause pour éviter la lecture continue de la même carte
  // }
}