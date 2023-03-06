#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

#define buttonPin1 13
#define buttonPin2 10

#define screenCS 6
#define screenDS 5

Adafruit_ILI9341 tft = Adafruit_ILI9341(screenCS, screenDS);

void setup() {
  Serial.begin(115200);
  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);

  tft.begin();
  tft.setRotation(3);

}

String response;
String responseCheck;
unsigned long previousMillis = 0;
const long interval = 4500;

int x = 1;


void loop() {



  unsigned long currentMillis = millis();


  if (currentMillis - previousMillis >= interval) {

    previousMillis = currentMillis;
    Serial.print("btn: ");
    Serial.println(digitalRead(buttonPin1));

  }

  delay(500);

  responseCheck = Serial.readString();
  
  if(responseCheck.startsWith("msg:")){
    response = responseCheck;
    response.remove(0,5);
  }


  tft.fillScreen(ILI9341_BLACK);

  if(digitalRead(buttonPin2) == 0){
    if(x < 3){
      x++;
      delay(500);
    } else {
      x = 1;
      delay(500);
    }
  }
  
  tft.setCursor(52,0);
  tft.setTextSize(3);
  tft.println("Ai Assistant");
  tft.drawLine(0, 30, 320, 30, ILI9341_WHITE);
  tft.setTextSize(x);
  tft.setCursor(0,35);
  tft.println(response);

}

  