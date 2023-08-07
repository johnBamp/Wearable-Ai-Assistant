#include <Adafruit_GFX.h>
#include <SPI.h>
#include <Adafruit_ILI9341.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <SPI.h>
#include <Adafruit_ILI9341.h>
#include <Wire.h>
#include <Adafruit_FT6206.h>
#include "time.h"
#include "arduinoGUI.h"
#include <Arduino.h>

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = -18000;
const int   daylightOffset_sec = 3600;

extern Adafruit_FT6206 ts;
extern Adafruit_ILI9341 tft;

extern Grid mainGrid;

Label timeHeader= Label("Time",0, 0, 1, 5, ILI9341_WHITE, ILI9341_BLACK, 2,true, &tft, &mainGrid, 4, 4, 2, ILI9341_BLACK, 10);

Label currentTime = Label("0:0:0",1, 0, 3, 8, ILI9341_WHITE, ILI9341_BLACK, 4,true, &tft, &mainGrid, 4, 4, 2, ILI9341_BLACK, 10);

Label currentDate = Label("",4, 0, 2, 8, ILI9341_WHITE, ILI9341_BLACK, 2,true, &tft, &mainGrid, 4, 4, 2, ILI9341_BLACK, 10);


String getTime() {
  struct tm timeinfo;

  if(!getLocalTime(&timeinfo)){
    return "Internet Error";
  }
  
  char timeStringBuff[50]; // make sure this is large enough for your date and time string
  strftime(timeStringBuff, sizeof(timeStringBuff), "%H:%M:%S", &timeinfo);
  
  String timeStr = String(timeStringBuff);
  
  return timeStr;
}

String getDate() {
  struct tm timeinfo;

  if(!getLocalTime(&timeinfo)){
    return "Internet Error";
  }
  
  char timeStringBuff[50]; // make sure this is large enough for your date and time string
  strftime(timeStringBuff, sizeof(timeStringBuff), "%A, %B %d", &timeinfo);
  
  String timeStr = String(timeStringBuff);
  
  return timeStr;
}

void clockSetup(){
  timeHeader.draw();
  currentTime.draw();
  currentDate.draw();
}

String oldTime;
String oldDate;

void clockLoop(){
  String time = getTime();
  if(time != oldTime){
    currentTime.setText(time);
  }

  String date = getDate();
  if(date != oldDate){
    currentDate.setText(date);
  }

  oldTime = time;

  oldDate = date;
}
