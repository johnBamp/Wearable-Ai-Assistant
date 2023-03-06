/*Code created by John R

This code initializes an Adafruit ILI9341 TFT LCD display, reads messages from the Serial Monitor and displays them on the screen with an adjustable font size. It also cycles through the font sizes when a button is pressed andprints the state of another button to the Serial Monitor every 4.5 seconds.

The setup() function initializes the Serial communication, sets the button pins as inputs with pull-up resistors, initializes the TFT screen and sets its rotation to landscape.

The loop() function reads the current time and checks if it's time to print the state of buttonPin1 to the Serial Monitor. It also reads any incoming messages from the Serial Monitor and saves them to the response variable if they start with "msg:". It then clears the screen, cycles through the font sizes when buttonPin2 is pressed and prints the header and the message with the selected font size to the screen.

The response variable holds the message to be displayed on the screen 

Also, all the code was commetented by chat gpt, including the message above, as I forgot to put them in beforehand ¯\ (ツ) /¯ */

// include required libraries
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

// define button pins
#define buttonPin1 13
#define buttonPin2 10

// define screen pins
#define screenCS 6
#define screenDS 5

// create an instance of the Adafruit_ILI9341 class
Adafruit_ILI9341 tft = Adafruit_ILI9341(screenCS, screenDS);

void setup() {
  // start serial communication
  Serial.begin(115200);

  // set button pins as input pins pulled-up to the chips internal pull ups
  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);

  // initialize the TFT screen
  tft.begin();
  // set screen rotation to 3 (landscape)
  tft.setRotation(3);
}

// define some variables
String response;
String responseCheck;
unsigned long previousMillis = 0;
const long interval = 4500;
int x = 1;

void loop() {
  // get the current time
  unsigned long currentMillis = millis();

  // check if it's time to print the button state
  if (currentMillis - previousMillis >= interval) {
    // save the current time
    previousMillis = currentMillis;
    // print the state of buttonPin1 to the serial monitor
    Serial.print("btn: ");
    Serial.println(digitalRead(buttonPin1));
  }

  // wait for 500 milliseconds
  delay(500);

  // read any incoming message from the serial monitor
  responseCheck = Serial.readString();
  
  // check if the message starts with "msg:"
  if(responseCheck.startsWith("msg:")){
    // if yes, save the message to the response variable
    response = responseCheck;
    // remove "msg:" from the message
    response.remove(0,5);
  }

  // clear the screen
  tft.fillScreen(ILI9341_BLACK);

  // check if buttonPin2 is pressed
  if(digitalRead(buttonPin2) == 0){
    // if yes, cycle through the font sizes
    if(x < 3){
      x++;
      delay(500);
    } else {
      x = 1;
      delay(500);
    }
  }

  // print the header
  tft.setCursor(52,0);
  tft.setTextSize(3);
  tft.println("Ai Assistant");
  tft.drawLine(0, 30, 320, 30, ILI9341_WHITE);

  // print the message with the selected font size
  tft.setTextSize(x);
  tft.setCursor(0,35);
  tft.println(response);
}


  
