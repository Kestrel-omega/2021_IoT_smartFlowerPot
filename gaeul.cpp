#include "gaeul.h"


#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <Servo.h> 


#define servoPin 9 //servo Pin
#define buzzerPin 12//buzz

Servo servo; 


void settingGaeul()
{
  pinMode(buzzerPin,OUTPUT); //buzzer
  Serial.begin(115200);
  
  //scale.set_scale(calibration_factor); 
  //scale.tare();  //set init value as 0
  //servo.attach(servoPin); 
}




void buzz(int value)
{
  if (value==0)
  {
    digitalWrite(buzzerPin,0);
  }
  else
  {
    digitalWrite(buzzerPin,1);
    delay(1000);
    digitalWrite(buzzerPin,0);
    delay(1000);
  }
}

////////////////////////////////////////////////////////////////////////////////////

void servofunc(int angle)
{
  if(angle > 180) 
  { 
    angle = 180;
    servo.write(angle); 
  } 
  if(angle < 0) 
  { 
    angle = 0 ;
    servo.write(angle);   
  }
}


