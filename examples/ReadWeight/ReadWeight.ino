/**
 * Read weight from a load cell using the ADS1230 IC in the LoadCell Nanoshield.
 *
 * Copyright (c) 2015 Circuitar
 * This software is released under the MIT license. See the attached LICENSE file for details.
 */
#include <SPI.h>
#include <Nanoshield_LoadCell.h>

// LoadCell Nanoshield with a 100kg load cell and jumper on pin D8
Nanoshield_LoadCell loadCell(100000, 8);

void setup() {
  Serial.begin(9600);
  
  // Initialize load cell and wait for calibration
  loadCell.begin();
  delay(1000);
  
  // Set current value to zero weight (tare)
  loadCell.setZero();
}

void loop() {
  // If a reading has been completed, read weight from load cell
  if (loadCell.dataReady()) {
    Serial.print(loadCell.readWeight(), 0);
    Serial.println("g");
  }
}
