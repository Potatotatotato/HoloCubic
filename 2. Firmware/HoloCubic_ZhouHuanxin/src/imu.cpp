#include <imu.h>
#include <WiFi.h>
#include "GUI_APP/WIN/desktop_win.h"

//================================
//   Variable Declaration
//================================
IMU imu;

//================================
//      Function Defination
//================================
void IMU::init(void)
{
    Wire.begin(IMU_I2C_SDA, IMU_I2C_SCL, 400000);
    Wire.setClock(400000);
    while (imu.testConnection() != true)
        ;
    Serial.println("Connected to MPU6050!");
    imu.initialize();
}

void IMU::update(uint32_t interval)
{
    imu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

    if (millis() - lastUpdateTime > interval)
    {
        lastUpdateTime = millis();

        // static char str[100];
        // sprintf(str, "ax: %d  ay: %d  az: %d\r\n\r\ngx: %d  gy: %d  gz: %d\r\n\r\n", ax, ay, az, gx, gy, gz);
        // Serial.println(str);

        if (appSwitchHandlerLock == false)
        {
            if (ay > 4000)
            {
                manuver = TURN_LEFT;
                Serial.println("MPU6050 Encoder: Left");
                appSwitchHandlerLock = true; //上锁
                Serial.println("Lock");
            }
            else if (ay < -4000)
            {
                manuver = TURN_RIGHT;
                Serial.println("MPU6050 Encoder: Right");
                appSwitchHandlerLock = true; //上锁
                Serial.println("Lock");
            }
            else // ax（左右移动优先级更低）
            {
                if (ax < 2000)
                {
                    manuver = TURN_DOWN;
                    Serial.println("MPU6050 Encoder: Forward");
                    appSwitchHandlerLock = true; //上锁
                    Serial.println("Lock");
                }
                else if (ax > 8000)
                {
                    manuver = TURN_UP;
                    Serial.println("MPU6050 Encoder: Backward");
                    appSwitchHandlerLock = true; //上锁
                    Serial.println("Lock");
                }
            }
        }
    }
}
