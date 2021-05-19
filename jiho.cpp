#include "jiho.h"

// ESP8266 and sensors initialize
void sensorSetup()
{
  // LED Setting
  pinMode(ledPin, OUTPUT);
  // MPU6050 Setting
  Wire.begin(4, 5);
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);
  Wire.write(0x01);
  Wire.write(0x1C);
  Wire.write(0x00);
  // SZH-SSBH-074 Setting
  pinMode(sensorPin, INPUT);
  // LCD Setting
  LiquidCrystal lcd(RS, E, D4, D5, D6, D7);
  lcd.begin(16,2);
}

// LED brightness (%)
void LEDControl(float state)
{
  analogWrite(state*10.23);
}

// get temperature data from MPU6050
float getTemp()
{
  int16_t tempData
  float temp = 0;
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x41);
  Wire.endTransmission(true);
  Wire.requestFrom(MPU_ADDR, 2, true);
  tempData = Wire.read() << 8 | Wire.read();
  temp = (float)Tmp/340.00+36.53;
  return temp;
}

// get humidity_of_soil data from SZH-SSBH-074
float getHumid()
{
  float humid = ((1023-analogRead(sensorPin))*0.0977517);
  return humid;
}

// export data to LCD
void displayLCD(float temp, float humid)
{
  char str1[16] = {0,};
  char str2[16] = {0,};
  lcd.setCursor(0,0);
  snprintf(str1,"Temp : %f °C", temp);
  lcd.setCursor(0,1);
  snprintf(str2,"Humid : %f %%", humid);
}

// get current weather data from Openweather API
void getWeather()
{
  
}
