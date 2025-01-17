#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H

#include <M5Unified.h>
#include <WiFi.h>
#include <Preferences.h>

extern bool isWiFiConnected;

enum WifiModeState {
    MY_WIFI_MODE_STA,
    MY_WIFI_MODE_AP
};
extern WifiModeState currentWifiMode;

void loadWiFiCredentials(String &ssid, String &password);
void saveWiFiCredentials(const String &ssid, const String &password);
void connectToWiFi(const String &ssid, const String &password);
void reconnectWiFi(const String &ssid, const String &password);
void waitForWiFi();
void startAPMode();
void tryReconnectionAP(const String &ssid, const String &password);

#endif // WIFIMANAGER_H
