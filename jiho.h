#ifndef __JIHO_H__
#define __JIHO_H__

#include <ESP8266WiFi.h>
#include <Wire.h>
#include <LiquidCrystal.h>

int ledPin = 13;
int sensorPin = 6;
int MPU_ADDR = 0x68;

int RS = 12;
int E = 11;
int D4 = 5;
int D5 = 4;
int D6 = 3;
int D7 = 2;

// ESP8266 and sensors initialize
void sensorSetup();
// LED brightness (%)
void LEDControl(float state);
// get temperature data from MPU6050
float getTemp();
// get humidity_of_soil data from SZH-SSBH-074
float getHumid();
// export data to LCD
void displayLCD(float temp, float humid);
// get current weather data from Openweather API
void getWeather();

#endif
