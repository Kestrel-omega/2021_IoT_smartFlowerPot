#ifndef __JIHO_H__
#define __JIHO_H__

#include <ESP8266WiFi.h>
#include <Wire.h>
#include <LiquidCrystal.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

#define DHT11PIN 14 //D4

HTTPClient MyClient;
DynamicJsonDocument doc(2048);

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
// LED On
void ledOn();
// get temperature data from MPU6050
float getTemp();
// get humidity_of_soil data from SZH-SSBH-074
float getHumid();
// using DHT11 sensor with polling method
int readDHT11(int *readTemp, int* readHumid);
// export data to LCD
void displayLCD(float temp, float humid);
// get current weather data from Openweather API
void getWeather(float *temp, float *humid);

#endif
