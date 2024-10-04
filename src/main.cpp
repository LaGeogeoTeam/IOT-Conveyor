/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with M5Core sample source code
*                          配套  M5Core 示例源代码
* Visit for more information:https://docs.m5stack.com/en/module/grbl13.2
* 获取更多资料请访问：https://docs.m5stack.com/zh_CN/module/grbl13.2
*
* Describe: GRBL 13.2 Module.
* Date: 2021/11/14
*******************************************************************************
  GRBL 13.2 Module TEST Example,use I2C to control stepper motors(Stack two
  Module at the same time)
  步进电机模块测试示例,使用I2C控制步进电机(同时堆叠两个模块)
*/
#include <M5Stack.h>
#include "grbl/Module_GRBL_13.2.h"
#include "rfid/reader/MFRC522_I2C.h"

/*
 * The I2C address of GRBL 13.2  Module is 0x70 by default.
 * GRBL 13.2 模块的 I2C 地址默认为 0x70。
 * You could use the DIP Switch for modify I2C address to 0x71
 * 您可以使用拨码开关将 I2C 地址修改为 0x71
 */

#define STEPMOTOR_I2C_ADDR_1 0x70
#define STEPMOTOR_I2C_ADDR_2 0x71
#define MFRC522_I2C_ADDR 0x28

Module_GRBL _GRBL_A = Module_GRBL(STEPMOTOR_I2C_ADDR_1);
Module_GRBL _GRBL_B = Module_GRBL(STEPMOTOR_I2C_ADDR_2);
MFRC522 _MFRC522 = MFRC522(MFRC522_I2C_ADDR);

void setup()
{
  M5.begin();
  M5.Power.begin();
  Wire.begin(21, 22);
  _GRBL_A.Init(&Wire);
  _GRBL_B.Init(&Wire);
  Serial.begin(115200);
  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.setTextSize(3);
  M5.lcd.setBrightness(100);
  M5.Lcd.setCursor(80, 40);
  // M5.Lcd.println("GRBL 13.2");
  M5.Lcd.setCursor(50, 80);
  M5.Lcd.println("Press Btn A/B FOR Motor");
  M5.Lcd.setCursor(50, 120);
  // M5.Lcd.println("Control Motor");
  _GRBL_A.setMode("absolute");
  _GRBL_B.setMode("absolute");

  // M5.Lcd.println("MFRC522 Test");
  _MFRC522.PCD_Init();  // Init _MFRC522
  M5.Lcd.println("Please put the card\n\nUID:");
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