#include <M5Unified.h>
#include "wifi/WiFiManager.h"
#include "webserver/WebConfigServer.h"

WebConfigServer webserver;

String ssid = "";
String password = "";

void setup() {
  M5.begin();
  Serial.begin(115200);

  loadWiFiCredentials(ssid, password);

  if (ssid == "" || password == "") {
    Serial.println("Pas de WiFi config - Lancement AP Mode (premiere mise en place)...");
    startAPMode();
  } else {
    connectToWiFi(ssid, password);
    waitForWiFi();

    if (!isWiFiConnected) {
      startAPMode();
    }
  }
  webserver.begin();
}

void loop() {
  M5.update();
  reconnectWiFi(ssid, password);
}
