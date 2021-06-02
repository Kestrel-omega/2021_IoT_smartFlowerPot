#ifndef __GAEUL_H__
#define __GAEUL_H__

#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <Servo.h> 
#include <HX711.h>

#define calibration_factor -7050.0 
#define DOUT  3 //data Pin
#define CLK  2 // clk Pin
#define servoPin 9 //servo Pin

HX711 scale(DOUT, CLK);
Servo servo; 
ESP8266WebServer myHttpServer(80);

void settingGaeul();
//setting
void wifiset();
//setting wifi
void servofunc(angle);
//servo motor contol as angle
float getloadcell();
//return readed weight
void webhtml();
//html
void buzz(int value)//
//turn on when true, turn off when false


#endif





