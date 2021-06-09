#include <gaeul.h>
#include <jiho.h>
#include "Arduino.h"
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <PubSubClient.h>
#include "AnotherIFTTTWebhook.h"
#define key "dp2pmH8w5fDxygupiFknDhX92YotQgbicf5jvKp9CIs"

WiFiClient TCPClient;
PubSubClient MQTTClient;

ESP8266WebServer myHttpServer(80);
int conditionHumid = 50;
int conditionWeight = 50;
void wifiset();
void setup()
{
  settingGaeul();
  sensorSetup();
  wifiset();
  webhtml();
  ledOn();//-------------1.LED ON
  MQTTClient.setClient(TCPClient);
  MQTTClient.setServer("mqtt.thingspeak.com",1883);
  MQTTClient.connect("smartflowerpot");
}
int angle=0;

void loop()
{
  myHttpServer.handleClient();
  int temp=1;
  int humid=2;
  float temp1;
  float humid1;
  int weight=70;
  //getHumid();//-----------2. humid and temp 받아오기
  //readDHT11(&temp, &humid);
  //getWeather(&temp1, &humid1);
  Serial.println("온/습도 받아옴");
  MQTTClient.loop();
  thingspeakpub(1,temp);
  MQTTClient.loop();
  thingspeakpub(2,humid);// 앞에는 field 번호 뒤에는 값 thingspeak가 15초가 최소간격이라 타이밍 문제 있음 
  Serial.println("thingspeak publish");
  //displayLCD(temp, humid);//------------lcd 출력
  Serial.println("lcd 출력완료");
  myHttpServer.handleClient();
  if(humid<conditionHumid)//--------------------설정한 습도값보다 낮으면
  {
    Serial.println("humid<conditionHumid");
    Serial.println("습도 낮음");//체크
    buzz(true);
    servofunc(30);
    delay(2000);
    servofunc(0);
  }
  myHttpServer.handleClient();
  getloadcell();//아직 안짬 :  무게 받아오기
  if(weight<conditionWeight)//--------------------설정한 무게보다 낮으면
  {
    //ifttt로 물통 채우라는 알람보내기 ( 알람 너무 많이 옴 이거 해결하기)
    Serial.println("weight<conditionWeight ifttt알람보냄");
    char wei[20];
    snprintf(wei,sizeof(wei),"%d",weight);
    send_webhook("smartFlowerPot",key,wei,"","");
  }

}

void wifiset()
{
  Serial.println("wifi setting start");
  WiFi.begin("iPhone","12345678");
  while(1)
  {
    if(WiFi.status()==WL_CONNECTED)
      break;
      Serial.println(".");
      delay(500);
  }
  
  Serial.println(WiFi.localIP());
}

void webhtml()
{
  myHttpServer.on("/",fnRoot );
  myHttpServer.on("/status",fnStatus );
  myHttpServer.on("/on",fnOn );
  myHttpServer.on("/off",fnOff );
  myHttpServer.on("/input",fnInput );
  myHttpServer.onNotFound(fnNotFound);
  myHttpServer.begin();
}


/////////////////////////////////////webserverFn//////////////////////////////////////
void fnRoot(void)//callback function
{
  char buff[2000];//html buff
  strcpy(buff,"<html>\r\n2021_IoT_smartFlowerPot<br>\r\n<a href=/status>Check Status</a><br>\r\n<a href=/on>LED On</a><br>\r\n<a href=/off>LED Off</a><br>\r\n<form method=\"get\" action=\"input\">Condition maintain humid <input type =\"text\" name=\"Humid\"><input type=\"submit\" ></form>\r\n<form method=\"get\" action=\"input\">Condition maintain weight <input type =\"text\" name=\"Weight\"><input type=\"submit\" ></form>\r\n</html>");
  myHttpServer.send(200,"text/html",buff);//text type html send
}
void fnStatus(void)
{
  char buff[2000];//html buff
  sprintf(buff,"<html>\r\n temp condition : %d<br>\r\n\r\n weight condition : %d<br>\r\n</html>",conditionHumid,conditionWeight);
  myHttpServer.send(200,"text/html",buff);
}
void fnNotFound()
{
  myHttpServer.send(404,"text/plain","Wrong");
}
void fnOn()
{
  ledOn();
  myHttpServer.send(200,"text/html","led on");
}
void fnOff()
{
  ledOff();
  myHttpServer.send(200,"text/html","led off");
}
void fnInput()
{
  char co_Humid[400];
  char co_Weight[400];

  if (myHttpServer.hasArg("Humid"))
  {
    strcpy(co_Humid,"<meta charset=utf-8>");
    strcat(co_Humid,"습도 조건 변경 (0~100) : ");
    strcat(co_Humid, myHttpServer.arg("Humid").c_str());
    conditionHumid=atoi(myHttpServer.arg("Humid").c_str());

    if(conditionHumid>=100) conditionHumid=100;
    else if(conditionHumid<0) conditionHumid=0;

    myHttpServer.send(200,"text/html",co_Humid);
    
  }
  if (myHttpServer.hasArg("Weight"))
  {
    strcpy(co_Weight,"<meta charset=utf-8>");
    strcat(co_Weight,"무게 조건 변경 (0~100) : ");
    strcat(co_Weight, myHttpServer.arg("Weight").c_str());
    conditionWeight=atoi(myHttpServer.arg("Weight").c_str());

    if(conditionWeight>=100) conditionWeight=100;
    else if(conditionWeight<0) conditionWeight=0;

    myHttpServer.send(200,"text/html",co_Weight);
    
  }
  else myHttpServer.send(200,"text/plain","Somthing wrong");
}
void thingspeakpub(int num,int value)
{
  char buff[5], pub[1000];
  snprintf(buff,sizeof(buff),"%d",value);
  snprintf(pub,sizeof(pub),"channels/1393641/publish/fields/field%d/0NX20M0E910B87BB",num);
  MQTTClient.publish(pub,buff);
}
