
#include <M5Stack.h>
#include "grbl/Module_GRBL_13.2.h"
#include <stdio.h>
#include "GoPlus2.h"
#include <driver/rmt.h>
#include <math.h>

/*
 * The I2C address of GRBL 13.2  Module is 0x70 by default.
 * You could use the DIP Switch for modify I2C address to 0x71
 */

// GRBL

#define STEPMOTOR_I2C_ADDR_1 0x70
#define STEPMOTOR_I2C_ADDR_2 0x71

Module_GRBL _GRBL_A = Module_GRBL(STEPMOTOR_I2C_ADDR_1);
Module_GRBL _GRBL_B = Module_GRBL(STEPMOTOR_I2C_ADDR_2);

// GOPLUS2

GoPlus2 goPlus;

#define X_LOCAL 40
#define Y_LOCAL 30

#define X_OFFSET 160
#define Y_OFFSET 23
#define rrmt_item32_timemout_us 9500

int _hub1, hub1 = 0;
rmt_item32_t signals[1024];

size_t received = 0;

int flag = 0;
int num = 0;

void doTask()
{
  if (num == 4)
  {
    num = 0;
  }
  else
  {
    num++;
  }
  Serial.println(num);
}

void setup()
{
  M5.begin();
  M5.Power.begin();

  // GRBL
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
  M5.Lcd.println("Stopped");
  M5.Lcd.setCursor(10, 200);
  M5.Lcd.println("Forward");
  _GRBL_A.setMode("distance");
  M5.Lcd.setCursor(140, 200);
  M5.Lcd.println("Lock");
  M5.Lcd.setCursor(220, 200);
  M5.Lcd.println("Backward");
  _GRBL_B.setMode("distance");

  // GOPLUS2
  goPlus.begin();
}

void Servo()
{
  goPlus.Servo_write_angle(SERVO_NUM0, 0);
  goPlus.Servo_write_angle(SERVO_NUM1, 0);
  goPlus.Servo_write_angle(SERVO_NUM2, 0);
  goPlus.Servo_write_angle(SERVO_NUM3, 0);
  delay(1000);
  goPlus.Servo_write_plusewidth(SERVO_NUM0_PW, 2500);
  goPlus.Servo_write_plusewidth(SERVO_NUM1_PW, 2500);
  goPlus.Servo_write_plusewidth(SERVO_NUM2_PW, 2500);
  goPlus.Servo_write_plusewidth(SERVO_NUM3_PW, 2500);
  delay(1000);
}

void stepMotor()
{
  bool isLooping = false;
  int loopDirection = 0;
  M5.update();

  if (M5.BtnA.wasPressed())
  {
    M5.Lcd.setCursor(0, 80);
    M5.Lcd.println("Forwarding     ");
    isLooping = true;
    loopDirection = 1;
  }

  if (M5.BtnC.wasPressed())
  {
    M5.Lcd.setCursor(0, 80);
    M5.Lcd.println("Backward     ");
    isLooping = true;
    loopDirection = -1;
  }

  if (M5.BtnB.wasReleased())
  {
    M5.Lcd.setCursor(0, 80);
    M5.Lcd.println("Locked     ");
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

void loop()
{
  stepMotor();
  Servo();
}