#include "jiho.h"

#include <Adafruit_NeoPixel.h>
#include <Arduino.h>
HTTPClient myClient;
LiquidCrystal lcd(RS, RW, DB4, DB5, DB6, DB7);
// ESP8266 and sensors initialize
DynamicJsonDocument doc(2048);
void sensorSetup()
{
  // LED Setting
  pinMode(LED, OUTPUT);
  // SZH-SSBH-074 Setting
  pinMode(DHT11PIN, INPUT);
  // LCD Setting
  
  lcd.begin(16,2);
  Serial.begin(115200);
  delay(100);
  WiFi.begin("KT_Giga_Wifi", "");
  while(1)
  {
    if(WiFi.status() == WL_CONNECTED) break;
    else delay(100);
  }
  Serial.printf("WiFi Connected!\r\n");
  
}

// LED brightness
void ledOn()
{
  int PIN = 5;
  Adafruit_NeoPixel strip = Adafruit_NeoPixel(16, PIN, NEO_GRB + NEO_KHZ800);
  
  strip.begin();
  strip.show(); 

  for(int i=0; i<strip.numPixels(); i++) 
      strip.setPixelColor(i, strip.Color(255, 0, 255));
  strip.show();
}

// get humidity_of_soil data from SZH-SSBH-074
float getHumid()
{
  float humid = (1023-analogRead(sensorPin));
  return humid;
}

// export data to LCD
void displayLCD(float temp, float humid)
{
  char str1[16] = {0,};
  char str2[16] = {0,};
  lcd.setCursor(0,0);
  sprintf(str1,"Temp : %f °C", temp);
  lcd.setCursor(0,1);
  sprintf(str2,"Humid : %f %%", humid);
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

// get current weather data from Openweather API
void getWeather(float *temp, float *humid)
{
  myClient.begin("http://api.openweathermap.org/data/2.5/weather?q=seongnam&appid=95a4855b2d6b5d5228cc00bf3ed6c3e5");
  int getResult = myClient.GET();
  if(getResult == HTTP_CODE_OK) // 200
  {
    String receivedData = myClient.getString();
    Serial.printf("%s\r\n\r\nEND\r\n\r\n", receivedData.c_str());
    deserializeJson(doc, receivedData); // 해석 완료
    const char* city = doc["name"]; // 도시 이름
    const char* weather = doc["weather"][0]["main"]; // 대략적 상태
    float temp = (float)(doc["main"]["temp"]) - 273.0; // 기온
    float humid = (float)doc["main"]["humidity"]; // 습도
    delay(5000);
}
}
