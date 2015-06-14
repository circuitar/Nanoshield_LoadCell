Nanoshield_LoadCell
===================

This is an Arduino library to measure weight using load cells with the ADS1230 IC from Texas Instruments.

* Full documentation available on ReadTheDocs_
* Source code available on _GitHub
* Reference board to use this library is the `LoadCell Nanoshield`_ from Circuitar

Library features include:

* Conversion to weight units, based on load cell capacity and sensitivity
* Moving average filter, with configurable number of samples
* Set zero weight point (tare)
* Get measurement in weight units or raw 20-bit value
* Continuous sampling using hardware timer (Timer 2)
* ADS1230 offset calibration

**Note**: this library uses Timer2 on the ATmega microcontroller.
On ATmega168 and ATmega328, usage of this library will interfere with the `tone()` function.

To install, just copy it under ``<Arduino sketch folder>/libraries/``

The following examples_ are provided:

* ReadWeight_ shows how to do a simple weight measurement using a load cell.
* MultiWeight_ shows how to read more than one weight using multiple load cells.
* ReadRawValue_ reads the raw 20-bit integer value from the ADS1230 IC.
* ReadWeightNoAveraging_ reads weight without averaging (default is to average the last 10 samples).
* ReadWeightLowGain_ use low gain to measure a wider weight range.
* OffsetCalibration_ shows how to use offset calibration.

.. _ReadTheDocs: http://nanoshield-loadcell.readthedocs.org
.. _GitHub: https://github.com/circuitar/Nanoshield_LoadCell
.. _`LoadCell Nanoshield`: https://www.circuitar.com.br/en/nanoshields/modules/loadcell/
.. _examples: https://github.com/circuitar/Nanoshield_LoadCell/blob/readthedocs/examples/
.. _ReadWeight: https://github.com/circuitar/Nanoshield_LoadCell/blob/readthedocs/examples/ReadWeight/ReadWeight.ino
.. _MultiWeight: https://github.com/circuitar/Nanoshield_LoadCell/blob/readthedocs/examples/MultiWeight/MultiWeight.ino
.. _ReadRawValue: https://github.com/circuitar/Nanoshield_LoadCell/blob/readthedocs/examples/ReadRawValue/ReadRawValue.ino
.. _ReadWeightNoAveraging: https://github.com/circuitar/Nanoshield_LoadCell/blob/readthedocs/examples/ReadWeightNoAveraging/ReadWeightNoAveraging.ino
.. _ReadWeightLowGain: https://github.com/circuitar/Nanoshield_LoadCell/blob/readthedocs/examples/ReadWeightLowGain/ReadWeightLowGain.ino
.. _OffsetCalibration: https://github.com/circuitar/Nanoshield_LoadCell/blob/readthedocs/examples/OffsetCalibration/OffsetCalibration.ino

----

Copyright (c) 2015 Circuitar
All rights reserved.

This software is released under an MIT license. See the attached LICENSE file for details.
