#ifndef __GAEUL_H__
#define __GAEUL_H__

#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <Servo.h> 

#define servoPin 12 //servo Pin
#define buzzerPin 13//buzz


void settingGaeul();
//setting




void buzz(int value);
//turn on when true, turn off when false
void servofunc(int angle);
//servo motor contol as angle
float getloadcell();


#endif





