# Full Stack IoT Platform
> A simple implementation of an IoT platform using esp idf. Including: Sensors, Gateway and a Server.

* [Introduction](#Introduction)
* [Sensors](#Sensors)
* [Gateway](#Gateway)
* [Sever](#Sever)
* [Dependencies](#Dependencies)

## Introduction
This projects uses three esp32 modules; Two sensors and a gateway which are all flashed using esp-idf.
In a nutshell, the two sensors send their data to the gateway and the gate way sends the aggregated data to the server. The sever validates the data and stores them in a sqlite data base.
![overview](/screenshots/overview.png)

## Dependencies
* [esp-idf](https://github.com/espressif/esp-idf)
* [esp32-wifi-manager](https://github.com/tonyp7/esp32-wifi-manager)
* [esp-gateway](https://github.com/espressif/esp-gateway)
