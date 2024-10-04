
#include "grbl/Module_GRBL_13.2.h"
#include "rfid/MFRC522_I2C.h"
#include <stdio.h>
#include "GoPlus2.h"
#include <driver/rmt.h>
#include <math.h>
#undef min
#undef max
#include <thread>
#include <chrono>
#include <unordered_map>
using namespace std;

/*
 * The I2C address of GRBL 13.2  Module is 0x70 by default.
 * You could use the DIP Switch for modify I2C address to 0x71
 */

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

TaskHandle_t Task1;
TaskHandle_t Task2;

// GRBL

#define STEPMOTOR_I2C_ADDR_1 0x70
#define STEPMOTOR_I2C_ADDR_2 0x71
#define MFRC522_I2C_ADDR 0x28

Module_GRBL _GRBL_A = Module_GRBL(STEPMOTOR_I2C_ADDR_1);
Module_GRBL _GRBL_B = Module_GRBL(STEPMOTOR_I2C_ADDR_2);
MFRC522 _MFRC522 = MFRC522(MFRC522_I2C_ADDR);
void setup()
{

}

void loop()
{
  // If Button A was pressed,stepmotor will rotate back and forth at a time
  // 如果按下按钮 A，步进电机将一次来回旋转
  if (M5.BtnA.wasPressed())
  { // A button
    Serial.print(_GRBL_A.readStatus());
    _GRBL_A.setMotor(5, 5, 5, 200);
    _GRBL_B.setMotor(5, 5, 5, 200);
    _GRBL_A.setMotor(0, 0, 0, 200);
    _GRBL_B.setMotor(0, 0, 0, 200);
  }

  if (M5.BtnB.wasPressed())
  {
    // USE Gcode
    _GRBL_A.sendGcode("G1 X5Y5Z5 F200");
    _GRBL_B.sendGcode("G1 X5Y5Z5 F200");
    _GRBL_A.sendGcode("G1 X0Y0Z0 F200");
    _GRBL_B.sendGcode("G1 X0Y0Z0 F200");
  }

  if (M5.BtnC.wasReleased())
  {
    _GRBL_A.unLock();
    _GRBL_B.unLock();
  }

  M5.Lcd.setCursor(40, 47);
  if (!_MFRC522.PICC_IsNewCardPresent() ||
      !_MFRC522.PICC_ReadCardSerial()) {
      delay(200);
        M5.Lcd.clearDisplay ();
        M5.Lcd.println("Press Btn A/B FOR Motor");
        M5.Lcd.println("Please put the card\n\nUID:");
        M5.Lcd.println("");
      return;
  }
  M5.Lcd.fillRect(42, 47, 320, 20, BLACK);
  M5.Lcd.clearDisplay ();
  for (byte i = 0; i < _MFRC522.uid.size;
        i++) {  // Output the stored UID data
      M5.Lcd.print(_MFRC522.uid.uidByte[i] < 0x10 ? " 0" : " ");
      M5.Lcd.print(_MFRC522.uid.uidByte[i], HEX);
  }
  M5.update();
}