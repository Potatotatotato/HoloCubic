#ifndef __IMU_H
#define __IMU_H
#include <Arduino.h>
#include <MPU6050.h>
#include "lv_port_indev.h"

#define IMU_I2C_SDA 32
#define IMU_I2C_SCL 33

enum MANEUVER_TYPE
{
    TURN_RIGHT = 0,
    TURN_LEFT,
    TURN_UP,
    TURN_DOWN,
    NO_MANEUVER
};

class IMU
{
private:
    MPU6050 imu;
    int16_t ax = 0, ay = 0, az = 0;
    int16_t gx = 0, gy = 0, gz = 0;

    uint32_t lastUpdateTime = 0;

public:
    volatile MANEUVER_TYPE manuver;
    void init(void);
    void update(uint32_t interval);
};

extern IMU imu;

#endif
