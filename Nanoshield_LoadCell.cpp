/**
 * This is an Arduino library to measure weight using load cells with the ADS1230 IC from Texas Instruments.
 *
 * Copyright (c) 2015 Circuitar
 * This software is released under the MIT license. See the attached LICENSE file for details.
*/
#include <Nanoshield_LoadCell.h>

SPISettings Nanoshield_LoadCell::spiSettings = SPISettings(4000000, MSBFIRST, SPI_MODE1);
int Nanoshield_LoadCell::timeout = 120;

Nanoshield_LoadCell::Nanoshield_LoadCell(float maxWeight, int cs) {
  this->cs = cs;
  this->maxWeight = maxWeight;
}

void Nanoshield_LoadCell::begin() {
  pinMode(cs, OUTPUT);
  digitalWrite(cs, HIGH);
  SPI.begin();
}

void Nanoshield_LoadCell::setZero() {
  offset = read();
}

bool Nanoshield_LoadCell::dataReady() {
  digitalWrite(cs, LOW);
  bool result = digitalRead(12) == LOW;
  digitalWrite(cs, HIGH);
  return result;
}

int32_t Nanoshield_LoadCell::read() {
  int32_t result = 0;
  
  // Wait until data is ready
  unsigned int t0 = millis();
  while(!dataReady() && millis() - t0 < timeout);

  // Read data vis SPI
  SPI.beginTransaction(spiSettings);
  digitalWrite(cs, LOW);
  result |= SPI.transfer(0);
  result <<= 8;
  result |= SPI.transfer(0);
  result <<= 8;
  result |= SPI.transfer(0);
  result <<= 8;
  digitalWrite(cs, HIGH);
  SPI.endTransaction();
  
  return result / (1L << 12);
}

float Nanoshield_LoadCell::readWeight() {
  return maxWeight * (read() - offset) / (1L << 19);
}
