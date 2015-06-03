/**
 * Read multiple load cells using the ADS1230 IC in the LoadCell Nanoshield.
 *
 * Copyright (c) 2015 Circuitar
 * This software is released under the MIT license. See the attached LICENSE file for details.
 */
#include <SPI.h>
#include <Nanoshield_LoadCell.h>

// LoadCell Nanoshield with a 10kg load cell and jumper on pin D7
Nanoshield_LoadCell lc1(10000, 7);

// LoadCell Nanoshield with a 100kg load cell and jumper on pin D8
Nanoshield_LoadCell lc2(100000, 8);

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
  // If a reading has been completed, read weight from load cell
  if (lc1.dataReady() && lc2.dataReady()) {
    Serial.print(lc1.readWeight(), 0);
    Serial.print("g,");
    Serial.print(lc2.readWeight(), 0);
    Serial.println("g");
  }
}
