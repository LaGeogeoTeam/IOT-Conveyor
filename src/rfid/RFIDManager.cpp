#include "RFIDManager.h"
#include "../motor/MotorManager.h"

static Preferences rfidPrefs;
extern bool isWriting;         // Déclaré dans main.cpp
extern String dataToWrite;     // Idem

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

String RFIDManager::readMifare1K()
{
    MFRC522::MIFARE_Key key;

    for (byte i = 0; i < 6; i++)
    {
        key.keyByte[i] = 0xFF;
    }

    byte blockStart = 6;
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

        printBlockAsText(buffer, 16);

        printBlockAsDecimal(buffer, 16);
        
        for (byte i = 0; i < 16; i++)
        {
            if (buffer[i] == 0)
            {
                break;
            }
            
            readData += (char)buffer[i];
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

void RFIDManager::loadRFIDPrefs() {
  rfidPrefs.begin("rfidState", true); // Mode lecture
  isWriting = rfidPrefs.getBool("isWriting", false);
  dataToWrite = rfidPrefs.getString("dataWrite", "");
  rfidPrefs.end();
}

void RFIDManager::saveRFIDPrefs() {
  rfidPrefs.begin("rfidState", false); // Mode écriture
  rfidPrefs.putBool("isWriting", isWriting);
  rfidPrefs.putString("dataWrite", dataToWrite);
  rfidPrefs.end();
  Serial.println("[Main] isWriting / dataToWrite saved.");
}