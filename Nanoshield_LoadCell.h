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

#define LOADCELL_MAX_SAMPLES 10

class Nanoshield_LoadCell
{
  public:
    Nanoshield_LoadCell(float capacity, float sensitivity, int cs = 8,
                        bool hiGain = true, int numSamples = LOADCELL_MAX_SAMPLES);
    void begin(bool calibrate = true);
    bool updated();
    int32_t getValue();
    int32_t getRawValue();
    int32_t getLatestValue();
    int32_t getLatestRawValue();
    float getWeight();
    void setZero();
    void calibrate();
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
    bool calibrateOnNextCycle;
    uint8_t actualSamples;
    int32_t samplesSum;
    uint8_t numSamples;
    uint8_t head;
    uint8_t tail;
    int32_t samples[LOADCELL_MAX_SAMPLES];
    
    void resetBuffer();
};

#endif
