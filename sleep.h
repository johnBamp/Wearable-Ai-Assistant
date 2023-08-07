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

extern Adafruit_FT6206 ts;
extern Adafruit_ILI9341 tft;


void sleepSetup(){
  tft.fillScreen(ILI9341_BLACK);
}

#define TOUCH_THRESHOLD 5000 // Duration for touch to trigger sleep mode in ms

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
