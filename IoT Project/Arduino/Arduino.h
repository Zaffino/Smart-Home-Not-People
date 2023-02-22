#ifndef __ARDUINO__H__
#define __ARDUINO__H__

#define DHTTYPE DHT11         //Specifies the type of the DHT module that we are using
#define DHTPIN A0             //Pin of the arduino module that receive Analog Data from the DHT module

#define INTERNAL_LIGHT 2      //Pin of the arduino module that sends signal to the internal light led
#define MOTION_SENSOR_PIN 3   //Pin of the arduino module that communicate with the motion sensor
#define MOTION_LED 4          //Pin of the arduino module that sends signal to the led in case of movement
#define LIGHT_SENSOR A1       //Pin of the arduino module that communicates with the light sensor
#define LED_LIGHT_SENSOR 5    //Pin of the arduino module that sends signal to the led
#define BLINDPIN 6            //Pin of the arduino module that communicates with the servo used for the blind
#define DOORPIN 7             //Pin of the arduino module that communicates with the servo used for the door
#define SS_PIN 10             //Pin of the arduino module used to select slave to communicate with RFID
#define RST_PIN 9             //Pin of the arduino module used to reset the RFID

#define DEVICE_ADDRESS 1      //device addres for I2C communication

/* To use the RFID module we need three more pins
* MOSI: Pin 11 / ICSP-4           // Master Output Slave Input
* MISO: Pin 12 / ICSP-1           // Master Input Slave Output
* SCK: Pin 13 / ISCP-3            // Syncronous Communication Clock
*/


/***************************************
*
* setup() - Function that setups Arduino pins, sensors and the I2C connection used to communicate with the NodeMcu
*
****************************************/
void setup();

/***************************************
*
* loop() - Function that keeps active certain functionality of the code
*
****************************************/
void loop();


/***************************************
*
* first_read() - Function that reads the first card ever used to access, the card ID will then be saved and the function will
*                set the flg master_card_registerd as true.
*
****************************************/
void first_read();

/***************************************
*
* is_master_card() - Function that checks if the card that the RFID is reading is the master card or not.
* 
* bool (return): true if it's the master card, not otherwise
*
****************************************/
bool is_master_card();

/***************************************
*
* read_master_card() - Function that reads the cards and calls the function is_master_card(), if it's the master card the flag
*                      card_id is set as true and it's mean that the card reading has been succesful.
*
****************************************/
void read_master_card();

/***************************************
*
* read_card() - First fucntion that it's executed when we place a card in front of RFID, if it's not the first time that we read a card
*               it will call the function read_master_card() or it will call the function first_read();
*
****************************************/
void read_card();


/***************************************
*
* motion() - Function that checks the state of the motion sensor: if there's a movement it will turn on the led, otherwise it will turn off
*            the led or it will do nothing
*
****************************************/
void motion();


/***************************************
*
* sensore_movimento() - It triggers ultrasonic sound pulses and then it waits until it receive an echo signal. 
*                       Then it use the time between the sending of the signal and the receiving of the echo to compute the distance 
*                       between the sensor and other objects.
*                       If the distance is different from a fixed value it will trigger a signal and turn on a led (that simulate a lamp).
****************************************/
void sensore_movimento();            

/***************************************
*
* void temperatura() - Function that reads the Temperature and Humidity values that are provided by the DHT11 sensor. These values
*                      are passed from the sensor to the Arduino through digital signals. 
*
****************************************/
void temperature();

/***************************************
*
* light_Sensor() - Function that checks the value provided by the light sensor through an analog pin.
*                  If the value received is less than a fixed one, then the function will turn on the light, nothing otherwise.
*
****************************************/
void light_Sensor();

/***************************************
*
* receiveEvent() - Function that manage the two servos connected to Arduino: one used as a door and one used as a blind.
*                  This function will be executed when the Arduino will receive a request from the NodeMcu
*
* int howMany: Parameter not used but requested by default for the I2C communication
****************************************/
void receiveEvent(int howMany);

/***************************************
* 
* requestEvent() - Function that sends different data to the NodeMcu
*                  The data sent can be: temperature and humidity values and status about the leds (light sensor led and movement sensor led).
*                  This function will be executed when the Arduino will receive a request from the NodeMcu
*
****************************************/
void requestEvent();

#endif