#include "gaeul.h"

#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Servo.h> 
#include <HX711.h>

void settingGaeul()
{
  servo.attach(servoPin); 
  Serial.begin(115200);  
  scale.set_scale(calibration_factor); 
  scale.tare();  //set init value as 0
}

void servofunc(angle)
{
  for(angle > 180) 
  { 
    angle = 180;
    servo.write(angle); 
  } 
  for(angle < 0) 
  { 
    angle = 0 ;
    servo.write(angle);   }
  }
}
float getloadcell()
{
  float weight = scale.get_units();
  Serial.println(weight); //scale.get_units() returns a float
  return weight;
}
void webhtml()
{
  
}
