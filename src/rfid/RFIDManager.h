#ifndef RFID_MANAGER_H
#define RFID_MANAGER_H

#include <M5Unified.h>
#include "../lib/rfid/MFRC522_I2C.h"


#define MFRC522_I2C_ADDR 0x28

class RFIDManager {
public :
    //RFIDManager(MotorManager *motorManager);
    void initMFRC522();
    String getCardUID();
    

private:
    MFRC522 _MFRC522 = MFRC522(MFRC522_I2C_ADDR);
};

#endif // RFID_MANAGER_H