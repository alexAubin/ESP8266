ESP8266
========

Experimentation / implementation of an hardware abstraction layer for the [ESP8266 (Olimex version)](https://www.olimex.com/Products/IoT/MOD-WIFI-ESP8266/open-source-hardware).

Introduction
============

The ESP8266 offers a great oppportunity of connecting electronics project to the Internet
because of its relative simplicity of use and cheap cost (around 5 euros or dollars). However
as it is quite a new technology, and despite the enthusiasm of the community, it still lacks
documentation, simple examples as well as robust implementations to easily interface it with 
the Arduino Uno.

This document lists several points that a developper willing to use the ESP8266 in a project 
should be aware of. Additionnaly the related github project aims to provide a user-friendly
library for the ESP8266 for it to be as easy to use as possible in the context of Arduino
Uno based projects and sending POST/GET requests.

General description of the module
=================================

The module is made of these elements :
- the actual ESP8266 chip which is the core of the module
- the antenna that receive and emits signal
- a flash memory
- the pinout to interface the module with the rest of your project.

![Setup](doc/description.jpg)

The module is a TTL serial device, meaning it it communicates using the same protocol used 
to program your Uno, writing datas on some RX and TX pins. Note that the pinout of the Olimex 
version of the module is quite different from the more official module. In particular, the 
GPIO pins need to be set to a particular state when trying to upgrade the firmware of the
module, and they are not easiliy accessible on the Olimex version (you will need a soldering 
iron).

Another important remark is that the ESP8266 operates at 3.3V. This is not a problem regarding
the powering as the Uno provides directly a 3.3V, but it might be a problem for the Serial
communication as the Uno will send data using 5V voltage whereas the ESP8266 expects 3.3V,
and vice-versa. To be entirely clean, one would need a voltage converter but it turns out
that a direct connexion also works (at least if you are not afraid of loosing your ESP8266).

Finally, there are comments on the Internets suggesting that the module might require
more than 200mA to operate (up to 1A ?). So far I was able to operate the module without
any need of an additional power stage, but that might also explain some of the instabilities
encountered.

The firmware
============

As mentionned, the firmware of the module can be upgraded provided that the GPIO and other
pins are mapped appropriately and you've downloaded the appropriate tools. However I didn't 
find any strong motivation at this point to upgrade it, except for the fact that one might
be able to change the baud rate used for serial communication by the ESP8266. It might
also be that if you're using a different firmware version, the format of the communication
with the ESP8266 changes. 

[Firmware version used when developping HAL goes here]

[Links related to firmware upgrade goes here]

Other threads over the Internets mention the SDK, which to my understanding is related to
developping your own firmware but I'm not intersted in that here.

Interfacing with Arduino Uno
============================

As mentionned, the module uses TTL serial communication. TTL serial capabilities are provided
natively by the Uno on the pin 0 and 1. However, as this is also a mean for debugging your
project by writing messages and monitoring them on the Serial Monitor, you probably want
to have two Serials at the same time. This is possible using the SerialSoftware library
which can emulate TTL serial communication on other pins of your Uno. That's not the end
of the story as in my case I was not able to get a stable connexion on a software serial
with the baudrate of the ESP (115200). Therefore I went for this solution :
- pins 0 and 1, i.e. native Uno serial, are used for communicating with the ESP
- pins 2 and 3 are used via a software serial and interfaced with your laptop via a FTDI chip (USB <-> TTL)

![Setup](doc/wiring.png)

Commands format
===============

Commands to the ESP8266 are sent using a format similar to other devices using TTL 
communication, such as GSMs. The format is "AT+[command name]" or "AT+[command name]=[arguments]"
if the command requires arguments. For instance, "AT+RST" will reset the module, then
it will send a bunch of information and finish with "ready". At any moment, you may send
simply "AT" to check the status of your device and it should answer something like "OK" 
or "busy" depending on its status.

Concrete example
================

```
// Create a software serial on pins 2 and 3 for debugging purpose

SoftwareSerial dbgSerial(2,3);

// Starts the serial communications with appropriate baud rate

dbgSerial.begin(9600); // This is for debugging
Serial.begin(115200);  // This is for the ESP

// Send reset command to the ESP

String command = "AT+RST";
dbgSerial.println("---- Sending :");
dbgSerial.println(command);

Serial.print(command);
Serial.print("\r\n");

// Read ESP answer during timeout = 2s

int timeout = 2000; 
String response = "";
long deadline = millis() + timeout;
while (millis() < deadline)
if (Serial.available())
{
    response += (char) Serial.read();
}

// Print answer on debugging

dbgSerial.println("---- Receiving :");
dbgSerial.println(response);

// Use a search/find function on the response string to find a "ready" substring.
bool ack = find(response,"ready");

// If "ready" has not been found, stop the Uno by looping to infinity
if (!ack) while (1) { }
```

Hardware abstraction layer
==========================

Todo

Ressources, references
======================

Todo

