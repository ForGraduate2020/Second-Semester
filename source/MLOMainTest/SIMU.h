/**
 * 2020. 5. 25 eastroot
 * *
 * * IMU Sensor
 * *
 */

#pragma once

#include "Sensor.h"
#include "I2Cdev.h"

class SIMU : public Sensor
{
public:
    SIMU();
    ~SIMU();

    bool Initialize();
    
    virtual bool Active();
    void GetAccel(float* x, float* y, float* z);
    void GetGyro(float* x, float* y);
    float GetAccel() { return CurrentAccel; };
    float GetGyro() { return CurrentGyro; };
    
private:
    bool IsDanger;

    uint8_t buffer[14];
    int AxRaw, AyRaw, AzRaw;
    int GxRaw, GyRaw;

    float Ax, Ay, Az;
    float Gx, Gy;
    
    float CurrentAccel;
    float CurrentGyro;
    
};
