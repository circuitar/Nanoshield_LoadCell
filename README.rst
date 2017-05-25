Nanoshield_LoadCell
===================

This is an Arduino library to measure weight using load cells and the ADS1230 IC from Texas Instruments.

* Source code: https://github.com/circuitar/Nanoshield_LoadCell
* Documentation: http://nanoshield-loadcell.readthedocs.org/
* Reference board: `LoadCell Nanoshield`_ from Circuitar_

Library features include:

* Conversion to weight units, based on load cell capacity and sensitivity
* Moving average filter, with configurable number of samples
* Set zero weight point (tare)
* Get measurement in weight units or raw 20-bit value
* Continuous sampling using hardware timer (Timer 2)
* ADS1230 offset calibration

**Note**: this library uses Timer 2 on the ATmega microcontroller, or Timer 5 on ATmega2560 and ATmega1280.
On ATmega168 and ATmega328 (i.e. Arduino UNO or similar), usage of this library will interfere with the `tone()` function.

To install, just click **Download ZIP** and install it using **Sketch > Include Library... > Add .ZIP Library** in the Arduino IDE.

The following examples_ are provided:

* ReadWeight_ shows how to do a simple weight measurement using a load cell.
* LcdDigitalWeightScale_ a digital weight scale using a load cell and an LCD display.
* MultiWeight_ shows how to read more than one weight using multiple load cells.
* ReadRawValue_ reads the raw 20-bit integer value from the ADS1230 IC.
* ReadWeightNoAveraging_ reads weight without averaging (default is to average the last 10 samples).
* ReadWeightLowGain_ uses low gain to measure a wider weight range.
* OffsetCalibration_ shows how to use offset calibration.

.. _`LoadCell Nanoshield`: https://www.circuitar.com/nanoshields/modules/loadcell/
.. _Circuitar: https://www.circuitar.com/
.. _examples: https://github.com/circuitar/Nanoshield_LoadCell/blob/master/examples/
.. _ReadWeight: https://github.com/circuitar/Nanoshield_LoadCell/blob/master/examples/ReadWeight/ReadWeight.ino
.. _LcdDigitalWeightScale: https://github.com/circuitar/Nanoshield_LoadCell/blob/master/examples/LcdDigitalWeightScale/LcdDigitalWeightScale.ino
.. _MultiWeight: https://github.com/circuitar/Nanoshield_LoadCell/blob/master/examples/MultiWeight/MultiWeight.ino
.. _ReadRawValue: https://github.com/circuitar/Nanoshield_LoadCell/blob/master/examples/ReadRawValue/ReadRawValue.ino
.. _ReadWeightNoAveraging: https://github.com/circuitar/Nanoshield_LoadCell/blob/master/examples/ReadWeightNoAveraging/ReadWeightNoAveraging.ino
.. _ReadWeightLowGain: https://github.com/circuitar/Nanoshield_LoadCell/blob/master/examples/ReadWeightLowGain/ReadWeightLowGain.ino
.. _OffsetCalibration: https://github.com/circuitar/Nanoshield_LoadCell/blob/master/examples/OffsetCalibration/OffsetCalibration.ino

----

Copyright (c) 2015 Circuitar
All rights reserved.

This software is released under an MIT license. See the attached LICENSE file for details.
