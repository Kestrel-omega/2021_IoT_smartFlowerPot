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
#define buzzerPin 12//buzz


HX711 scale(DOUT, CLK);
Servo servo; 
ESP8266WebServer myHttpServer(80);//have to call "myHttpServer.handleClient();" as many as possible in loop()


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

void buzz(int value)//
//turn on when true, turn off when false
void servofunc(angle);
//servo motor contol as angle
float getloadcell();
//return readed weight


#endif





