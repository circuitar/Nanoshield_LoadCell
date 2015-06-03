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
    Nanoshield_LoadCell(float maxWeight, int cs = 8);
    void begin();
    void setZero();
    bool dataReady();
    int32_t read();
    float readWeight();

  private:
    static SPISettings spiSettings;
    static int timeout;
  
    int cs;
    int32_t offset;
    float maxWeight;
};

#endif
