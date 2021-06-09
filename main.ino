#include <gaeul.h>
#include <jiho.h>
#include "Arduino.h"
//ESP8266WebServer myHttpServer(80);

void setup()
{
  settingGaeul();
  wifiset();
  webhtml();
  sensorSetup();
  ledOn();
}
int angle=0;

void loop()
{
  //myHttpServer.handleClient();
  int temp;
  int humid;
  float temp1;
  float humid1;
  getHumid();
  readDHT11(&temp, &humid);
  getWeather(&temp1, &humid1);
  servofunc(angle);
  displayLCD(temp, humid);
}
