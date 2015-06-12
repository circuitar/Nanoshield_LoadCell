Nanoshield_LoadCell
===================

This is our Arduino library to measure weight using load cells with the ADS1230 IC from Texas Instruments.

The reference board to use this library is the [LoadCell Nanoshield](https://www.circuitar.com.br/en/nanoshields/modules/loadcell/) from Circuitar.

**Note**: this library uses Timer2 on the ATmega microcontroller.
On ATmega168 and ATmega328, usage of this library will interfere with the `tone()` function.

To install, just copy it under &lt;Arduino sketch folder&gt;/libraries/

The following examples are provided:

* **ReadWeight** shows how to do a simple weight measurement using a load cell.
* **MultiWeight** shows how to read more than one weight using multiple load cells.
* **ReadRawValue** reads the raw 20-bit integer value from the ADS1230 IC.
* **ReadWeightNoAveraging** reads weight without averaging (default is to average the last 10 samples).
* **ReadWeightLowGain** use low gain to measure a wider weight range.

---
Copyright (c) 2015 Circuitar
All rights reserved.

This software is released under an MIT license. See the attached LICENSE file for details.
