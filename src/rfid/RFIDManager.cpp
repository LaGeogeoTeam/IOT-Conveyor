#include "RFIDManager.h"
#include "../motor/MotorManager.h"

int warehouseIdArray[3][2] = {
        {1, 500},
        {2, 1000},
        {3, 1500}
};

RFIDManager::RFIDManager(MotorManager *motorManager) {
    this->motorManager = motorManager;
}

void RFIDManager::initMFRC522(){
    _MFRC522.PCD_Init();
}

String RFIDManager::getCardUID() {
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

void RFIDManager::rfidReader(const int id){
    for(int cpt = 0; cpt < 3; cpt++){
        if(id == warehouseIdArray[cpt][0]){
            motorManager->servoMotor(warehouseIdArray[cpt][1]);
        }
    }
    M5.update();
}