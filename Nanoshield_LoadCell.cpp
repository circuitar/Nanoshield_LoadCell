/**
 * This is an Arduino library to measure weight using load cells with the ADS1230 IC from Texas Instruments.
 *
 * Copyright (c) 2015 Circuitar
 * This software is released under the MIT license. See the attached LICENSE file for details.
*/
#include <Nanoshield_LoadCell.h>

// Use Timer 2 prescaler of 1, 8, 32, 64, 128, 256 or 1024
#define LOADCELL_PRESCALER     64

// Possible prescaler values
#define LOADCELL_PS_1    1
#define LOADCELL_PS_8    2
#define LOADCELL_PS_32   3
#define LOADCELL_PS_64   4
#define LOADCELL_PS_128  5
#define LOADCELL_PS_256  6
#define LOADCELL_PS_1024 7

// Utility macros
#define LOADCELL_TCCR2B(PS) PRIMITIVE_CAT(LOADCELL_PS_, PS)
#define PRIMITIVE_CAT(a, ...) a ## __VA_ARGS__

SPISettings Nanoshield_LoadCell::spiSettings = SPISettings(4000000, MSBFIRST, SPI_MODE1);
int Nanoshield_LoadCell::timeout = 120;

static Nanoshield_LoadCell* loadCells[15]; // Pointers to all registered load cell objects
static uint8_t loadCellCount = 0;          // Number of registered load cell objects
static uint8_t t2Count = 0;                // Number of times Timer 2 has overflowed

Nanoshield_LoadCell::Nanoshield_LoadCell(float capacity, float sensitivity, bool hiGain, int cs) {
  this->cs = cs;
  this->newData = false;
  this->offset = 0;
  this->capacity = capacity;
  this->sensitivity = sensitivity;
  this->hiGain = hiGain;
  
  // Register load cell
  loadCells[loadCellCount++] = this;
}

void Nanoshield_LoadCell::begin() {
  this->newData = false;

  pinMode(cs, OUTPUT);
  digitalWrite(cs, HIGH);
  SPI.begin();
  SPI.usingInterrupt(255);
  
  // Configure timer 2
  TCCR2A = 0b00000000;
  TCCR2B = LOADCELL_TCCR2B(LOADCELL_PRESCALER);
  TIMSK2 = 0b00000001;
}

void Nanoshield_LoadCell::setZero() {
  offset = value;
}

bool Nanoshield_LoadCell::updated() {
  return newData;
}

int32_t Nanoshield_LoadCell::getValue() {
  newData = false;
  return value - offset;
}

int32_t Nanoshield_LoadCell::getRawValue() {
  newData = false;
  return value;
}

float Nanoshield_LoadCell::getWeight() {
  return capacity * getValue() / ((1L << 20) * (hiGain ? 128 : 64) * (sensitivity / 1000));
}

void Nanoshield_LoadCell::TIMER2_OVF_ISR() {
  // Check if there is new data to read
  SPI.beginTransaction(spiSettings);
  digitalWrite(cs, LOW);
  if (digitalRead(12) == LOW) {
    // Read data via SPI if /DRDY is low
    value = 0;
    value |= SPI.transfer(0);
    value <<= 8;
    value |= SPI.transfer(0);
    value <<= 8;
    value |= SPI.transfer(0);
    value <<= 8;
    value /= 1L << 12;
    newData = true;
  }
  digitalWrite(cs, HIGH);
  SPI.endTransaction();
}

ISR(TIMER2_OVF_vect) {
  // Process ISR for all configured load cells at about 100Hz
  //  Each cycle = 10240uS (rounding to make division exact for 16MHz)
  if (10240 / clockCyclesToMicroseconds(256 * LOADCELL_PRESCALER)) {
    for (uint8_t i = 0; i < loadCellCount; i++) {
      loadCells[i]->TIMER2_OVF_ISR();
    }
  }
}
