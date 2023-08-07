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

extern Adafruit_FT6206 ts;
extern Adafruit_ILI9341 tft;

extern Grid mainGrid;

Label pointsA = Label("0",0, 0, 1, 2, ILI9341_WHITE, ILI9341_BLACK, 2,true, &tft, &mainGrid, 1, 1, 1, ILI9341_BLACK, 10);

Label pointsB = Label("0",0, 2, 1, 2, ILI9341_WHITE, ILI9341_BLACK, 2,true, &tft, &mainGrid, 1, 1, 1, ILI9341_BLACK, 10);

#define PADDLE_HEIGHT 50
#define PADDLE_WIDTH 10
#define BALL_SIZE 10
#define LABEL_HEIGHT 40

// Paddle and Ball Positions
int paddle1Y = 0;
int paddle2Y = 0;
int ballX = 0;
int ballY = 0;
int oldPaddle1Y = 0; // added to track old paddle positions
int oldPaddle2Y = 0;

// Ball direction
int ballDX = 1; // Decreased speed
int ballDY = 1; // Decreased speed

int pointsPlayer1 = 0;
int pointsPlayer2 = 0;

void pongSetup() { 

  pointsA.draw();
  pointsB.draw();
  
  // Initial paddle positions
  paddle1Y = (tft.height() - LABEL_HEIGHT) / 2;
  paddle2Y = (tft.height() - LABEL_HEIGHT) / 2;
  
  // Initial ball position
  ballX = tft.width() / 2;
  ballY = (tft.height() - LABEL_HEIGHT) / 2;

  // Draw paddles and ball
  tft.fillRect(0, paddle1Y, PADDLE_WIDTH, PADDLE_HEIGHT, ILI9341_WHITE);
  tft.fillRect(tft.width()-PADDLE_WIDTH, paddle2Y, PADDLE_WIDTH, PADDLE_HEIGHT, ILI9341_WHITE);
  tft.fillRect(ballX, ballY, BALL_SIZE, BALL_SIZE, ILI9341_WHITE);
}

void pongLoop() {

  // Clear old ball position
  tft.fillRect(ballX, ballY, BALL_SIZE, BALL_SIZE, ILI9341_BLACK);
  
  // Update ball position
  ballX += ballDX;
  ballY += ballDY;

  // Check for collision with screen edge and paddle
  if (ballX <= PADDLE_WIDTH && ballY >= paddle1Y && ballY <= paddle1Y + PADDLE_HEIGHT) ballDX *= -1;
  if (ballX + BALL_SIZE >= tft.width() - PADDLE_WIDTH && ballY >= paddle2Y && ballY <= paddle2Y + PADDLE_HEIGHT) ballDX *= -1;

  if (ballY <= 0 + LABEL_HEIGHT || ballY + BALL_SIZE >= tft.height()) ballDY *= -1;

  // Check if a player scored a point
  if (ballX <= 0) {
    pointsPlayer2++;
    pointsB.setText(String(pointsPlayer2));
    pointsB.draw();
    ballX = tft.width() / 2;
    ballY = (tft.height() - LABEL_HEIGHT) / 2;
  }
  if (ballX + BALL_SIZE >= tft.width()) {
    pointsPlayer1++;
    pointsA.setText(String(pointsPlayer1));
    pointsA.draw();
    ballX = tft.width() / 2;
    ballY = (tft.height() - LABEL_HEIGHT) / 2;
  }

  // If the screen is touched, update paddle positions
  if (ts.touched()) {
    TS_Point p = ts.getPoint();
    int swap = p.x;
    p.x = p.y;
    p.y = swap;

    p.y = map(p.y, 240, 0, 0, 240);
    if (p.x < tft.width()/2) paddle1Y = p.y - PADDLE_HEIGHT / 2; // Player 1
    else paddle2Y = p.y - PADDLE_HEIGHT / 2; // Player 2
  }

  // Keep the paddles on the screen
  paddle1Y = max(0, min(paddle1Y, tft.height() - PADDLE_HEIGHT - LABEL_HEIGHT));
  paddle2Y = max(0, min(paddle2Y, tft.height() - PADDLE_HEIGHT - LABEL_HEIGHT));

  // Clear old paddle positions if they have moved
  if (paddle1Y != oldPaddle1Y) {
    tft.fillRect(0, oldPaddle1Y, PADDLE_WIDTH, PADDLE_HEIGHT, ILI9341_BLACK);
    oldPaddle1Y = paddle1Y;
  }
  if (paddle2Y != oldPaddle2Y) {
    tft.fillRect(tft.width()-PADDLE_WIDTH, oldPaddle2Y, PADDLE_WIDTH, PADDLE_HEIGHT, ILI9341_BLACK);
    oldPaddle2Y = paddle2Y;
  }

  // Draw paddles and ball in new positions
  tft.fillRect(0, paddle1Y, PADDLE_WIDTH, PADDLE_HEIGHT, ILI9341_WHITE);
  tft.fillRect(tft.width()-PADDLE_WIDTH, paddle2Y, PADDLE_WIDTH, PADDLE_HEIGHT, ILI9341_WHITE);
  tft.fillRect(ballX, ballY, BALL_SIZE, BALL_SIZE, ILI9341_WHITE);
}
