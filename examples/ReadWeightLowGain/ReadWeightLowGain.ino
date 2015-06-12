/**
 * Read weight from a load cell using the ADS1230 IC in the LoadCell Nanoshield, with low gain.
 *
 * A low gain can be used when measuring weight over a wide range. To use a low gain the GAIN jumper
 * in the LoadCell Nanoshield must remain open. This example uses averaging of 10 samples.
 *
 * Copyright (c) 2015 Circuitar
 * This software is released under the MIT license. See the attached LICENSE file for details.
 */
#include <SPI.h>
#include <Nanoshield_LoadCell.h>

// LoadCell Nanoshield with the following parameters:
//  - Load cell capacity: 100kg
//  - Load cell sensitivity: 3mV/V
//  - CS on pin D8 (D8 jumper closed)
//  - Low gain (GAIN jumper open)
Nanoshield_LoadCell loadCell(100000, 3, 8, false);

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
