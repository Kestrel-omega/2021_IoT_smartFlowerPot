#ifndef __GAEUL_H__
#define __GAEUL_H__

#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Servo.h> 
int servoPin = 9;
int loadcellPin = 11;

void servofunc(angle);
//servo motor contol as angle
void getloadcell();
//read weight
void webhtml();
//html


#endif
