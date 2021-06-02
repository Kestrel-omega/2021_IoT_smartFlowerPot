// jiho.h

#ifndef __JIHO_H__
#define __JIHO_H__

#include <ESP8266WiFi.h>
#include <Wire.h>
#include <LiquidCrystal.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <Adafruit_NeoPixel.h>

#define RS 0 //D03
#define RW 2 //D04
#define DB4 14 //D05
#define DB5 15 //D08
#define DB6 3 //RX
#define DB7 1 //TX
#define LED 10 //SD3
#define DHT11PIN 16 //D01

HTTPClient MyClient;
DynamicJsonDocument doc(2048);

// ESP8266 and sensors initialize
void sensorSetup();
// LED On
void ledOn();
// get humidity_of_soil data from SZH-SSBH-074
float getHumid();
// using DHT11 sensor with polling method
int readDHT11(int *readTemp, int* readHumid);
// export data to LCD
void displayLCD(float temp, float humid);
// get current weather data from Openweather API
void getWeather(float *temp, float *humid);

#endif
