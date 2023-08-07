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
#include <EEPROM.h>

extern Adafruit_FT6206 ts;
extern Adafruit_ILI9341 tft;

extern Grid mainGrid;

Grid keyGrid2 = Grid(7, 10, &tft);

Keyboard keys2 = Keyboard(2, &tft, &keyGrid2, &ts);

WiFiClientSecure client;

ChatGPT<WiFiClientSecure> chat_gpt(&client, "v1", "my api key");

ScrollableLabel gptInterface = ScrollableLabel(
"",1,  0, 2, 10, ILI9341_WHITE, ILI9341_BLACK, 2, false, &tft,  &keyGrid2, 4,4,2,ILI9341_WHITE, 5,&ts);

Label gptHeader= Label("Ai Assistant",0, 0, 1, 5, ILI9341_WHITE, ILI9341_BLACK, 2,true, &tft, &mainGrid, 4, 4, 2, ILI9341_BLACK, 10);

void aiSetup(){
  keys2.draw();
  gptInterface.draw();
  gptHeader.draw();
}
String oldInput3;

void aiLoop(){
  keys2.readKeys();
  if(oldInput3 != keys2.input){
    gptInterface.setText(keys2.input);
  }
  gptInterface.checkTouch();

  oldInput3 = keys2.input;

  if(keys2.enterClicked()){
    String result;
    if (chat_gpt.simple_message("gpt-3.5-turbo-0301", "user", keys2.input, result)) {
      Serial.println("===OK===");
      Serial.println(result);
      gptInterface.setText(result);
    } else {
      Serial.println("===ERROR===");
      Serial.println(result);
    }
  }
  
}
