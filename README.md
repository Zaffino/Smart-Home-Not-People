TODO

project layout

check compile and execution

add links





# PROJECT SMART HOME NOT PEOPLE

Smart home not people is a model of a smart home.

Using an Arduino UNO and a mcu with an embodied WIFI receiver, any person with an internet connection can build his own smart home model and control it from his own smartphone.

These two mcu work together using I²C technology as master and slave. 



## Requirements

###### Software requirements 

- The Blynk app
- [RFID library](https://www.progettiarduino.com/uploads/8/1/0/8/81088074/rfidmaster.zip)
- Servo library
- arduino-timer library
- Blynk library
- DHT sensor library
- WiFiEsp library
- Wire library
- Stepper library
- Servo library

Every library without link can be added from the arduino IDE.



###### Hardware requirements

- Arduino UNO
- NodeMCU esp8266
- One smartphone with available internet connection
- RFID reader RC522 with a compatible card
- One 180° Servo motor SG90
- One 360° Servo motor
- One temperature and humidity sensor DHT11
- One photoresistor
- One movement sensor HC-SR501
- Two leds
- Some resistor
- External power supply between 6V and 10V



## Project layout

```
├── readme.MD
├── NodeMCU
|   ├── NodeMCU.ino
│   └── NodeMCU.h
└── Arduino
    ├── Arduino
    └── Arduino.h
```



## Compiling and execution

###### Blynk

Blynk is needed to make most of the smart home work, and to make a working Blynk application you need to follow the instructions given here:

On the Blynk app add a new device.

Click on the wrench icon and add the following widgets:

| Widget type | Datastream | Other details          |
| :---------- | :--------- | :--------------------- |
| Gauge       | V0         | Double                 |
| Gauge       | V1         | Integer                |
| Switch      | V2         | Off = 1 On = 0 Integer |
| Switch      | V2         | Off = 3 On = 2 Integer |
| Switch      | V3         | Off = 4 On = 5 Integer |
| LED         | V4         | Integer                |
| LED         | V5         | Integer                |

Go to Project Settings > Code and copy the code.

Paste the code on your Node.h file.



###### Compiling

You need to follow [this](https://docs.google.com/presentation/d/1yFMCCMylUWCsUBBLMD4ziyuEtQKCLL_s7DBCxvGskGc/edit?usp=sharing) scheme to connect every module and sensor used.

Remember to download every library we need.



To use this application you also need arduino IDE to burn the code on your Arduino UNO and your NodeMCU esp8266.



There is a change you need to make to let your NodeMCU to connect to the WiFi.

```c
//replace your wifi SSID and PASSWORD on the nodeMCU.ino file following this example
const char* ssid = "your_ssid";
const char* pass = "your_password";
```



###### Execution

The smartphone application gives you control of the rolling shutters and the door, the door can also be opened if you swipe your card on the RFID reader.

The first time you use the RFID reader it won't work because it's registering it as the master card, the only card that can open the door.

If outside the home there isn't enough light it will light a LED, the other LED will be light by the ultrasonic sensor. 

Inside the home there is a movement sensor to check if there is someone inside and give power a LED.



## Contributors

Alessandro Cacciabue: Cleaned the code, wrote documentation, fixed the project layout and prepared the powerpoint presentation.

Daniele Obrelli: Implemented most of the sensors and integrated them with Blynk.

Cosimo Tobia Zaffino: Took care of the Hardware and implemented the RFID reader and the movement sensor.



## Powerpoint and YouTube links

yt: link

[Powerpoint](https://docs.google.com/presentation/d/1yFMCCMylUWCsUBBLMD4ziyuEtQKCLL_s7DBCxvGskGc/edit?usp=sharing)





