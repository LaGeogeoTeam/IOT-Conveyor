#include "WifiManager.h"

bool isWiFiConnected = false;
WifiModeState currentWifiMode = MY_WIFI_MODE_STA;

Preferences wifiPrefs;

void WifiManager::loadWiFiCredentials(String &ssid, String &password) {
    wifiPrefs.begin("WiFiSettings", true); // Lecture seule
    ssid     = wifiPrefs.getString("ssid",     ssid);
    password = wifiPrefs.getString("password", password);
    wifiPrefs.end();
}


void WifiManager::saveWiFiCredentials(const String &ssid, const String &password) {
    wifiPrefs.begin("WiFiSettings", false); // Écriture
    wifiPrefs.putString("ssid",     ssid);
    wifiPrefs.putString("password", password);
    wifiPrefs.end();
}

void WifiManager::connectToWiFi(const String &ssid, const String &password) {
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(), password.c_str());

    M5.Lcd.clear();
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.println("Connexion WiFi...");

    int retryCount = 0;
    while (WiFi.status() != WL_CONNECTED && retryCount < 20) {
        delay(500);
        retryCount++;
        M5.Lcd.print(".");
    }

    if (WiFi.status() == WL_CONNECTED) {
        isWiFiConnected = true;
        currentWifiMode = MY_WIFI_MODE_STA;

        M5.Lcd.clear();
        M5.Lcd.setCursor(0, 0);
        M5.Lcd.print("IP: ");
        M5.Lcd.println(WiFi.localIP());
    } else {
        isWiFiConnected = false;
        currentWifiMode = MY_WIFI_MODE_STA;
        M5.Lcd.clear();
        M5.Lcd.setCursor(0, 0);
        M5.Lcd.println("Echec connexion.");
    }
}

void WifiManager::reconnectWiFi(const String &ssid, const String &password) {
    if (currentWifiMode == MY_WIFI_MODE_STA) {
        if (WiFi.status() != WL_CONNECTED) {
            if (isWiFiConnected) {
                isWiFiConnected = false;
                M5.Lcd.clear();
                M5.Lcd.setCursor(0, 0);
                M5.Lcd.println("Deconnexion detectee...");
            }
            connectToWiFi(ssid, password);
        } else {
            if (!isWiFiConnected) {
                isWiFiConnected = true;
                currentWifiMode = MY_WIFI_MODE_STA;
                M5.Lcd.clear();
                M5.Lcd.setCursor(0, 0);
                M5.Lcd.print("IP: ");
                M5.Lcd.println(WiFi.localIP());
            }
        }
    }
    else if (currentWifiMode == MY_WIFI_MODE_AP) {
        tryReconnectionAP(ssid, password);
    }
}

void WifiManager::waitForWiFi() {
    int retryCount = 0;
    while (WiFi.status() != WL_CONNECTED && retryCount < 20) {
        delay(500);
        retryCount++;
    }
    if (WiFi.status() != WL_CONNECTED) {
        M5.Lcd.clear();
        M5.Lcd.setCursor(0, 0);
        M5.Lcd.println("Connexion WiFi echouee!");
    }
}

void WifiManager::startAPMode() {
    WiFi.mode(WIFI_AP);
    WiFi.softAP("M5AP", "12345678");
    IPAddress apIP = WiFi.softAPIP();

    isWiFiConnected = false;
    currentWifiMode = MY_WIFI_MODE_AP;

    M5.Lcd.clear();
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.println("Mode AP actif:");
    M5.Lcd.println("SSID: M5AP / Pass: 12345678");
    M5.Lcd.print("AP IP: ");
    M5.Lcd.println(apIP);
}

void WifiManager::tryReconnectionAP(const String &ssid, const String &password) {
    static unsigned long lastAttempt = 0;
    unsigned long now = millis();

    // Tenter de se reconnecter toutes les 10 secondes
    if (now - lastAttempt > 60000) {
        lastAttempt = now;
        Serial.println("AP: Tentative de reconnexion reseau...");

        WiFi.mode(WIFI_STA);
        WiFi.begin(ssid.c_str(), password.c_str());

        int retry = 0;
        while (WiFi.status() != WL_CONNECTED && retry < 5) {
            delay(500);
            retry++;
        }

        if (WiFi.status() == WL_CONNECTED) {
            isWiFiConnected = true;
            currentWifiMode = MY_WIFI_MODE_STA;

            M5.Lcd.clear();
            M5.Lcd.setCursor(0, 0);
            M5.Lcd.print("IP: ");
            M5.Lcd.println(WiFi.localIP());
        } else {
            WiFi.disconnect();
            WiFi.mode(WIFI_AP);
            currentWifiMode = MY_WIFI_MODE_AP;
        }
    }
}
