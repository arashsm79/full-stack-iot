# Full Stack IoT Platform
> A simple implementation of an IoT platform using esp-idf. Including Sensors, Gateway, and a Server.

* [Introduction](#Introduction)
* [Sensors](#Sensors)
* [Gateway](#Gateway)
* [Sever](#Sever)
* [Dependencies](#Dependencies)

## Introduction
This project uses three esp32 modules; Two sensors and a gateway which are all flashed using esp-idf.
In a nutshell, the two sensors send their data to the gateway and the gateway sends the aggregated data to the server. The server validates the data and stores them in a SQLite database.

The connection between the sensors and the gateway is via HTTP over Wifi and the connection between the server and the gateway is via HTTPS over Wifi and the Internet.
![overview](/screenshots/overview.png)

## Sensors
The sensors are written in C and use esp-idf's rich libraries and APIs along with [FreeRTOS](https://www.freertos.org/).
The sensors use [esp32-wifi-manager](https://github.com/tonyp7/esp32-wifi-manager) for their initial configuration. They first start in AP mode and the user has to connect to their AP using another device. After loading the webpage served at the default gateway, they can set the SSID and PASSWORD of the gateway's access points. After the initial configuration, the SSID and PASSWORD are saved to the ROM for future use.

After the sensor gets connected to the AP of the gateway (the station of the sensor gets an IP), it spins up two tasks and creates a queue:
- `sensor_task`: Every 10 seconds generates random values and treats them as real sensor data. The values are placed in a heap-allocated struct and are sent to a queue.
- `gateway_client_task`: Sleeps on the receiving end of the queue and wakes up whenever there is a new sensor reading message on the queue. Pops out the message and creates a JSON object out of it using [cJSON](https://github.com/DaveGamble/cJSON). It then sends then converts the JSON object to a string and sends it to the gateway using HTTP

Here is an example of the JSON object and the sensors send:
```json
{
	"name": "30:ae:a4:00:97:a8",
	"timestamp":    2696,
	"temperature":  31,
	"humidity":     56
}
```

## Gateway
The gateway is also written in C and uses esp-idf along with [FreeRTOS](https://www.freertos.org/).
The gateway uses [esp-gateway](https://github.com/espressif/esp-gateway) for its initial configuration. It first starts in AP mode and the user has to connect to their AP using another device. After loading the webpage served at the default gateway, they can set the SSID and PASSWORD of another access point (the access point serves as a router between gateway and server). After the initial configuration, the SSID and PASSWORD are saved to the ROM for future use.

The code for the gateway is in `/gateway/components/web_server/src/web_server.c` and starts from line `1898` where it says `// Custom code` (the rest of the codes are from esp-gateway).

After the gateway gets connected to the AP of a router (the station of the gateway gets an IP), it spins up a task and creates a queue. It also registers a handler for the `/device` endpoint where sensors send their data to. It also creates a JSON array that acts as a buffer; whenever it gets full, the buffer is flushed to the server (aggregation of data).

- `device_post_handler`: Receives post request on the `/device` endpoint from the sensors. It parses the JSON into a cJSON object and adds it to the buffer. If the length of the buffer is above a certain threshold, it sends a `BUFFER_FULL` event into the queue.

- `server_client_task`: Sleeps on the receiving end of the queue and wakes up whenever there is a new event. If the new event is `BUFFER_FULL`, it empties out the buffer and flushes the JSON array to the server on the `telemetry` endpoint via HTTPS.

Here is an example of the JSON object and the gateway sends:
```json
{
   "name": "24:0a:c4:80:cb:b0",
   "data": [{
                   "name": "24:0a:c4:80:cd:84",
                   "timestamp":    2727,
                   "luminance":    48
           }, {
                   "name": "30:ae:a4:00:97:a8",
                   "timestamp":    2696,
                   "temperature":  31,
                   "humidity":     56
           }, {
                   "name": "24:0a:c4:80:cd:84",
                   "timestamp":    2730,
                   "luminance":    36
           }, {
                   "name": "24:0a:c4:80:cd:84",
                   "timestamp":    2734,
                   "luminance":    32
           }, {
                   "name": "30:ae:a4:00:97:a8",
                   "timestamp":    2703,
                   "temperature":  32,
                   "humidity":     56
           }, {
                   "name": "24:0a:c4:80:cd:84",
                   "timestamp":    2737,
                   "luminance":    42
           }, {
                   "name": "24:0a:c4:80:cd:84",
                   "timestamp":    2737,
                   "luminance":    39
           }]
}
```
## Server
A super simple Django app that receives data from the gateway and parses the JSON. It then verifies that the names of the devices in the JSON are valid; if they are indeed valid, it stores the time series into a SQLite database. You can add devices using Django admin (username: admin, password: admin). The name of the device is its MAC address.

Database ER diagram:
![er](/screenshots/db-1.png)

Example timeseries:
![timeseries](/screenshots/db-2.png)

Example devices:
![devices](/screenshots/db-3.png)

There is a jupyter notebook in the server directory that connects to the SQLite database and periodically plots the sensor's readings.

![ui](/screenshots/ui-1.png)
![ui](/screenshots/ui-2.png)


## Dependencies
* [esp-idf](https://github.com/espressif/esp-idf)
* [esp32-wifi-manager](https://github.com/tonyp7/esp32-wifi-manager)
* [esp-gateway](https://github.com/espressif/esp-gateway)
