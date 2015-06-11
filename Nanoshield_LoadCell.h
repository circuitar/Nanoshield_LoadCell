/**
 * This is an Arduino library to measure weight using load cells with the ADS1230 IC from Texas Instruments.
 *
 * Copyright (c) 2015 Circuitar
 * This software is released under the MIT license. See the attached LICENSE file for details.
*/
#ifndef NANOSHIELD_LOADCELL_h
#define NANOSHIELD_LOADCELL_h

#include <Arduino.h>
#include <SPI.h>

class Nanoshield_LoadCell
{
  public:
    Nanoshield_LoadCell(float capacity, float sensitivity, bool hiGain = true, int cs = 8);
    void begin();
    void setZero();
    bool updated();
    int32_t getValue();
    int32_t getRawValue();
    float getWeight();
    void TIMER2_OVF_ISR();

  private:
    static SPISettings spiSettings;
    static int timeout;
  
    int cs;
    bool newData;
    int32_t offset;
    float capacity;
    float sensitivity;
    bool hiGain;
    int32_t value;
};

#endif
