/**
 * Read multiple load cells using the ADS1230 IC in the LoadCell Nanoshield.
 *
 * Copyright (c) 2015 Circuitar
 * This software is released under the MIT license. See the attached LICENSE file for details.
 */
#include <SPI.h>
#include <Nanoshield_LoadCell.h>

// LoadCell Nanoshield with the following parameters:
//  - Load cell capacity: 30kg
//  - Load cell sensitivity: 10mV/V
//  - Low gain (GAIN jumper open)
//  - Jumper on pin D7
Nanoshield_LoadCell lc1(100000, 3, false, 7);

// LoadCell Nanoshield with the following parameters:
//  - Load cell capacity: 100kg
//  - Load cell sensitivity: 3mV/V
//  - High gain (GAIN jumper closed)
//  - Jumper on pin D8
Nanoshield_LoadCell lc2(100000, 3, true, 8);

void setup() {
  Serial.begin(9600);
  
  // Initialize load cell and wait for calibration
  lc1.begin();
  lc2.begin();
  delay(1000);
  
  // Set current value to zero weight (tare)
  lc1.setZero();
  lc2.setZero();
}

void loop() {
  Serial.print(lc1.getWeight(), 0);
  Serial.print("g,");
  Serial.print(lc2.getWeight(), 0);
  Serial.println("g");
  delay(100);
}
