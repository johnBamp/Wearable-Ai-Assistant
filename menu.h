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

int screenValue = 0;
const int numButtons = 12;
Grid menuGrid = Grid(3, 4, &tft);
Button* buttons[numButtons];
const char* names[numButtons] = { "Clock", "Jarvis", "Pong", "Mines", "Timer", "Count", "Calc", "ToDo", "Weath", "Trivia", "WiFi", "Sleep" };

void placeholder2() {
  return;
}

void setupMenu() {
  for (int i = 0; i < numButtons; ++i) {
    buttons[i] = new Button(names[i], i / 4, i % 4, 1, 1, ILI9341_WHITE, ILI9341_BLACK, 2, true, &tft, &menuGrid, 1, 1, 1, ILI9341_BLACK, 40, placeholder2, &ts, ILI9341_BLUE);
  }
  screenValue = 0;
}

void drawMenu() {
  for (int i = 0; i < numButtons; ++i) {
    buttons[i]->draw();
  }
}

void readMenu() {
  for (int i = 0; i < numButtons; ++i) {
    buttons[i]->checkTouch();
    if (buttons[i]->isPressed) {
      screenValue = i + 1;
    }
  } 
}
