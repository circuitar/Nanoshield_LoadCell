/** @file
 * This is an Arduino library to measure weight using load cells with the ADS1230 IC from Texas Instruments.
 *
 * Copyright (c) 2015 Circuitar
 * This software is released under the MIT license. See the attached LICENSE file for details.
*/
#include <Nanoshield_LoadCell.h>
#include <util/atomic.h>

// Configure timer constants for different timers (default to timer 2)
#if LOADCELL_TIMER == 1

#define TCCRxA TCCR1A
#define TCCRxB TCCR1B
#define TIMSKx TIMSK1
#define TCCRxB_VALUE PRESCALER_16BIT_1
#define PRESCALER 1
#define TIMER_BITS 16
#define TIMER_INT_VECTOR TIMER1_OVF_vect

#elif LOADCELL_TIMER == 3

#define TCCRxA TCCR3A
#define TCCRxB TCCR3B
#define TIMSKx TIMSK3
#define TCCRxB_VALUE PRESCALER_16BIT_1
#define PRESCALER 1
#define TIMER_BITS 16
#define TIMER_INT_VECTOR TIMER3_OVF_vect

#elif LOADCELL_TIMER == 4

#define TCCRxA TCCR4A
#define TCCRxB TCCR4B
#define TIMSKx TIMSK4
#define TCCRxB_VALUE PRESCALER_16BIT_1
#define PRESCALER 1
#define TIMER_BITS 16
#define TIMER_INT_VECTOR TIMER4_OVF_vect

#elif LOADCELL_TIMER == 5

#define TCCRxA TCCR5A
#define TCCRxB TCCR5B
#define TIMSKx TIMSK5
#define TCCRxB_VALUE PRESCALER_16BIT_1
#define PRESCALER 1
#define TIMER_BITS 16
#define TIMER_INT_VECTOR TIMER5_OVF_vect

#else

#define TCCRxA TCCR2A
#define TCCRxB TCCR2B
#define TIMSKx TIMSK2
#define TCCRxB_VALUE PRESCALER_8BIT_256
#define PRESCALER 256
#define TIMER_BITS 8
#define TIMER_INT_VECTOR TIMER2_OVF_vect

#endif

// Prescaler values for 8-bit timers
#define PRESCALER_8BIT_1    1
#define PRESCALER_8BIT_8    2
#define PRESCALER_8BIT_32   3
#define PRESCALER_8BIT_64   4
#define PRESCALER_8BIT_128  5
#define PRESCALER_8BIT_256  6
#define PRESCALER_8BIT_1024 7

// Prescaler values for 16-bit timers
#define PRESCALER_16BIT_1    1
#define PRESCALER_16BIT_8    2
#define PRESCALER_16BIT_64   3
#define PRESCALER_16BIT_256  4
#define PRESCALER_16BIT_1024 5

SPISettings Nanoshield_LoadCell::spiSettings = SPISettings(4000000, MSBFIRST, SPI_MODE1);
int Nanoshield_LoadCell::timeout = 120;

static Nanoshield_LoadCell* loadCells[15]; // Pointers to all registered load cell objects
static uint8_t loadCellCount = 0;          // Number of registered load cell objects
static uint8_t timerOvfCount = 0;          // Timer overflow counter

Nanoshield_LoadCell::Nanoshield_LoadCell(float capacity, float sensitivity, int cs,
                                         bool hiGain, int numSamples) {
  // Initialize fixed parameters
  this->cs = cs;
  this->offset = 0;
  this->capacity = capacity;
  this->sensitivity = sensitivity;
  this->hiGain = hiGain;
  this->calibrateOnNextCycle = false;
  
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

void Nanoshield_LoadCell::begin(bool calibrate) {
  // Calibrate on first cycle
  calibrateOnNextCycle = calibrate;

  // Reset circular buffer
  resetBuffer();

  // Initialize hardware resources
  pinMode(cs, OUTPUT);
  digitalWrite(cs, HIGH);
  SPI.begin();
  SPI.usingInterrupt(255);
  
  // Configure timer
  TCCRxA = 0b00000000;
  TCCRxB = TCCRxB_VALUE;
  TIMSKx = 0b00000001;
}

bool Nanoshield_LoadCell::updated() {
  bool nd;
  uint8_t as;
  ATOMIC_BLOCK(ATOMIC_FORCEON) {
    nd = newData;
    as = actualSamples;
  }
  return nd && as >= numSamples;
}

int32_t Nanoshield_LoadCell::getValue() {
  int32_t ss;
  uint8_t as;
  int32_t of;
  ATOMIC_BLOCK(ATOMIC_FORCEON) {
    newData = false;
    ss = samplesSum;
    as = actualSamples;
    of = offset;
  }
  return ss / as - of;
}

int32_t Nanoshield_LoadCell::getRawValue() {
  int32_t ss;
  uint8_t as;
  ATOMIC_BLOCK(ATOMIC_FORCEON) {
    newData = false;
    ss = samplesSum;
    as = actualSamples;
  }
  return ss / as;
}

int32_t Nanoshield_LoadCell::getLatestValue() {
  int32_t st;
  int32_t of;
  ATOMIC_BLOCK(ATOMIC_FORCEON) {
    newData = false;
    st = samples[tail];
    of = offset;
  }
  return st - of;
}

int32_t Nanoshield_LoadCell::getLatestRawValue() {
  int32_t st;
  ATOMIC_BLOCK(ATOMIC_FORCEON) {
    newData = false;
    st = samples[tail];
  }
  return st;
}

float Nanoshield_LoadCell::getWeight() {
  return capacity * getValue() / ((1L << 20) * (hiGain ? 128 : 64) * (sensitivity / 1000));
}

void Nanoshield_LoadCell::setZero() {
  offset = getRawValue();
}

void Nanoshield_LoadCell::calibrate() {
  ATOMIC_BLOCK(ATOMIC_FORCEON) {
    calibrateOnNextCycle = true;
  }
}

void Nanoshield_LoadCell::readDataIfReady() {
  // Check if there is new data to read
  SPI.beginTransaction(spiSettings);
  digitalWrite(cs, LOW);
  if (digitalRead(MISO) == LOW) {
    // Read data via SPI if /DRDY is low
    int32_t sample = 0;
    sample |= SPI.transfer(0);
    sample <<= 8;
    sample |= SPI.transfer(0);
    sample <<= 8;
    sample |= SPI.transfer(0);
    sample <<= 8;
    sample /= 1L << 12;

    if (calibrateOnNextCycle) {
      // Send at least two more clock pulses to initiate ADS1230 offset calibration, if requested
      SPI.transfer(0);
      calibrateOnNextCycle = false;
    } else {
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
  }
  digitalWrite(cs, HIGH);
  SPI.endTransaction();
}

void Nanoshield_LoadCell::resetBuffer() {
  ATOMIC_BLOCK(ATOMIC_FORCEON) {
    newData = false;
    actualSamples = 0;
    samplesSum = 0;
    head = 0;
    tail = numSamples - 1;
    samples[tail] = 0;
  }
}

void timerOverflowIsr(int i) {
  loadCells[i]->readDataIfReady();
}

ISR(TIMER_INT_VECTOR) {
  // Process ISR for all configured load cells at a rate slightly higher than 80Hz (maximum ADS1230 sample rate)
  //  Each timer cycle = 4096uS (8-bit timer with prescaler = 256 or 16-bit timer with prescaler = 1)
  //  Three timer cycles = 12288uS ~ 81.38Hz
  if (++timerOvfCount * clockCyclesToMicroseconds((unsigned long)PRESCALER << TIMER_BITS) >= 12288) {
    for (uint8_t i = 0; i < loadCellCount; i++) {
      timerOverflowIsr(i);
    }
    timerOvfCount = 0;
  }
}
