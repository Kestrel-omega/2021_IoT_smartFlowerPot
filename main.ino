#include <gaeul.h>
#include <jiho.h>
#include "Arduino.h"
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
ESP8266WebServer myHttpServer(80);

int conditionHumid = 50;
int conditionWeight = 50;
void wifiset();
void setup()
{
  settingGaeul();
  wifiset();
  webhtml();
  //sensorSetup();
  //ledOn();
}
int angle=0;

void loop()
{
  myHttpServer.handleClient();
  int temp;
  int humid;
  float temp1;
  float humid1;
  //getHumid();
  //readDHT11(&temp, &humid);
  //getWeather(&temp1, &humid1);
  //servofunc(angle);
  //displayLCD(temp, humid);
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
  Serial.printf("connected!\n");
  Serial.printf("please contact IP addr...");
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
  strcpy(buff,"<html>\r\n2021_IoT_smartFlowerPot<br>\r\n<a href=/status>Check Status</a><br>\r\n<a href=/on>On</a><br>\r\n<a href=/off>Off</a><br>\r\n<form method=\"get\" action=\"input\">Condition maintain humid <input type =\"text\" name=\"Humid\"><input type=\"submit\" ></form>\r\n<form method=\"get\" action=\"input\">Condition maintain weight <input type =\"text\" name=\"Weight\"><input type=\"submit\" ></form>\r\n</html>");
  myHttpServer.send(200,"text/html",buff);//text type html send
}
void fnStatus(void)
{
  myHttpServer.send(200,"text/html","status");
}
void fnNotFound()
{
  myHttpServer.send(404,"text/plain","Wrong");
}
void fnOn()
{
  myHttpServer.send(200,"text/html","on");
}
void fnOff()
{
  myHttpServer.send(200,"text/html","off");

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
