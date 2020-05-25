/**
 * 2020. 5. 25 eastroot
 *
 */

#include "SIMU.h"
#include "Wire.h"
#include "I2Cdev.h"

SIMU::SIMU()
{
    OldAccel = 0.0f;
    OldGyro = 0.0f;
}

SIMU::~SIMU()
{
    
}

bool SIMU::Initialize()
{
    Wire.begin();
    
    // 0x68 : devAddr
    I2Cdev::writeBits(0x68, 0x6B, 2, 3, 0x01);

    // gyro range to full
    I2Cdev::writeBits(0x68, 0x1B, 4, 2, 0x00);

    // accel range to full
    I2Cdev::writeBits(0x68, 0x1C, 4, 2, 0x00);

    // sleep enable
    I2Cdev::writeBit(0x68, 0x6B, 6, false);
    
    return true;
}

void SIMU::GetAccel(float* x, float* y, float* z)
{
  *x = Ax;
  *y = Ay;
  *z = Az;
}

void SIMU::GetGyro(float* x, float* y)
{
  *x = Gx;
  *y = Gy;
}

bool SIMU::Active()
{
    bool Result = false;
    
    I2Cdev::readBytes(0x68, 0x3B, 14, buffer);

    AxRaw = (((int16_t)buffer[0]) << 8) | buffer[1];
    AyRaw = (((int16_t)buffer[2]) << 8) | buffer[3];
    AzRaw = (((int16_t)buffer[4]) << 8) | buffer[5];
    GxRaw = (((int16_t)buffer[8]) << 8) | buffer[9];
    GyRaw = (((int16_t)buffer[10]) << 8) | buffer[11];
    
    Ax = (float) AxRaw / 16384;
    Ay = (float) AyRaw / 16384;
    Az = (float) AzRaw / 16384;
    
    Gx = (float) GxRaw * 250 / 32768;
    Gy = (float) GyRaw * 250 / 32768;
    
    float CurrentAccel;
    float CurrentGyro;
    
    CurrentAccel = sqrt(Ax*Ax + Ay*Ay + Az*Az);
    CurrentGyro = sqrt(Gx*Gx * Gy*Gy);

    if (CurrentAccel > 2.0 && CurrentGyro > 200)
    {
      Result = true;
    }
    
    return Result;
}
