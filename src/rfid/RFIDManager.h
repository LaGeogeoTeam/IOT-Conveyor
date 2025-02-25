#ifndef RFID_MANAGER_H
#define RFID_MANAGER_H

#include <M5Unified.h>
#include <Preferences.h>
#include "api/APIClient.h"
#include "json/JsonParser.h"
#include "motor/MotorManager.h"
#include "../lib/rfid/MFRC522_I2C.h"

#define MFRC522_I2C_ADDR 0x28

class RFIDManager
{
public:
    void initMFRC522();
    void loadRFIDPrefs();
    void saveRFIDPrefs();
    String getCardUID();
    String readMifare1K();
    void writeMifare1k(byte *data);
    void printBlockAsDecimal(byte *data, int size);
    void printBlockAsText(byte *data, int size);

private:
    MFRC522 _MFRC522 = MFRC522(MFRC522_I2C_ADDR);
};

#endif // RFID_MANAGER_H