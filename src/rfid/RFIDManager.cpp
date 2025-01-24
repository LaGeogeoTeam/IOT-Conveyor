#include "RFIDManager.h"
#include "../motor/MotorManager.h"

void RFIDManager::initMFRC522()
{
    _MFRC522.PCD_Init();
}

String RFIDManager::getCardUID()
{
    // Vérifie si une nouvelle carte est présente
    if (!_MFRC522.PICC_IsNewCardPresent() || !_MFRC522.PICC_ReadCardSerial())
    {
        return ""; // Pas de carte détectée
    }

    // Construire l'UID sous forme de chaîne de caractères
    String uid = "";
    for (byte i = 0; i < _MFRC522.uid.size; i++)
    {
        if (_MFRC522.uid.uidByte[i] < 0x10)
        {
            uid += "0"; // Ajout d'un zéro pour les valeurs hexadécimales < 10
        }
        uid += String(_MFRC522.uid.uidByte[i], HEX); // Conversion en hexadécimal
    }

    uid.toUpperCase(); // Convertir la chaîne en majuscules (modifie en place)
    return uid;        // Retourner l'UID
}

String RFIDManager::readCardData()
{
    if (!_MFRC522.PICC_IsNewCardPresent() || !_MFRC522.PICC_ReadCardSerial())
    {
        return ""; // Pas de carte détectée
    }

    // Identifier le type de carte
    byte piccTypeByte = _MFRC522.PICC_GetType(_MFRC522.uid.sak);

    // Afficher le type de carte
    Serial.print("Type de carte détecté (byte): ");
    Serial.println(piccTypeByte, HEX);
    String dataRead = "";
    if (piccTypeByte == MFRC522::PICC_TYPE_MIFARE_1K)
    {
        Serial.println("Lecture des données de la carte MIFARE 1K...");
       dataRead = readMifare1K();
    } else {
        dataRead = "";
        Serial.println("Type de carte non pris en charge");
    }

    // Arrêter la communication avec la carte
    _MFRC522.PICC_HaltA();
    _MFRC522.PCD_StopCrypto1();

    return dataRead;
}

String RFIDManager::readMifare1K()
{
    MFRC522::MIFARE_Key key;

    // Clé par défaut pour la carte
    for (byte i = 0; i < 6; i++)
    {
        key.keyByte[i] = 0xFF;
    }

    byte blockStart = 6; // Bloc de départ du secteur
    if (!_MFRC522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, blockStart, &key, &(_MFRC522.uid)))
    {
        Serial.print("Erreur d'authentification pour le secteur ");
        Serial.println(1);
    }

    byte buffer[18];
    byte size = sizeof(buffer);
    String readData = "";

    if (_MFRC522.MIFARE_Read(blockStart, buffer, &size) == MFRC522::STATUS_OK)
    {
        Serial.print("Bloc ");
        Serial.print(blockStart);
        Serial.print(": ");
        for (byte i = 0; i < 16; i++)
        {
            Serial.print(buffer[i] < 0x10 ? " 0" : " ");
            Serial.print(buffer[i], HEX);
        }
        Serial.println();

        // Afficher en texte brut
        printBlockAsText(buffer, 16);

        // Afficher en décimal
        printBlockAsDecimal(buffer, 16);

        // Construire une String ASCII à partir du buffer
        
        for (byte i = 0; i < 16; i++)
        {
            readData += (char)buffer[i]; // Ajout de chaque octet sous forme de caractère
        }
    }
    else
    {
        Serial.print("Impossible de lire le bloc ");
        Serial.println(blockStart);
        readData = "";
    }

    _MFRC522.PICC_HaltA();
    _MFRC522.PCD_StopCrypto1();
    
    return readData;
}

void RFIDManager::printBlockAsDecimal(byte *data, int size)
{
    Serial.print("Données (décimal) : ");
    for (int i = 0; i < size; i++)
    {
        Serial.print(data[i]);
        Serial.print(" ");
    }
    Serial.println();
}

void RFIDManager::printBlockAsText(byte *data, int size)
{
    Serial.print("Données (texte) : ");
    for (int i = 0; i < size; i++)
    {
        if (isPrintable(data[i]))
        {
            Serial.print((char)data[i]);
        }
        else
        {
            Serial.print(".");
        }
    }
    Serial.println();
}

void RFIDManager::writeMifare1k(byte *data)
{
    byte block = 6; // Exemple : Bloc 6
    MFRC522::MIFARE_Key key;
    // Clé par défaut pour la carte
    for (byte i = 0; i < 6; i++)
    {
        key.keyByte[i] = 0xFF;
    }

    // Authentifier avant d'écrire
    if (_MFRC522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(_MFRC522.uid)) != MFRC522::STATUS_OK)
    {
        Serial.println("Échec de l'authentification !");
        _MFRC522.PICC_HaltA();
        return;
    }

    // Écrire les données
    if (_MFRC522.MIFARE_Write(block, data, 16) != MFRC522::STATUS_OK)
    {
        Serial.println("Erreur lors de l'écriture des données !");
    }
    else
    {
        Serial.println("Écriture réussie");
    }

    // Arrêter la communication avec la carte
    _MFRC522.PICC_HaltA();
    _MFRC522.PCD_StopCrypto1();
}

String currentUid = "";
String warehouseId;

void RFIDManager::rfidConveyor(const char *baseURL, const char *token)
{
    // get the rfid mode from nvs
    Preferences preferences;
    preferences.begin("rfid", false);
    APIClient apiClient(baseURL, token);
    MotorManager motorManager;
    bool rfidMode = preferences.getBool("rwMode");
    preferences.end();
    // Récupérer l'UID de la carte
    String uid = getCardUID();

    if (uid != "")
    {
        // Si une carte est détectée
        if (rfidMode == false)
        {
            readMifare1K();
            delay(1000);
            if (!currentUid.isEmpty())
            {
                String payload = "{";
                payload += "\"product_id\":\"" + uid + "\",";
                payload += "\"warehouse_id\":\"" + warehouseId + "\",";
                payload += "\"qty\":1";
                payload += "}";

                // Effectuer la requête POST avec le payload
                String postResponse = apiClient.postRequest("stockmovements", payload);
            }
            String response = apiClient.getRequest("products/ref/", uid);

            warehouseId = getJsonValue(response, "fk_default_warehouse");
            uid = getJsonValue(response, "id");
            motorManager.defineAngleForServoMotor(warehouseId.toInt());
        }
        else
        {
            uid.trim();
            currentUid.trim();
            if (uid != currentUid)
            { // Vérifier si c'est une nouvelle carte
                Serial.println("Nouvelle carte détectée, écriture en cours...");
                byte data[] = {
                    'p',
                    'r',
                    'o',
                    'u',
                    't',
                    ' ',
                    'd',
                    'e',
                    ' ',
                    'g',
                    'r',
                    'e',
                    'g',
                };
                writeMifare1k(data); // Écrire sur la carte
                currentUid = uid;    // Mettre à jour l'UID courant
                delay(1000);
            }
            else
            { // Si c'est la même carte qu'avant
                Serial.println("Même carte détectée, lecture en cours...");
                readMifare1K(); // Lire les données sur la carte
                delay(1000);
            }
        }
    }

    else
    {
        delay(500); // Réduire la fréquence de boucle pour économiser les ressources
    }
}