#ifndef RFID_MANAGER_H
#define RFID_MANAGER_H

#include <M5Unified.h>
#include "../lib/rfid/MFRC522_I2C.h"

void initMFRC522();
String getCardUID();

#endif // RFID_MANAGER_H