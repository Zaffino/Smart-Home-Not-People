#ifndef __NODEMCU__H__
#define __NODEMCU__H__

#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPLE3t3VLM0"
#define BLYNK_DEVICE_NAME "Progetto"
#define BLYNK_AUTH_TOKEN "OFo0xKtMxl94h9YXCv_xVYkusAoyR_Y0"

#define TV_ID "63c00e0c42545d8b75a9568d"
#define APP_KEY "9441fe9d-d7b1-4ee7-97f8-590a8044a559" 
#define APP_SECRET "c025c864-a22b-4d62-b665-0b7bc161481d-edd1ae8b-e26b-4d5d-b114-77c371b622f4"

#define DEVICE_ADDRESS 1          //device address for the I2C communication

/***************************************
*
* setup() - Function that setups I2C connection used to communicate with the Arduino and the connection with the application Blynk
*
****************************************/
void setup();

/***************************************
*
* loop() - Function that keeps active the Blynk connection (application), the I2C connection with Arduino and manage every request from Blynk;
*          it also updates different values sending them to Blynk (e.g temperature and humidity)
*
****************************************/
void loop();

/***************************************
*
* get_temperature() - Function that send temperature and humidity values to the Blynk application
*
* double t: temperature value
* double h: humidity value
*
****************************************/
void get_temperature(double t, double h);

#endif