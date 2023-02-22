#include <Wire.h>
#include <DHT.h>
#include <Servo.h>
#include <SPI.h>
#include <RFID.h>
#include <arduino-timer.h>
#include "Arduino.h"

//const int stepsPerRevolution = 200;   //.h

//DHT variables
DHT dht(DHTPIN, DHTTYPE);
float h, t; //h = humidity, t = temperature


//variable used for RFID
RFID rfid(SS_PIN, RST_PIN); 
int recordedNum[5];
bool master_card_registered = false;

//servos variables
Servo blind, porta;

//light sensor variables
bool light;     //bool that tell if the light is on or off

//RFID variable 
bool card_id = false;   //tell if the card is read or not


//movemente (motion) sensor variables
bool led_on_off;
int motionStateCurrent      = LOW; // current  state of motion sensor's pin
int motionStatePrevious     = LOW; // previous state of motion sensor's pin

//setup of Arduino 
void setup() {

  //servo setup
  blind.attach(BLINDPIN);     
  porta.attach(DOORPIN);
  porta.write(90);

  //led setup  
  pinMode (LED_LIGHT_SENSOR, OUTPUT);
  pinMode(MOTION_LED, OUTPUT);
  pinMode(INTERNAL_LIGHT, OUTPUT);

  //motion sensor setup
  pinMode(MOTION_SENSOR_PIN, INPUT);

  //I2C connection setup
  Wire.begin(DEVICE_ADDRESS);                 
  Wire.onReceive(receiveEvent); /* register receive event */
  Wire.onRequest(requestEvent); /* register request event */
  Serial.begin(9600);           /* start serial for debug */

   //dht setup
  dht.begin(); 

  //RFID setup
  SPI.begin(); 
  rfid.init();
}

void loop() {
  temperature();
  light_Sensor();
  motion();
  read_card();
}


/******************************************
*
* RFID - card reader code
*
*******************************************/

//reading the first card ever 
void first_read(){
  if (rfid.isCard()){
    if (rfid.readCardSerial()){
      for(int i=0 ;i<5;i++){
        recordedNum[i] = rfid.serNum[i] ;
      }
      Serial.println("registered master card");
      master_card_registered = true;
    }
  }
}


bool is_master_card(){
  for(int i=0 ;i<5;i++){                                //we check every single byte(5) to check if it's the master card o not
    if(rfid.serNum[i] != recordedNum[i]) return false;
  }
  return true;
}

void read_master_card(){
  card_id == false;
  if (rfid.isCard()){
    if (rfid.readCardSerial()){
      if (is_master_card()){
        card_id = 1;
        delay(10);
      }
      else{
        card_id == false;
      }
      
    }
        
  }
  
}

//first function called when we place a card in front of RFID
void read_card(){
  card_id=false;
  if (master_card_registered) {
    read_master_card();
  }
  else first_read();
}
/*----------------------------------------*/



/******************************************
*
* Movement sensor code
*
*******************************************/
void motion(){
  motionStatePrevious = motionStateCurrent;             // store old state
  motionStateCurrent  = digitalRead(MOTION_SENSOR_PIN); // read new state
  if (motionStatePrevious == LOW && motionStateCurrent == HIGH) { // pin state change: LOW -> HIGH
    digitalWrite(MOTION_LED, HIGH); //turn on
    led_on_off = true;
  }
  else
  if (motionStatePrevious == HIGH && motionStateCurrent == LOW) { // pin state change: HIGH -> LOW
    digitalWrite(MOTION_LED, LOW);  // turn off
    led_on_off = false;
  }
}
/*----------------------------------------*/


/******************************************
*
* DHT sensor code
*
*******************************************/
void temperature() {

  h = dht.readHumidity();           //humidity reading
  t = dht.readTemperature();        //temperature (Celsius) reading

  //Error DHT
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Error DHT!"));
    return;
  }
}
/*----------------------------------------*/


/******************************************
*
* Light sensor code
*
*******************************************/
void light_Sensor() {
  int luce = analogRead (LIGHT_SENSOR);   //This variable read from the analogical pin A0
 
  if (luce < 300){      //If the values is less than 300, the led will be turned on
    light = true;
    digitalWrite (LED_LIGHT_SENSOR, HIGH);
  }
  else{
    light = false;
    digitalWrite (LED_LIGHT_SENSOR, LOW);
  }
}
/*----------------------------------------*/
 


/******************************************
*
* Function that executes whenever data is requested from master 
* This function is used to manage the servos and led
*
*******************************************/
void receiveEvent(int howMany) {
  char c;
  while (0 < Wire.available()) {
    c = Wire.read(); /* receive byte as a character */
  }
  int i = c - '0';
  if (i == 0) {             //rool up blind (value 0,1)
    blind.write(90);
  } else if (i == 1) {
    blind.write(0);    
  } else if (i == 3) {       //roll down blind (value 2,3)
    blind.write(90);
  } else if (i == 2) {
    blind.write(180);
  } else if (i == 4) {        //open door
    porta.write(90);
  } else if (i == 5) {        //close door
    porta.write(0);
  } else if(i == 7){          //turn on light
    digitalWrite (INTERNAL_LIGHT, HIGH);  
  }else if(i == 8){           //turn on light
    digitalWrite (INTERNAL_LIGHT, LOW);
  }
  i = 0;
}
/*----------------------------------------*/



/******************************************
*
* Function that executes whenever data is requested from master 
* This function is used to send temperature and humidity value and to control the status of the leds and 
*
*******************************************/
void requestEvent() {
  
  String temp = String(t, 1);

  if (isnan(h) || isnan(t)) {   //If the values are null, Arduino will send to the NodeMcu '-1' as value for temperature and humidity
    Wire.print('-');
    Wire.print(1.00);
    Wire.print('-');
    Wire.print(1.00);
  }else if(t <  10){            //If the value is less than 10, Arduino will send a 0 before the temperature value
    Wire.print(0);
    Wire.print(t);
    if(h < 10){
      Wire.print(0);
      Wire.print(h);
    }
  }else if(h <  10){            //If the value is less than 10, Arduino will send a 0 before the humidity value
    Wire.print(0);
    Wire.print(t);
    if(t < 10){
      Wire.print(0);
      Wire.print(h);
    }
  }else{
    Wire.print(t);
    Wire.print(h);

  }
  

  //10 - sending information about the led that's related to the movement sensor (a -> off, b -> on)
  if (led_on_off == 0) {
    Wire.print("a");
  } else if (led_on_off == 1) {
    Wire.print("b");
  }

  //11 - sending information about the led that's related to the light sensor (a -> off, b -> on)
  if (light == 0) {
    Wire.print("a");
  } else if (light == 1) {
    Wire.print("b");
  }
  

  if(card_id == 1){       //if the card is read and it's correct opens the door 
    card_id == 0;
    porta.write(0);    
  }


}