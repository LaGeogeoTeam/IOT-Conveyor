#include "WebConfigServer.h"
#include <AsyncTCP.h>
#include <M5Unified.h>

WebConfigServer::WebConfigServer()
  : server(80),
    ssid(""),
    password(""),
    apiURL(""),
    token("")
{}

void WebConfigServer::begin() {
    // Part Wifi
    preferences.begin("WiFiSettings", true); // true = lecture seule // false = ecriture
    ssid = preferences.getString("ssid", apiURL);
    password  = preferences.getString("password",  token);
    preferences.end();
    // Part Api
    preferences.begin("ApiConfig", true);
    apiURL = preferences.getString("apiURL", apiURL);
    token  = preferences.getString("token",  token);
    preferences.end();

    setupServer();

    server.begin();
    Serial.println("WebConfigServer: Serveur web demarre.");
}

void WebConfigServer::setupServer() {
    // Page principale : formulaire
    server.on("/", HTTP_GET, [this](AsyncWebServerRequest *request) {
        String page = generateConfigPage(ssid, password, apiURL, token);

        request->send(200, "text/html", page);
    });

    // Route POST pour sauvegarder
    server.on("/set", HTTP_POST, [this](AsyncWebServerRequest *request) {
        if (request->hasParam("ssid", true)) {
            ssid = request->getParam("ssid", true)->value();
        }
        if (request->hasParam("password", true)) {
            password = request->getParam("password", true)->value();
        }
        if (request->hasParam("api", true)) {
            apiURL = request->getParam("api", true)->value();
        }
        if (request->hasParam("token", true)) {
            token = request->getParam("token", true)->value();
        }

        savePreferences();

        // Réponse
        request->send(200, "text/plain", "Settings saved! Rebooting...");
        delay(1000);
        ESP.restart();
    });

    server.on("/reset", HTTP_GET, [this](AsyncWebServerRequest *request) {
        resetPreferences();
        // Réponse
        request->send(200, "text/plain", "All data have been reset. Restarting...");
        delay(1000);
        ESP.restart();
    });
}

String WebConfigServer::generateConfigPage(const String &ssid, 
                          const String &password,
                          const String &apiURL,
                          const String &apiToken)
{
  String html = R"html(
<!DOCTYPE html>
<html>
<head>
  <meta charset="utf-8"/>
  <title>M5 Configuration</title>
  <style>
    body { font-family: Arial; margin: 0; padding: 0; background: #f4f4f4; }
    .container { width: 300px; margin: 80px auto; background: #fff; padding: 20px; }
    label { font-weight: bold; margin: 10px 0 5px; display: block; }
    input[type="text"] {
      width: 100%; padding: 8px; margin-bottom: 10px; box-sizing: border-box;
      border: 1px solid #ccc; border-radius: 3px;
    }
    input[type="submit"], button {
      background: #2196f3; color: #fff; border: none; padding: 10px 16px;
      font-size: 16px; border-radius: 4px; cursor: pointer;
      margin-right: 10px;
    }
    input[type="submit"]:hover, button:hover { background: #1976d2; }
    
    #resetBtn {
      background: #f44336; /* rouge */
    }
    #resetBtn:hover {
      background: #d32f2f; /* rouge plus sombre au survol */
    }
    .btn-group {
      display: flex;
      justify-content: space-between;
      align-items: center;
      margin-top: 10px; 
    }
  </style>
</head>
<body>
  <div class="container">
    <h2>Configuration</h2>
    <form action="/set" method="POST">
      <label for="ssid">Wifi Name</label>
      <input type="text" name="ssid" id="ssid" placeholder=")html";
  html += ssid;
  html += R"html(" />

      <label for="password">Wifi Password</label>
      <input type="text" name="password" id="password" placeholder=")html";
  html += password;
  html += R"html(" />

      <label for="api">API URL</label>
      <input type="text" name="api" id="api" placeholder=")html";
  html += apiURL;
  html += R"html(" />

      <label for="token">API Token</label>
      <input type="text" name="token" id="token" placeholder=")html";
  html += apiToken;
  html += R"html(" />

      <div class="btn-group">
        <input type="submit" value="Save"/>
        <button type="button" id="resetBtn" onclick="location.href='/reset'">Reset</button>
      </div>
    </form>
  </div>
</body>
</html>
)html";

  return html;
}

void WebConfigServer::savePreferences() {
    //Part Wifi
    preferences.begin("WiFiSettings", false);
    preferences.putString("ssid", ssid);
    preferences.putString("password", password);
    preferences.end();
    //Part Api
    preferences.begin("ApiConfig", false);
    preferences.putString("apiURL", apiURL);
    preferences.putString("token",  token);
    preferences.end();

    Serial.println("WebConfigServer: Parametres sauvegardes.");
}

void WebConfigServer::resetPreferences() {
    //Part Wifi
    preferences.begin("WiFiSettings", false);
    preferences.clear();
    preferences.end();
    //Part Api
    preferences.begin("ApiConfig", false);
    preferences.clear();
    preferences.end();

    Serial.println("WebConfigServer: Parametres reinitialises.");
}
