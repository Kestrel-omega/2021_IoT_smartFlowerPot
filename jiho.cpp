#include "jiho.h"

#include <Adafruit_NeoPixel.h>
#include <Arduino.h>
const int soilSensorPin = A0;
// ESP8266 and sensors initialize
void sensorSetup()
{
  // LED Setting
  pinMode(LED, OUTPUT);
  // SZH-SSBH-074 Setting
  pinMode(DHT11PIN, INPUT);
  Serial.begin(115200);
 
}
// LED brightness
void ledOn()
{
  int PIN = LED;
  Adafruit_NeoPixel strip = Adafruit_NeoPixel(16, PIN, NEO_GRB + NEO_KHZ800);
  
  strip.begin();
  strip.show(); 

  for(int i=0; i<strip.numPixels(); i++) 
      strip.setPixelColor(i, strip.Color(255, 0, 255));
  strip.show();
}
void ledOff()
{
  int PIN = LED;
  Adafruit_NeoPixel strip = Adafruit_NeoPixel(16, PIN, NEO_GRB + NEO_KHZ800);
  
  strip.begin();
  strip.show(); 

  for(int i=0; i<strip.numPixels(); i++) 
      strip.setPixelColor(i, strip.Color(0, 0, 0));
  strip.show();
}

// get humidity_of_soil data from SZH-SSBH-074
int getHumid()
{
  int humid = (100-((float)analogRead(soilSensorPin)/1024*100));
  return humid;
}

int readDHT11(int *readTemp, int* readHumid)
{

  int dt[82] = {0,};
  //Phase 1
  digitalWrite(DHT11PIN, 1);
  pinMode(DHT11PIN, OUTPUT);
  delay(1);
  digitalWrite(DHT11PIN, 0);
  delay(20);
  pinMode(DHT11PIN, INPUT_PULLUP);
  /*
    while(1)
      if(digitalRead(DHT11PIN)==1) break;
    while(1)
      if(digitalRead(DHT11PIN)==0) break;
  */
  //Phase 2, 3
  int cnt = 0;
  for (cnt = 0; cnt < 41; cnt++)
  {
    dt[cnt * 2] = micros();
    while (1)
      if (digitalRead(DHT11PIN) == 1) break;
    dt[cnt * 2] = micros() - dt[cnt * 2];

    dt[cnt * 2 + 1] = micros();
    while (1)
      if (digitalRead(DHT11PIN) == 0) break;
    dt[cnt * 2 + 1] = micros() - dt[cnt * 2 + 1];
  }

  //Phase 4
  *readHumid = 0;
  *readTemp = 0;
  for (cnt = 2; cnt < 11; cnt++) //Humid
  {
    *readHumid = *readHumid << 1;
    if (dt[cnt * 2 + 1] > 49)
      *readHumid = *readHumid + 1;
    else
      *readHumid = *readHumid + 0;

  }
  for (cnt = 18; cnt < 26; cnt++) //Temp
  {
    *readTemp = *readTemp << 1;
    if (dt[cnt * 2 + 1] > 49)
      *readTemp = *readTemp + 1;
    else
      *readTemp = *readTemp + 0;
  }
  //for(cnt=0;cnt<41;cnt++)
  //  Serial.printf("cnt:%d, dt[%d, %d]\r\n",cnt,dt[cnt*2],dt[cnt*2+1]);
  return 1;
}
