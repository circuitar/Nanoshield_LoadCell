/** @file
 * Nanoshield_LoadCell Arduino library.
 *
 * This is an Arduino library to measure weight using load cells with the ADS1230 IC from Texas Instruments.
 *
 * Copyright (c) 2015 Circuitar
 * This software is released under the MIT license. See the attached LICENSE file for details.
*/
#ifndef NANOSHIELD_LOADCELL_h
#define NANOSHIELD_LOADCELL_h

#include <Arduino.h>
#include <SPI.h>

/**
 * Maximum number of samples that can be used in the moving average filter. This is also the
 * default value.
 */
#define LOADCELL_MAX_SAMPLES 10

/**
 * Timer to use for sampling ADS1230. Timers 1 to 5 are supported.
 */
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
#define LOADCELL_TIMER 5
#elif defined(__AVR_ATmega32U4__)
#define LOADCELL_TIMER 3
#else
#define LOADCELL_TIMER 2
#endif

/**
 * A LoadCell Nanoshield or similar module using the ADS1230 IC.
 */
class Nanoshield_LoadCell
{
  public:
    /**
     * Constructor.
     *
     * Creates an object to access one LoadCell Nanoshield. The constructor parameters must match
     * the hardware configuration used.
     *
     * @param capacity    load cell capacity. Any measurement unit can be used.
     * @param sensitivity load cell sensitivity, in mV/V.
     * @param cs          chip select pin matching the jumper selected on the board (D4, D7, etc.).
     * @param hiGain      true if operating in high gain mode (GAIN jumper closed), false if in low
     *                    gain mode (GAIN jumper open).
     * @param numSamples  number of samples used in moving average filter. Minimum is one (no
     *                    filtering) and maximum is {@link LOADCELL_MAX_SAMPLES}.
     * @see   begin()
     */
    Nanoshield_LoadCell(float capacity, float sensitivity, int cs = 8,
                        bool hiGain = true, int numSamples = LOADCELL_MAX_SAMPLES);

    /**
     * Initializes the module.
     *
     * Initializes SPI, CS pin, Timer 2 interrupts and moving average circular buffer. Performs
     * initial self-calibration if requested via the \p calibrate parameter.
     *
     * @param calibrate if true, performs ADS1230 offset calibration during initialization.
     */
    void begin(bool calibrate = true);
    
    /**
     * Checks if a new load cell reading is available.
     *
     * @return true if a new load cell reading is available.
     */
    bool updated();

    /**
     * Gets the current 20-bit value, filtered and offset-corrected.
     *
     * @return current load cell value.
     */
    int32_t getValue();

    /**
     * Gets the current 20-bit value, filtered but not offset-corrected.
     *
     * @return current load cell value without offset correction.
     */
    int32_t getRawValue();

    /**
     * Gets the latest reading (unfiltered 20-bit value), offset-corrected.
     *
     * @return latest load cell value.
     */
    int32_t getLatestValue();

    /**
     * Gets the latest reading (unfiltered 20-bit value), offset-corrected.
     *
     * @return latest load cell value, without offset correction.
     */
    int32_t getLatestRawValue();

    /**
     * Gets the weight, filtered and offset-corrected.
     *
     * @return current weight reading.
     */
    float getWeight();
    
    /**
     * Sets the current value to be the zero-weight value (offset or tare).
     */
    void setZero();

    /**
     * Performs ADS1230 offset calibration.
     */
    void calibrate();

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
    
    void readDataIfReady();
    void resetBuffer();

    friend void timerOverflowIsr(int i);
};

#endif
