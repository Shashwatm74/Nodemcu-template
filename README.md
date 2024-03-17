# Project Title

This project is a sensor data collection and display system using ESP8266, ultrasonic sensors, and GPS module.

## Description

The system collects data from two ultrasonic sensors and a GPS module. The data is then served on a local web server hosted on the ESP8266. The ultrasonic sensor data is displayed on the webpage, and the GPS data is processed and stored.

## Libraries Used

- `TinyGPS++.h`: This library is used to parse the data coming from the GPS module.
- `SoftwareSerial.h`: This library provides serial communication using software, which is used to communicate with the GPS module.
- `ESP8266WiFi.h`: This library allows the ESP8266 to connect to a Wi-Fi network or act as an access point.
- `ESP8266WebServer.h`: This library allows the ESP8266 to host a web server.
- `ESP8266HTTPClient.h`: This library allows the ESP8266 to send HTTP requests.

## Setup

The system uses two ultrasonic sensors and a GPS module. The ultrasonic sensors are connected to the ESP8266 via digital pins. The GPS module is connected via software serial.

## Code Explanation

The code sets up a Wi-Fi access point, initializes the ultrasonic sensors and the GPS module, and starts a web server. In the main loop, it reads data from the ultrasonic sensors and the GPS module periodically, and serves the ultrasonic sensor data on the webpage.

## Usage

Upload the code to your ESP8266 device, power it on, and connect to the Wi-Fi access point it creates. Open a web browser and navigate to the IP address of the ESP8266 to view the sensor data.
