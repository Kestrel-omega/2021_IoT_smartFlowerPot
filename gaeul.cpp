#include "gaeul.h"


#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <Servo.h> 
#include <HX711.h>


#define calibration_factor -7050.0 
#define DOUT  3 //data Pin
#define CLK  2 // clk Pin
#define servoPin 9 //servo Pin
#define buzzerPin 12//buzz

HX711 scale(DOUT, CLK);
Servo servo; 
ESP8266WebServer myHttpServer(80);

void settingGaeul()
{
  pinMode(buzzerPin,OUTPUT); //buzzer
  servo.attach(servoPin);    //servomotor
  Serial.begin(115200);      //serial
  scale.set_scale(calibration_factor); 
  scale.tare();  //set init value as 0
  wifiset();
  webhtml();
}
void wifiset()
{
  WiFi.begin("iPhone","12345678");
  while(1)
  {
    if(WiFi.status()==WL_CONNECTED)
      break;
      delay(500);
  }
  Serial.printf("connected!\n");
  Serial.printf("please contact IP addr...");
  Serial.println(WiFi.localIP());
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
