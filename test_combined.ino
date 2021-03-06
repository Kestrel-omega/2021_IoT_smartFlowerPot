// 모든 헤더를 통합했습니다.

// Include Headers
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <Servo.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <Adafruit_NeoPixel.h>
#include "AnotherIFTTTWebhook.h"
#include "HX711.h"

// Define Constants
#define key "dp2pmH8w5fDxygupiFknDhX92YotQgbicf5jvKp9CIs"
#define ID "irslab"
#define PW "" // 

#define LED 0     //D3
#define DHT11 2   //D4
#define L_DATA 4  //D2
#define L_CLK 5   //D1
#define SERVO 12  //D6
#define BUZZ 13   //D7
#define SOIL A0   //A0

// Declare Object
WiFiClient TCPClient;
PubSubClient MQTTClient;
HTTPClient myHTTPClient;
ESP8266WebServer HttpServer(80);
DynamicJsonDocument doc(2048);
Servo servo;
HX711 scale;

// Initialize Variables
float calibration_factor = -100000;
int conditionHumid = 0;
int conditionWeight = 0;
int angle = 0;
int DHT_temp = 0;
int DHT_humid = 0;
float soil_humid = 0;
float atmos_temp = 0;
float atmos_humid = 0;
float weight = 0;

// Declare Functions
void ledOn();
void ledOff();
float getHumid();
int readDHT11(int *temp, int* humid);
void getWeather();
void buzz(int value);
void servofunc(int angle);
void thingspeakpub(int num, int value);
//webserverfn//
void webhtml();
void fnRoot();
void fnStatus();
void fnNotFound();
void fnOn();
void fnOff();
void fnInput();

void setup()
{
  Serial.begin(115200);
  delay(500);

  pinMode(LED, OUTPUT);
  pinMode(DHT11, INPUT);
  pinMode(BUZZ, OUTPUT);
  Serial.println("pinMode Setting Done!");

  
    scale.begin(L_DATA, L_CLK);
    scale.set_scale();
    scale.tare();
    long zero_factor = scale.read_average();
  
  servo.attach(SERVO);

  Serial.println("Setup Done!");

  Serial.println("wifi setting start");
  WiFi.begin(ID, PW);
  while (1)
  {
    if (WiFi.status() == WL_CONNECTED)
      break;
    Serial.println(".");
    delay(500);
  }
  Serial.println(WiFi.localIP());

  webhtml();
  ledOn();

  MQTTClient.setClient(TCPClient);
  MQTTClient.setServer("mqtt.thingspeak.com", 1883);
  MQTTClient.connect("smartflowerpot");
}

unsigned long long prevMillis = 0;
int count = 0;

void loop()
{
  if (millis() - prevMillis > 1000)
  {
    prevMillis = millis();
    millis();
    HttpServer.handleClient();
    delay(500);
    soil_humid = getHumid();
    delay(500);
    readDHT11(&DHT_temp, &DHT_humid);
    delay(500);
    getWeather();
    
    Serial.printf("readDHT - Temp : %d, Humid %d\r\n", DHT_temp, DHT_humid);
    Serial.printf("Atmosphere - Temp : %f, Humid %f\r\n", atmos_temp, atmos_humid);
    Serial.printf("Soil - Humid : %f\r\n", soil_humid);
    
    HttpServer.handleClient();
    if (soil_humid < conditionHumid) //--------------------흙의 습도가 설정한 습도값보다 낮으면
    {
      Serial.println("humid < conditionHumid");
      Serial.println("습도 낮음");//체크
      buzz(true);
      servofunc(30);
      delay(2000);
      servofunc(0);
    }
    HttpServer.handleClient();
     getloadcell();
    if (weight < conditionWeight) //--------------------설정한 무게보다 낮으면
    {
      //ifttt로 물통 채우라는 알람보내기 ( 알람 너무 많이 옴 이거 해결하기)
      Serial.println("weigh t< conditionWeight ifttt 알람 보냄");
      char wei[20];
      snprintf(wei, sizeof(wei), "%d", weight);
      send_webhook("smartFlowerPot", key, wei, "", "");
    }
    count++;
    if(count == 10)
    {
      thingspeakpub(1, DHT_temp);
      Serial.println("thingspeak publish");
    }
      
    if(count == 20)
      {
        thingspeakpub(2, soil_humid);
        Serial.println("thingspeak publish");
        count = 0;
      }
  }
  MQTTClient.loop();
}

void ledOn()
{
  Adafruit_NeoPixel strip = Adafruit_NeoPixel(16, LED, NEO_GRB + NEO_KHZ800);

  strip.begin();
  strip.show();

  for (int i = 0; i < strip.numPixels(); i++)
    strip.setPixelColor(i, strip.Color(255, 0, 255));
  strip.show();
}

void ledOff()
{
  Adafruit_NeoPixel strip = Adafruit_NeoPixel(16, LED, NEO_GRB + NEO_KHZ800);

  strip.begin();
  strip.show();

  for (int i = 0; i < strip.numPixels(); i++)
    strip.setPixelColor(i, strip.Color(0, 0, 0));
  strip.show();
}

float getHumid()
{
  float humid = (100 - ((float)analogRead(SOIL) / 1024 * 100));
  return humid;
}

int readDHT11(int *readTemp, int* readHumid)
{

  int dt[82] = {0,};
  //Phase 1
  digitalWrite(DHT11, 1);
  pinMode(DHT11, OUTPUT);
  delay(1);
  digitalWrite(DHT11, 0);
  delay(20);
  pinMode(DHT11, INPUT_PULLUP);
  /*
    while(1)
      if(digitalRead(DHT11)==1) break;
    while(1)
      if(digitalRead(DHT11)==0) break;
  */
  //Phase 2, 3
  int cnt = 0;
  for (cnt = 0; cnt < 41; cnt++)
  {
    dt[cnt * 2] = micros();
    while (1)
      if (digitalRead(DHT11) == 1) break;
    dt[cnt * 2] = micros() - dt[cnt * 2];

    dt[cnt * 2 + 1] = micros();
    while (1)
      if (digitalRead(DHT11) == 0) break;
    dt[cnt * 2 + 1] = micros() - dt[cnt * 2 + 1];
  }

  //Phase 4
  *readHumid = 0;
  *readTemp = 0;
  for (cnt = 2; cnt < 10; cnt++) //Humid
  {
    *readHumid = *readHumid << 1;
    if (dt[cnt * 2 + 1] > 49)
      *readHumid = *readHumid + 1;
    else
      *readHumid = *readHumid + 0;

  }
  for (cnt = 17; cnt < 25; cnt++) //Temp
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

void getWeather()
{
  myHTTPClient.begin("http://api.openweathermap.org/data/2.5/weather?q=yongin&appid=710a19c0a43aa3674ef3fa64c6a824ff");
  if(myHTTPClient.GET()==HTTP_CODE_OK)
  {
    //Serial.printf("HTTP Start........................\r\n");
    delay(1000);
    String data=myHTTPClient.getString();
    //Serial.printf("%s",data.c_str());
    deserializeJson(doc,data);
    atmos_temp = (float)(doc["main"]["temp"]) - 273.0; // 기온
    atmos_humid = (float)doc["main"]["humidity"]; // 습도
  }
  else Serial.printf("Server something wrong!\n\r");
  //Serial.printf("\r\n\r\nThis is END....\r\n\r\n");
  myHTTPClient.end();
}

void buzz(int value)
{
  if (value == 0)
  {
    digitalWrite(BUZZ, 0);
  }
  else
  {
    digitalWrite(BUZZ, 1);
    delay(1000);
    digitalWrite(BUZZ, 0);
    delay(1000);
  }
}

void servofunc(int angle)
{
  if (angle > 180)
  {
    angle = 180;
    servo.write(angle);
  }
  if (angle < 0)
  {
    angle = 0 ;
    servo.write(angle);
  }
}
float getloadcell()
{
  scale.set_scale(calibration_factor);
  return scale.get_units();
}

void webhtml()
{
  HttpServer.on("/", fnRoot );
  HttpServer.on("/status", fnStatus );
  HttpServer.on("/on", fnOn );
  HttpServer.on("/off", fnOff );
  HttpServer.on("/input", fnInput );
  HttpServer.onNotFound(fnNotFound);
  HttpServer.begin();
}

void fnRoot(void)//callback function
{
  char buff[2000];//html buff
  strcpy(buff, "<html>\r\n2021_IoT_smartFlowerPot<br>\r\n<a href=/status>Check Status</a><br>\r\n<a href=/on>LED On</a><br>\r\n<a href=/off>LED Off</a><br>\r\n<form method=\"get\" action=\"input\">Condition maintain humid <input type =\"text\" name=\"Humid\"><input type=\"submit\" ></form>\r\n<form method=\"get\" action=\"input\">Condition maintain weight <input type =\"text\" name=\"Weight\"><input type=\"submit\" ></form>\r\n</html>");
  HttpServer.send(200, "text/html", buff); //text type html send
}
void fnStatus(void)
{
  char buff[2000];//html buff
  sprintf(buff, "<html>\r\n temp condition : %d<br>\r\n\r\n weight condition : %d<br>\r\n</html>", conditionHumid, conditionWeight);
  HttpServer.send(200, "text/html", buff);
}
void fnNotFound()
{
  HttpServer.send(404, "text/plain", "Wrong");
}
void fnOn()
{
  ledOn();
  HttpServer.send(200, "text/html", "led on");
}
void fnOff()
{
  ledOff();
  HttpServer.send(200, "text/html", "led off");
}
void fnInput()
{
  char co_Humid[400];
  char co_Weight[400];

  if (HttpServer.hasArg("Humid"))
  {
    strcpy(co_Humid, "<meta charset=utf-8>");
    strcat(co_Humid, "습도 조건 변경 (0~100) : ");
    strcat(co_Humid, HttpServer.arg("Humid").c_str());
    conditionHumid = atoi(HttpServer.arg("Humid").c_str());

    if (conditionHumid >= 100) conditionHumid = 100;
    else if (conditionHumid < 0) conditionHumid = 0;

    HttpServer.send(200, "text/html", co_Humid);

  }
  if (HttpServer.hasArg("Weight"))
  {
    strcpy(co_Weight, "<meta charset=utf-8>");
    strcat(co_Weight, "무게 조건 변경 (0~100) : ");
    strcat(co_Weight, HttpServer.arg("Weight").c_str());
    conditionWeight = atoi(HttpServer.arg("Weight").c_str());

    if (conditionWeight >= 100) conditionWeight = 100;
    else if (conditionWeight < 0) conditionWeight = 0;

    HttpServer.send(200, "text/html", co_Weight);

  }
  else HttpServer.send(200, "text/plain", "Somthing wrong");
}
void thingspeakpub(int num, int value)
{
  char buff[5], pub[1000];
  snprintf(buff, sizeof(buff), "%d", value);
  //snprintf(pub, sizeof(pub), "channels/1393641/publish/fields/field%d/0NX20M0E910B87BB", num);
  snprintf(pub, sizeof(pub), "channels/1413355/publish/fields/field%d/IOGTH6XE5SQF54AH", num);
  MQTTClient.publish(pub, buff);
}
