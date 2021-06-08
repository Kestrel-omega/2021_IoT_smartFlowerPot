#include "Arduino.h"
#include "gaeul.h"
#include "jiho.h"

void setup()
{
  settingGaeul();
  wifiset();
  webthml();
  sensorSetup();
  ledOn();
}

int angle;

void loop()
{
  float temp;
  float humid;
  
  getHumid();
  readDHT11();
  getWeather(temp, humid);
  servofunc(angle);
  getloadcell();
  displayLCD(temp, humid);
}
