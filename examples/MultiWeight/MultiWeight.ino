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

// Load cell 1: 30kg capacity and 10mV/V sensitivity on pin D4
Nanoshield_LoadCell lc1(30000, 10, 4);

// Load cell 2: 100kg capacity and 3mV/V sensitivity on pin D7
Nanoshield_LoadCell lc2(100000, 3, 7);

// Load cell 2: 50kg capacity and 2mV/V sensitivity on pin D8
Nanoshield_LoadCell lc3(50000, 2, 8);

// Load cell 2: 150kg capacity and 2mV/V sensitivity on pin D10
Nanoshield_LoadCell lc4(150000, 2, 10);

// Load cell 2: 100kg capacity and 1.5mV/V sensitivity on pin A3
Nanoshield_LoadCell lc5(100000, 1.5, A3);


void setup() {
  Serial.begin(9600);
  lc1.begin();
  lc2.begin();
  lc3.begin();
  lc4.begin();
  lc5.begin();
  
  // Wait for calibration and set current value to zero weight (tare)
  while (!lc1.updated()
         || !lc2.updated()
         || !lc3.updated()
         || !lc4.updated()
         || !lc5.updated());

  lc1.setZero();
  lc2.setZero();
  lc3.setZero();
  lc4.setZero();
  lc5.setZero();
}

void loop() {
  if (lc1.updated() 
      && lc2.updated()
      && lc3.updated()
      && lc4.updated()
      && lc5.updated()) {
    Serial.print(lc1.getWeight(), 0);
    Serial.print("g, ");
    Serial.print(lc2.getWeight(), 0);
    Serial.print("g, ");
    Serial.print(lc3.getWeight(), 0);
    Serial.print("g, ");
    Serial.print(lc4.getWeight(), 0);
    Serial.print("g, ");
    Serial.print(lc5.getWeight(), 0);
    Serial.println("g");
  }
}
