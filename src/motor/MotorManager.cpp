#include "MotorManager.h"
#define STEPMOTOR_I2C_ADDR_1 0x70

int warehouseIdArray[3][2] = {
        {1, 500},
        {2, 1000},
        {3, 1500}
};

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
  goPlus.Servo_write_plusewidth(SERVO_NUM0_PW, angle);
}

void MotorManager::stepMotor()
{
  _GRBL_A.setMotor(50000, 50000, 50000, 200);
  _GRBL_A.setMotor(0, 0, 0, 0);
}

void MotorManager::defineAngleForServoMotor(const int id)
{
  for(int cpt = 0; cpt < 3; cpt++){
        if(id == warehouseIdArray[cpt][0]){
            servoMotor(warehouseIdArray[cpt][1]);
        }
    }
    M5.update();

}