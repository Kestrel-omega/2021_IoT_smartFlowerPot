#include <ESP8266WiFi.h>
#include <Wire.h>
#include <LiquidCrystal.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <Adafruit_NeoPixel.h>

#define DHT11PIN 13 // D7

HTTPClient myClient;
DynamicJsonDocument doc(2048);
LiquidCrystal lcd(12, 14, 5, 4, 0, 2);

int PIN = 15; // D8
int sensorPin = 9; // SD2

int readTemp;
int readHumid;

void setup()
{
  // LCD Setting
  lcd.begin(16,2);
  lcd.clear();
  Serial.begin(115200);
  delay(100);
  WiFi.begin("KT_Giga_Wifi", "");
  while(1)
  {
    if(WiFi.status() == WL_CONNECTED) break;
    else delay(100);
  }
  Serial.printf("WiFi Connected!\r\n");

  Adafruit_NeoPixel strip = Adafruit_NeoPixel(16, PIN, NEO_GRB + NEO_KHZ800);
  
  strip.begin();
  strip.show(); 

  for(int i=0; i<strip.numPixels(); i++) 
      strip.setPixelColor(i, strip.Color(255, 0, 255));
  strip.show();
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
    }
  else Serial.printf("Error, code: %d\r\n", getResult);
  delay(5000);
}

void loop()
{
  char buffer[4] = {0};
  int temp, humid;
  readDHT11(&temp, &humid);
  char ttemp[80];
  char thumid[80];
  snprintf(ttemp, sizeof(ttemp), "%d", temp);
  snprintf(thumid, sizeof(thumid), "%d", humid);
  lcd.setCursor(0,0);
  lcd.print(temp);
  lcd.setCursor(0,1);
  lcd.print(humid);
  delay(1000);
}
