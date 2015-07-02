/**
 * Read raw 20-bit integer value from a load cell using the ADS1230 IC in the LoadCell Nanoshield.
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
//  - High gain (GAIN jumper closed)
//  - No averaging (number of samples = 1)
Nanoshield_LoadCell loadCell(100000, 3, 8, true, 1);

void setup() {
  Serial.begin(9600);
  loadCell.begin();
}

void loop() {
  if (loadCell.updated()) {
    Serial.println(loadCell.getLatestRawValue());
  }
}
