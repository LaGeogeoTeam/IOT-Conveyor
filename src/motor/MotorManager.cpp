#include "MotorManager.h"
#define STEPMOTOR_I2C_ADDR_1 0x70


void MotorManager::initMotor()
{
    Wire.begin(21, 22);
    _GRBL_A.Init(&Wire);

    _GRBL_A.setMode("distance");

    // GOPLUS2
    this->goPlus.begin();
    this->goPlus.Servo_write_angle(SERVO_NUM0, 0); 
}

void MotorManager::servoMotor(int angle)
{
  Serial.println("Servo-motor move to ");
  Serial.print(angle);
  //this->goPlus.Servo_write_angle(SERVO_NUM0, angle);//entre 0° et 180°
  this->goPlus.Servo_write_plusewidth(SERVO_NUM0_PW, angle); //entre 500 et 2500 micro seconde
}

void MotorManager::stepMotor()
{
  _GRBL_A.setMotor(50000, 50000, 50000, 200);
  _GRBL_A.setMotor(0, 0, 0, 0);
}

void MotorManager::defineAngleForServoMotor(const int id)
{
  Serial.print("id : ");
  Serial.println(id);
  Serial.println("define angle");
  for(int cpt = 0; cpt < 3; cpt++){
    Serial.print("warehouse id : ");
    Serial.println(warehouseIdArray[cpt][0]);
    if(id == warehouseIdArray[cpt][0]){
      Serial.print("angle : ");
      Serial.println(warehouseIdArray[cpt][1]);
      this->servoMotor(warehouseIdArray[cpt][1]);
    }
  }
}