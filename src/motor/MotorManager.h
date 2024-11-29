#ifndef MOTOR_MANAGER_H
#define MOTOR_MANAGER_H

#include <M5Unified.h>
#include "../lib/grbl/Module_GRBL_13.2.h"
#include "../lib/goplus2/GoPlus2.h"
#define STEPMOTOR_I2C_ADDR_1 0x70

class MotorManager {
public:
    void initMotor();
    void servoMotor(int angle);
    void stepMotor();
    void defineAngleForServoMotor(const int id);

private:
    int warehouseIdArray[3][2] = {
        {1, 500},
        {2, 1500},
        {3, 2500}
    };
    GoPlus2 goPlus;
    Module_GRBL _GRBL_A = Module_GRBL(STEPMOTOR_I2C_ADDR_1);
};

#endif // MOTOR_MANAGER_H