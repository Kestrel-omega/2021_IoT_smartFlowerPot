#include "gaeul.h"


#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <Servo.h> 
#include "HX711.h" 
 
#define LOADCELL_DOUT_PIN 4
#define LOADCELL_SCK_PIN 5
#define servoPin 9 //servo Pin
#define buzzerPin 12//buzz
float calibration_factor = -100000;

Servo servo; 
HX711 scale;

void settingGaeul()
{
  pinMode(buzzerPin,OUTPUT); //buzzer
  Serial.begin(115200);
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale();
  scale.tare();
  long zero_factor = scale.read_average(); 
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
float getloadcell()
{
	scale.set_scale(calibration_factor);
	return scale.get_units();
}


