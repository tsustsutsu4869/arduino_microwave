// demo of Starter Kit V2.0

#include "rgb_lcd.h"


#define ADC_REF 5//reference voltage of ADC is 5v.If the Vcc switch on the seeeduino
         //board switches to 3V3, the ADC_REF should be 3.3
#define GROVE_VCC 5//VCC of the grove interface is normally 5v
#define FULL_ANGLE 300//full value of the rotary angle is 300 degrees

rgb_lcd lcd;

const int colorR = 255;
const int colorG = 45;
const int colorB = 0;

const int rotaryPin = A0;                    // rotary angle sensor connect to A0

void setup(){
    // set up the LCD's number of columns and rows:
    lcd.begin(16, 2);
    
    lcd.setRGB(colorR, colorG, colorB);
    // Print a message to the LCD.
    lcd.setCursor(9, 0);
    lcd.print("seconds");
    
    pinMode(rotaryPin, INPUT);
}


int getDegree(){
  int sensor_value = analogRead(rotaryPin);
  float voltage;
  voltage = (float)sensor_value*ADC_REF/1023;
  float degrees = (voltage*FULL_ANGLE)/GROVE_VCC;
  return degrees;
}


void loop(){
    lcd.setCursor(5, 0);
    int degrees;
    degrees = getDegree();  
    lcd.print(degrees);                      // pirnt the value on the serial monitor screen
    delay(100);                                  // wait 1000ms before printing next value
}
