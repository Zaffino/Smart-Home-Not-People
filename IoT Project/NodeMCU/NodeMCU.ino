#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Wire.h>

#include "NodeMCU.h"


// You should get Auth Token in the Blynk App.
char auth[] = BLYNK_AUTH_TOKEN;

// WiFi credentials
const char* ssid = "FRITZ!Box 7490";
const char* pass = "09843215662135353024";


WidgetLED led(V4);                   //Blynk widget that corresponds to a led
WidgetLED lls(V5);                   //Blynk widget that corresponds to a led
WidgetLCD lcd(V6);                   // Blynk widget that corresponds to a lcd display
WidgetLED internal_light(V7);        //Blynk widget that corresponds to a led



//BLIND - tapparella
/***************************************
*
* BLYNK_WRITE(V2) - Function that tells Arduino what to do: open or close the door
*
* V2: virtual pin from blynk
*
****************************************/
BLYNK_WRITE(V2) {
  if (param.asInt() == 1) {             //roll up blind  (value 2,3)
    Wire.beginTransmission(DEVICE_ADDRESS);          // begin with device address 8 
    Wire.write('0');
    Serial.println("v2 0");
    Wire.endTransmission();
  } else if (param.asInt() == 0){       
    Serial.println("v2 - off");
    Wire.beginTransmission(DEVICE_ADDRESS); 
    Wire.write('1');    
    Serial.println("v2 1");       
    Wire.endTransmission();
  }else if (param.asInt() == 2){      //roll down blind (value 2,3)
    Serial.println("v2 2");
    Wire.beginTransmission(DEVICE_ADDRESS); 
    Wire.write('2');          
    Wire.endTransmission();
  }else if (param.asInt() == 3){
    Serial.println("v2 3");
    Wire.beginTransmission(DEVICE_ADDRESS); 
    Wire.write('3');         
    Wire.endTransmission();
  }
}


//DOOR
/***************************************
*
* BLYNK_WRITE(V3) - Function that tells Arduino what to do: open or close the door
*
* V3: virtual pin from blynk
*
****************************************/
BLYNK_WRITE(V3) {
  if (param.asInt() == 4) {         //open door
    Wire.beginTransmission(DEVICE_ADDRESS);  
    Wire.write('4');
    Wire.endTransmission();
    delay(500);
  } else if (param.asInt() == 5){     //close door
    Serial.println("v3 - off");
    Wire.beginTransmission(DEVICE_ADDRESS); 
    Wire.write('5');           
    Wire.endTransmission();
    delay(500);
  }
}

/***************************************
*
* BLYNK_WRITE(V7) - Function that tells Arduino what to do: turn on or off a led/light
*
* V7: virtual pin from blynk
*
****************************************/
BLYNK_WRITE(V7){
  if(param.asInt() == 0){                         //turn on
    Wire.beginTransmission(DEVICE_ADDRESS); 
    Wire.write('7');           
    Wire.endTransmission();
  }else if(param.asInt() == 1){                   //turn off
    Wire.beginTransmission(DEVICE_ADDRESS); 
    Wire.write('8');           
    Wire.endTransmission();
  }
}


void setup() {
  Serial.begin(115200);
  //I2C connection between NodeMCU and Arduino                
  Wire.begin(D1, D2);
  //Blynk setup
  Blynk.begin(auth, ssid, pass);
}

void get_temperature(double t, double h) {
  Blynk.virtualWrite(V0, t);
  Blynk.virtualWrite(V1, h);
}

void loop() { 
  Blynk.run();                          //activate Blynk connection

  Wire.beginTransmission(DEVICE_ADDRESS);            //activate I2C connection
  Wire.endTransmission();               // stop transmitting
  Wire.requestFrom(DEVICE_ADDRESS, 12);              // request & read data of size 13 from slave 
  
  int i = 0;    //index
  int n = 0;
  double temp = 0.0, umidita = 0.0;

  while (Wire.available()) {    //while the I2C connection is available it will read a list of characters received from Arduino 
    char c = Wire.read();       //reads character from Arduino
    int v = c - '0';
  
    
    if (i < 5) {                //Reads temperature value and format it to be 4 numbers (eg 10.15)
      if (i != 2) {
        if (i == 0) {
          temp += v * 10;
        } else if (i == 1) {
          temp += v;
        } else if (i == 3) {
          temp += v * 0.1;
        } else if (i == 4) {
          temp += v * 0.01;
        }
      }
    } else if (5 <= i < 10) {       //check if there are any errors and write on the LCD display
      if(temp < 0){
        lcd.clear();
        lcd.print(4,0,"Error DHT");
        temp = -10.00;
        lcd.clear();       
      }
      if (i != 7) {               //Reads humidity value and format it to be 4 numbers (e.g. 10.15)
        if (i == 5) {
          umidita += v * 10;
        } else if (i == 6) {
          umidita += v;
        } else if (i == 8) {
          umidita += v * 0.1;
        } else if (i == 9) {
          umidita += v * 0.01;
        }
      }
    }
    if(i == 10){                    //check the led related to the movement sensor, check if it's on or off and send a command to blynk
      if(c == 'a'){
        led.off();
      }else if(c == 'b'){
        led.on();
      }
    }
    if(i == 11){                     //check the led related to the light sensor, check if it's on or off and send a command to blynk
      
      if(c == 'a'){
        lls.off();
      }else if(c == 'b'){
        lls.on();
      }
    }

    i++;
  }
  get_temperature(temp, umidita);           //get temperature and humidity value from Arduino

}
