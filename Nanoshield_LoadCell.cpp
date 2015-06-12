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

Nanoshield_LoadCell::Nanoshield_LoadCell(float capacity, float sensitivity, int cs,
                                         bool hiGain, int numSamples) {
  // Initialize fixed parameters
  this->cs = cs;
  this->offset = 0;
  this->capacity = capacity;
  this->sensitivity = sensitivity;
  this->hiGain = hiGain;
  
  if (numSamples > LOADCELL_MAX_SAMPLES) {
    this->numSamples = LOADCELL_MAX_SAMPLES;
  } else if (numSamples < 1) {
    this->numSamples = 1;
  } else {
    this->numSamples = numSamples;
  }
  
  // Reset circular buffer
  resetBuffer();

  // Register load cell
  loadCells[loadCellCount++] = this;
}

void Nanoshield_LoadCell::begin() {
  // Reset circular buffer
  resetBuffer();

  // Initialize hardware resources
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
  offset = samplesSum / actualSamples;
}

bool Nanoshield_LoadCell::updated() {
  return newData && actualSamples >= numSamples;
}

int32_t Nanoshield_LoadCell::getValue() {
  newData = false;
  return samplesSum / actualSamples - offset;
}

int32_t Nanoshield_LoadCell::getRawValue() {
  newData = false;
  return samplesSum / actualSamples;
}

int32_t Nanoshield_LoadCell::getLatestValue() {
  newData = false;
  return samples[tail] - offset;
}

int32_t Nanoshield_LoadCell::getLatestRawValue() {
  newData = false;
  return samples[tail];
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
    int32_t sample = 0;
    sample |= SPI.transfer(0);
    sample <<= 8;
    sample |= SPI.transfer(0);
    sample <<= 8;
    sample |= SPI.transfer(0);
    sample <<= 8;
    sample /= 1L << 12;

    // Save sample in circular buffer
    if (++actualSamples > numSamples) {
      // Buffer full, remove oldest sample (head), removing it from the sum
      actualSamples = numSamples;
      samplesSum -= samples[head];
      if (++head >= numSamples) {
        head = 0;
      }
    }
    
    // Put new sample in the circular buffer tail and add it to the sum
    if (++tail >= numSamples) {
      tail = 0;
    }
    samples[tail] = sample;
    samplesSum += sample;
    
    // Inform there is new data
    newData = true;
  }
  digitalWrite(cs, HIGH);
  SPI.endTransaction();
}

void Nanoshield_LoadCell::resetBuffer() {
  newData = false;
  actualSamples = 0;
  samplesSum = 0;
  head = 0;
  tail = numSamples - 1;
  samples[tail] = 0;
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
