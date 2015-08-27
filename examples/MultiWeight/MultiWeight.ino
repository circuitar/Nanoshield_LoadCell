/**
 * @file MultiWeight.ino
 * Read multiple load cells using the ADS1230 IC in the LoadCell Nanoshield.
 *
 * This example uses the following default settings for both load cells:
 *  - High gain (GAIN jumper closed)
 *  - Averaging of the latest 10 samples
 *
 * Copyright (c) 2015 Circuitar
 * This software is released under the MIT license. See the attached LICENSE file for details.
 */
#include <SPI.h>
#include <Nanoshield_LoadCell.h>

// Load cell 1: 30kg capacity and 10mV/V sensitivity on pin D7
Nanoshield_LoadCell lc1(30000, 10, 7);

// Load cell 2: 100kg capacity and 3mV/V sensitivity on pin D8
Nanoshield_LoadCell lc2(100000, 3, 8);

void setup() {
  Serial.begin(9600);
  lc1.begin();
  lc2.begin();
  
  // Wait for calibration and set current value to zero weight (tare)
  while (!lc1.updated() || !lc2.updated());
  lc1.setZero();
  lc2.setZero();
}

void loop() {
  if (lc1.updated() && lc2.updated()) {
    Serial.print(lc1.getWeight(), 0);
    Serial.print("g,");
    Serial.print(lc2.getWeight(), 0);
    Serial.println("g");
  }
}
