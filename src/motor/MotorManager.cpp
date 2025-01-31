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
  this->goPlus.Servo_write_angle(SERVO_NUM0, angle);//entre 0° et 180°
  //this->goPlus.Servo_write_plusewidth(SERVO_NUM0_PW, angle); //entre 500 et 2500 micro seconde
}

void MotorManager::startStepMotor()
{
  _GRBL_A.setMotor(50000, 50000, 50000, 200);
}

void MotorManager::defineAngleForServoMotor(const int id)
{
  for(int cpt = 0; cpt < 3; cpt++){
    if(id == warehouseIdArray[cpt][0]){
      this->servoMotor(warehouseIdArray[cpt][1]);
    }
  }
}