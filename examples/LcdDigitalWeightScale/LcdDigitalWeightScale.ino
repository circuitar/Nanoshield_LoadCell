/**
 * A digital weight scale using a load cell and an LCD display.
 *
 * This example uses the following default settings:
 *  - CS pin set to D8 (D8 jumper closed)
 *  - High gain (GAIN jumper closed)
 *  - Averaging of the latest 10 samples
 *
 * Copyright (c) 2017 Circuitar
 * This software is released under the MIT license. See the attached LICENSE file for details.
 */
#include <SPI.h>
#include "Nanoshield_LoadCell.h"
#include "Nanoshield_LCD.h"

// Load cell with 100kg capacity and 3mV/V sensitivity
Nanoshield_LoadCell loadCell(100000, 3);

// LCD Nanoshield
Nanoshield_LCD lcd;

void setup() {
  // Initialize LCD and load cell
  lcd.begin();
  loadCell.begin();

  // Wait for calibration and set current value to zero weight (tare)
  while (!loadCell.updated());
  loadCell.setZero();
}

void loop() {
  if (loadCell.updated()) {
    // IF there is a new reading, print the weight on the LCD
    lcd.home();
    lcd.print("Weight: ");
    lcd.printPadded(loadCell.getWeight(), 7);
    lcd.print("g");

    // Wait so that the LCD info doesn't change too fast
    delay(200);
  }
}
