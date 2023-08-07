#include <Adafruit_GFX.h>
#include <SPI.h>
#include <Adafruit_ILI9341.h>
#include <Wire.h>
#include <Adafruit_FT6206.h>
#include <WiFi.h>
#include "time.h"
#include "arduinoGUI.h"
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include <ChatGPT.hpp>
#include <EEPROM.h>

#include "menu.h"
#include "clock.h"
#include "WiFi.h"
#include "AiAssistant.h"
#include "pong.h"

#define TOUCH_THRESHOLD 5000

#define TFT_CHIP_SLCT 5
#define TFT_DATA_CMD 27
#define RESET 14

#define baud_rate 115200

Adafruit_FT6206 ts = Adafruit_FT6206();
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CHIP_SLCT, TFT_DATA_CMD);

Grid mainGrid = Grid(6, 8, &tft);

Button homeButton = Button("Home", 0, 5, 1, 3, ILI9341_WHITE, ILI9341_BLACK, 2, true, &tft, &mainGrid, 1, 1, 1, ILI9341_BLACK, 10, placeholder2, &ts, ILI9341_BLUE);


void homeButtonSetup(){
  tft.fillScreen(ILI9341_BLACK);
  homeButton.draw();
}

void homeButtonLoop(){
  homeButton.checkTouch();
  if (homeButton.isPressed) {
    screenValue = 0;
  }
}

void sleepSetup(){
  tft.fillScreen(ILI9341_BLACK);
}

unsigned long touchStart = 0; // Variable to store the start time of touch

void sleepLoop() {
  if (ts.touched()) {
    if (touchStart == 0) { // If a touch has just started
      touchStart = millis(); // Record the start time of touch
    } else if (millis() - touchStart > TOUCH_THRESHOLD) { // If the touch duration has exceeded the threshold
      screenValue = 0;
      touchStart = 0; // Reset touch start time
    }
  } else {
    touchStart = 0; // Reset touch start time
  }
}
void setup() {
  Serial.begin(115200);
  tft.begin();
  ts.begin();
  tft.setRotation(3);
  tft.fillScreen(ILI9341_BLACK);
  EEPROM.begin(EEPROM_SIZE);

  // Ignore SSL certificate validation
  client.setInsecure();

  setupMenu();
  drawMenu();

}

String oldInput;
int oldScreenValue;

void loop() {
  //Serial.println(screenValue);

  if(screenValue != oldScreenValue){
    if(screenValue == 0){
      tft.fillScreen(ILI9341_BLACK);
      drawMenu();
      //delay(2);
    }else{
      tft.fillScreen(ILI9341_BLACK);
      homeButtonSetup();
    }if(screenValue == 1){
      clockSetup();
    }else if(screenValue == 2){
      aiSetup();
    }else if(screenValue == 3){
      pongSetup();
    }else if(screenValue == 11){
      wifiSetup();
    }else if(screenValue == 12){
      sleepSetup();
    }
    oldScreenValue = screenValue;
    delay(2);
  }
  
  if(screenValue == 0){
    readMenu();
  }else{
    homeButtonLoop();
  }
  
  if(screenValue == 1){
    clockLoop();
  }else if(screenValue == 2){
    aiLoop();
  }else if(screenValue == 3){
    pongLoop();
  }else if(screenValue == 11){
    wifiLoop();
  }else if(screenValue == 12){
    sleepLoop();
  }
}
