#ifndef __GAEUL_H__
#define __GAEUL_H__

#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <Servo.h> 

#define servoPin 9 //servo Pin
#define buzzerPin 12//buzz



void settingGaeul();
//setting
void wifiset();
//setting wifi
void webhtml();
//html

void fnRoot(void);
void fnStatus(void);
void fnNotFound();
void fnOn();
void fnOff();
void fnInput();

void buzz(int value);
//turn on when true, turn off when false
void servofunc(int angle);
//servo motor contol as angle



#endif





