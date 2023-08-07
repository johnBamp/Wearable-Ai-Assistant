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
#include "time.h"

extern Adafruit_FT6206 ts;
extern Adafruit_ILI9341 tft;

extern Grid mainGrid;

#define EEPROM_SIZE 512  // Maximum is 512 bytes on ESP32

#define SSID_ADDRESS 0  // Start address for SSID
#define PASS_ADDRESS 256  // Start address for Password

void clearEEPROM() {
  for (int i = 0; i < EEPROM_SIZE; i++) {
    EEPROM.write(i, 255);
  }
  EEPROM.commit();
}

Label connectedStatus= Label("Not Connected",0, 0, 1, 5, ILI9341_WHITE, ILI9341_BLACK, 2,true, &tft, &mainGrid, 1, 1, 1, ILI9341_BLACK, 10);

Grid keyGrid = Grid(7, 10, &tft);

Keyboard keys = Keyboard(2, &tft, &keyGrid, &ts);

Button ssidButton = Button("", 1, 0, 1, 10, ILI9341_WHITE, ILI9341_BLACK, 2, false, &tft, &keyGrid, 1, 1, 1, ILI9341_BLACK, 10, placeholder2, &ts, ILI9341_BLUE);

Button passwordButton = Button("", 2, 0, 1, 10, ILI9341_WHITE, ILI9341_BLACK, 2, false, &tft, &keyGrid, 1, 1, 1, ILI9341_BLACK, 10, placeholder2, &ts, ILI9341_BLUE);

void saveToEEPROM(int address, const String& data) {
  for (int i = 0; i < data.length(); i++) {
    EEPROM.write(address + i, data[i]);
  }
  EEPROM.write(address + data.length(), '\0');  // Null terminator
  EEPROM.commit();  // Commit changes to EEPROM
}

String loadFromEEPROM(int address) {
  String data;
  char ch;
  int i = 0;
  do {
    ch = EEPROM.read(address + i);
    if (ch != '\0' && ch != 255) {
      data += ch;
      i++;
    }
  } while (ch != '\0' && ch != 255 && i < 255);
  return data;
}

void wifiSetup(){
  connectedStatus.draw();
  keys.draw();
  ssidButton.draw();
  passwordButton.draw();

  ssidButton.setText(loadFromEEPROM(SSID_ADDRESS));  // Load SSID from EEPROM
  passwordButton.setText(loadFromEEPROM(PASS_ADDRESS));  // Load Password from EEPROM
}

String oldInput2;
bool state;

void wifiLoop(){
  keys.readKeys();
  
  if(oldInput2 != keys.input){
    if(state == 0){
      ssidButton.setText(keys.input);
      ssidButton.draw();
    }else if(state == 1){
      passwordButton.setText(keys.input);
      passwordButton.draw();
    }
  }

  ssidButton.checkTouch();
  if (ssidButton.isPressed) {
    state = 0;
    keys.input = ssidButton.getText(); // Update keys.input with current text of the ssidButton
  }

  passwordButton.checkTouch();
  if (passwordButton.isPressed) {
    state = 1;
    keys.input = passwordButton.getText(); // Update keys.input with current text of the passwordButton
  }

  if (keys.enterClicked()) {
    connectedStatus.draw();
    String ssid = ssidButton.getText();
    String password = passwordButton.getText();

    saveToEEPROM(SSID_ADDRESS, ssid);
    saveToEEPROM(PASS_ADDRESS, password);

    Serial.println(ssid);
    Serial.println(password);

    char ssidChar[ssid.length()+1];
    ssid.toCharArray(ssidChar, ssid.length()+1);

    char passwordChar[password.length()+1];
    password.toCharArray(passwordChar, password.length()+1);

    Serial.println(ssidChar);
    Serial.println(passwordChar);

    WiFi.begin(ssidChar, passwordChar);

    int i = 0;
    while (WiFi.status() != WL_CONNECTED) {
      if(i >= 10){
        break;
      }
      Serial.println("Connecting...");
      i++;
      delay(500);
    }
    switch (WiFi.status()) {
      case WL_CONNECTED:
        configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
        connectedStatus.setText("Connected");
        connectedStatus.draw();
        Serial.println("Connected!");
        break;
      case WL_NO_SSID_AVAIL:
        connectedStatus.setText("Failed: No SSID Available");
        connectedStatus.draw();
        Serial.println("Failed to connect: No SSID Available");
        break;
      case WL_CONNECT_FAILED:
        connectedStatus.setText("Failed: Incorrect Password");
        connectedStatus.draw();
        Serial.println("Failed to connect: Incorrect Password");
        break;
      default:
        connectedStatus.setText("Failed: Unknown Error");
        connectedStatus.draw();
        Serial.println("Failed to connect: Unknown Error");
        break;
    }
  }
  oldInput2 = keys.input;
}
