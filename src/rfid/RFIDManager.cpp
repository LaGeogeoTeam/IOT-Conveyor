#include "RFIDManager.h"
#include "../motor/MotorManager.h"


void RFIDManager::initMFRC522()
{
    _MFRC522.PCD_Init();
}

String RFIDManager::getCardUID() 
{
    // Vérifie si une nouvelle carte est présente
    if (!_MFRC522.PICC_IsNewCardPresent() || !_MFRC522.PICC_ReadCardSerial()) {
        return ""; // Pas de carte détectée
    }    
    
    // Construire l'UID sous forme de chaîne de caractères
    String uid = "";
    for (byte i = 0; i < _MFRC522.uid.size; i++) {
        if (_MFRC522.uid.uidByte[i] < 0x10) {
            uid += "0"; // Ajout d'un zéro pour les valeurs hexadécimales < 10
        }
        uid += String(_MFRC522.uid.uidByte[i], HEX); // Conversion en hexadécimal
    }

    uid.toUpperCase(); // Convertir la chaîne en majuscules (modifie en place)
    return uid; // Retourner l'UID
}

void RFIDManager::readCardData() 
{
    if (!_MFRC522.PICC_IsNewCardPresent() || !_MFRC522.PICC_ReadCardSerial()) {
        return; // Pas de carte détectée
    }

    // Identifier le type de carte
    byte piccTypeByte = _MFRC522.PICC_GetType(_MFRC522.uid.sak);


    // Afficher le type de carte
    Serial.print("Type de carte détecté (byte): ");
    Serial.println(piccTypeByte, HEX);

    if (piccTypeByte == MFRC522::PICC_TYPE_MIFARE_1K) {
        Serial.println("Lecture des données de la carte MIFARE 1K...");
        readMifare1K();
    }

    // Arrêter la communication avec la carte
    _MFRC522.PICC_HaltA();
    _MFRC522.PCD_StopCrypto1();
}

void RFIDManager::readMifare1K() 
{
    MFRC522::MIFARE_Key key;

    // Clé par défaut pour la carte
    for (byte i = 0; i < 6; i++) {
        key.keyByte[i] = 0xFF;
    }

    // Lire chaque secteur et ses blocs
    for (byte sector = 0; sector < 16; sector++) {
        Serial.print("Secteur ");
        Serial.println(sector);

        byte blockStart = sector * 4; // Bloc de départ du secteur
        if (!_MFRC522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, blockStart, &key, &(_MFRC522.uid))) {
            Serial.print("Erreur d'authentification pour le secteur ");
            Serial.println(sector);
            continue;
        }

        for (byte block = blockStart; block < blockStart + 4; block++) {
            byte buffer[18];
            byte size = sizeof(buffer);

            if (_MFRC522.MIFARE_Read(block, buffer, &size) == MFRC522::STATUS_OK) {
                Serial.print("Bloc ");
                Serial.print(block);
                Serial.print(": ");
                for (byte i = 0; i < 16; i++) {
                    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
                    Serial.print(buffer[i], HEX);
                }
                Serial.println();

                // Afficher en texte brut
                printBlockAsText(buffer, 16);

                // Afficher en décimal
                printBlockAsDecimal(buffer, 16);
            } else {
                Serial.print("Impossible de lire le bloc ");
                Serial.println(block);
            }
        }
    }

    _MFRC522.PICC_HaltA();
    _MFRC522.PCD_StopCrypto1();
}

void RFIDManager::printBlockAsDecimal(byte *data, int size) 
{
    Serial.print("Données (décimal) : ");
    for (int i = 0; i < size; i++) {
        Serial.print(data[i]);
        Serial.print(" ");
    }
    Serial.println();
}

void RFIDManager::printBlockAsText(byte *data, int size) 
{
    Serial.print("Données (texte) : ");
    for (int i = 0; i < size; i++) {
        if (isPrintable(data[i])) {
            Serial.print((char)data[i]);
        } else {
            Serial.print(".");
        }
    }
    Serial.println();
}
