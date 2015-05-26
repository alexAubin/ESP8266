ESP8266
========

Experimentation / implementation of an hardware abstraction layer for the [ESP8266 (Olimex version)](https://www.olimex.com/Products/IoT/MOD-WIFI-ESP8266/open-source-hardware).

Basic setup / wiring
--------------------

The ESP is connected to the standard Serial (pin 0 and 1) while the pin 2 and 3 are used for debugging using a software serial communication via a FTDI. Right now my understanding is that software serial does not work with the default baudrate of the ESP (115200), which would of course simplify things.

![Setup](doc/wiring.png)
