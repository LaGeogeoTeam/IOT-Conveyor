#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <M5Unified.h>
#include <WiFi.h>

// Déclaration des fonctions
void connectToWiFi(const char *ssid, const char *password);
void reconnectWiFi(const char *ssid, const char *password);

#endif // WIFI_MANAGER_H
