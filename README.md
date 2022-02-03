
Solar Air Heating Collector using ESP8266
-
This project is home DIY Solar Air Heating Collector System.

Background
--
This project aims to create DIY air heating collector system with use of ESP8266.
This device is cheap and also has low memory and so it took significant effort
to make program memory optimal. It also utilizes SSL client to push statistics 
so really tiny heap remains for program itself. There were also more problems with
cheap sensors and its reliability, which caused program to grow in complexity, e.g.
detecting deadlocks on i2c communication line. 


Functionality
--
- WiFi configuration
- Friendly WEB user interface
- One touch button for
  - activating notification diode
  - activating wifi
  - launching ventilator for 5 minutes on demand
- Green/Red signalization diode
- There are 5 pins used to update the software. There are intended to be connected to USB TTL UART, CP2102 with DRT pin without need to press reset and flash buttons while uploading new scretch or SPIFFS data.



![alt text](gui.png)
![alt text](gui-rules.png)
![alt text](gui-help.png)

Frontend
--
Frontend is written in **Typescript** using **React**, **Redux** and **Material UI**
Backend is executed on ESP8266 utilizing AsyncWebServer, etc..

Frontend as well as configuration is saved on SPIFFS.

**Parts used:**
--
- AI-thinker ESP8266-12E + adapter
- TTP223 touch button
- HTU21D - temperature and humidity sensor for inside
- SHT20 - temperature and humidity sensor for outside
- Small AC-DC converter + AMS1117 3.3V voltage regulator
- 230ACV tube ventilator
- RobotDyn AC Dimmer

= the total investement in these parts were about 850 CZK (33€)

Scheme
--
14 pin headers are used to connect frontpart with ESP8266, that can be hidden in socket box in the wall.

![alt text](fe/public/1level.png)
![alt text](fe/public/2level.png)

Reality
--
![alt text](photo.jpg)
