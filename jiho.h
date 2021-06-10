// jiho.h

#ifndef __JIHO_H__
#define __JIHO_H__

#include <ESP8266WiFi.h>
#include <Wire.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <Adafruit_NeoPixel.h>

#define LED 0 //D3
#define DHT11PIN 2 //D4
#define sensorPin 99////////////////////수정필요
//HTTPClient MyClient;


// ESP8266 and sensors initialize
void sensorSetup();
// LED On
void ledOn();
void ledOff();
// get humidity_of_soil data from SZH-SSBH-074
int getHumid();
// using DHT11 sensor with polling method
int readDHT11(int *readTemp, int* readHumid);
// export data to LCD
void displayLCD(float temp, float humid);

#endif
