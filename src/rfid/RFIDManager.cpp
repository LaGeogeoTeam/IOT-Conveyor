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

        processSector(sector);

        // byte blockStart = sector * 4; // Bloc de départ du secteur
        // if (!_MFRC522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, blockStart, &key, &(_MFRC522.uid))) {
        //     Serial.print("Erreur d'authentification pour le secteur ");
        //     Serial.println(sector);
        //     continue;
        // }

        // for (byte block = blockStart; block < blockStart + 4; block++) {
        //     byte buffer[18];
        //     byte size = sizeof(buffer);

        //     if (_MFRC522.MIFARE_Read(block, buffer, &size) == MFRC522::STATUS_OK) {
        //         Serial.print("Bloc ");
        //         Serial.print(block);
        //         Serial.print(": ");
        //         for (byte i = 0; i < 16; i++) {
        //             Serial.print(buffer[i] < 0x10 ? " 0" : " ");
        //             Serial.print(buffer[i], HEX);
        //         }
        //         Serial.println();

        //         // Afficher en texte brut
        //         printBlockAsText(buffer, 16);

        //         // Afficher en décimal
        //         printBlockAsDecimal(buffer, 16);
        //     } else {
        //         Serial.print("Impossible de lire le bloc ");
        //         Serial.println(block);
        //     }
        //}
    }

    _MFRC522.PICC_HaltA();
    _MFRC522.PCD_StopCrypto1();
}

void RFIDManager::processSector(byte sector) {
    MFRC522::MIFARE_Key key;

    // Récupérer la clé depuis le bloc de contrôle
    if (!getKeyFromSector(sector, key)) {
        Serial.println("Impossible de récupérer la clé !");
        return;
    }

    // Lire les blocs 0, 1 et 2 avec la clé récupérée
    byte buffer[18];
    for (byte i = 0; i < 3; i++) {
        byte block = (sector * 4) + i;
        if (!readDataFromBlock(block, key, buffer)) {
            Serial.print("Erreur lors de la lecture du bloc ");
            Serial.println(block);
        }
    }
}

bool RFIDManager::getKeyFromSector(byte sector, MFRC522::MIFARE_Key &key) {
    byte controlBlock = (sector * 4) + 3; // Dernier bloc du secteur (bloc de contrôle)
    byte buffer[18];

    // Clé par défaut pour authentifier le bloc de contrôle
    MFRC522::MIFARE_Key defaultKey;
    for (byte i = 0; i < 6; i++) {
        defaultKey.keyByte[i] = 0x00; // Par défaut, FF FF FF FF FF FF
    }

    // Authentification du bloc de contrôle
    if (!_MFRC522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, controlBlock, &defaultKey, &(_MFRC522.uid))) {
        Serial.println("Erreur d'authentification pour le bloc de contrôle !");
        return false;
    }

    // Lecture des données du bloc de contrôle
    byte size = sizeof(buffer);
    if (_MFRC522.MIFARE_Read(controlBlock, buffer, &size) != MFRC522::STATUS_OK) {
        Serial.println("Erreur lors de la lecture du bloc de contrôle !");
        return false;
    }

    // Récupération de la clé A
    for (byte i = 0; i < 6; i++) {
        key.keyByte[i] = buffer[i];
    }

    Serial.print("Clé récupérée pour le secteur ");
    Serial.print(sector);
    Serial.print(": ");
    for (byte i = 0; i < 6; i++) {
        Serial.print(key.keyByte[i], HEX);
        Serial.print(" ");
    }
    Serial.println();

    return true;
}


bool RFIDManager::readDataFromBlock(byte block, MFRC522::MIFARE_Key &key, byte *buffer) {
    // Authentification du bloc
    if (!_MFRC522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(_MFRC522.uid))) {
        Serial.println("Erreur d'authentification pour le bloc !");
        return false;
    }

    // Lecture des données
    byte size = 18;
    if (_MFRC522.MIFARE_Read(block, buffer, &size) != MFRC522::STATUS_OK) {
        Serial.println("Erreur lors de la lecture du bloc !");
        return false;
    }

    // Affichage des données
    Serial.print("Bloc ");
    Serial.print(block);
    Serial.print(": ");
    for (byte i = 0; i < 16; i++) {
        Serial.print(buffer[i], HEX);
        Serial.print(" ");
    }

    printBlockAsDecimal(buffer, 16);

    printBlockAsText(buffer, 16);

    Serial.println();
    return true;
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
