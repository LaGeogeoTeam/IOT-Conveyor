
#include <M5Stack.h>
#include "grbl/Module_GRBL_13.2.h"
#include <stdio.h>

/*
 * The I2C address of GRBL 13.2  Module is 0x70 by default.
 * You could use the DIP Switch for modify I2C address to 0x71
 */

#define STEPMOTOR_I2C_ADDR_1 0x70
#define STEPMOTOR_I2C_ADDR_2 0x71

Module_GRBL _GRBL_A = Module_GRBL(STEPMOTOR_I2C_ADDR_1);
Module_GRBL _GRBL_B = Module_GRBL(STEPMOTOR_I2C_ADDR_2);

String motorStatus = "Stopped";
void setup()
{
  M5.begin();
  M5.Power.begin();
  Wire.begin(21, 22);
  _GRBL_A.Init(&Wire);
  _GRBL_B.Init(&Wire);
  Serial.begin(115200);
  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.setTextSize(2);
  M5.lcd.setBrightness(100);
  M5.Lcd.setCursor(80, 10);
  M5.Lcd.println("Control motor");
  M5.Lcd.setCursor(0, 80);
  M5.Lcd.println("Press button to start the motor");
  M5.Lcd.setCursor(10, 200);
  M5.Lcd.println("Forward");
  _GRBL_A.setMode("distance");
  M5.Lcd.setCursor(140, 200);
  M5.Lcd.println("Lock");
  M5.Lcd.setCursor(220, 200);
  M5.Lcd.println("Backward");
  _GRBL_B.setMode("distance");
}

void loop()
{
  bool isLooping = false;
  int loopDirection = 0;
  M5.update();

  if (M5.BtnA.wasPressed())
  {
    isLooping = true;
    loopDirection = 1;
  }

  if (M5.BtnC.wasPressed())
  {
    isLooping = true;
    loopDirection = -1;
  }

  if (M5.BtnB.wasReleased())
  {
    isLooping = false;
    _GRBL_A.unLock();
    _GRBL_B.unLock();
    Serial.println("Loop stopped");
  }

  if (isLooping)
  {
    int motorValue = loopDirection == 1 ? 5000 : -5000;
    Serial.print(_GRBL_A.readStatus());
    _GRBL_A.setMotor(motorValue, motorValue, motorValue, 200);
    _GRBL_A.setMotor(0, 0, 0, 0);
  }
}