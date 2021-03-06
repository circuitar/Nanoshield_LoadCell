/**
 * Read weight from a load cell using the ADS1230 IC in the LoadCell Nanoshield.
 *
 * This example uses the following default settings:
 *  - CS pin set to D8 (D8 jumper closed)
 *  - High gain (GAIN jumper closed)
 *  - Averaging of the latest 10 samples
 *
 * Copyright (c) 2015 Circuitar
 * This software is released under the MIT license. See the attached LICENSE file for details.
 */
#include <SPI.h>
#include <Nanoshield_LoadCell.h>

// Load cell with 100kg capacity and 3mV/V sensitivity
Nanoshield_LoadCell loadCell(100000, 3);

void setup() {
  Serial.begin(9600);
  loadCell.begin();
  
  // Wait for calibration and set current value to zero weight (tare)
  while (!loadCell.updated());
  loadCell.setZero();
}

void loop() {
  if (loadCell.updated()) {
    Serial.print(loadCell.getWeight(), 0);
    Serial.println("g");
  }
}
