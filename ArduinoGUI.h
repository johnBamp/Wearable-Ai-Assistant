#ifndef ARDUINOGUI_H
#define ARDUINOGUI_H

#include <Adafruit_GFX.h>
#include <SPI.h>
#include <Adafruit_ILI9341.h>
#include <Wire.h>
#include <Adafruit_FT6206.h>

#define TFT_CHIP_SLCT 32
#define TFT_DATA_CMD 33

#define baud_rate 9600

class Grid {
  private:
    int rows;
    int columns;
    Adafruit_ILI9341* tft;

  public:
    Grid(int rows, int columns, Adafruit_ILI9341* tft);
    int getRows();
    int getColumns();
    void setRows(int r);
    void setColumns(int c);
};

class Label {
  protected:
    String text;
    int row;
    int column;
    int rowspan;
    int colspan;
    uint16_t backgroundColor;
    uint16_t textColor;
    uint8_t textSize;
    bool centered;
    Adafruit_ILI9341* tft;
    Grid* grid;
    int padx;  // Padding x
    int pady;  // Padding y
    int border;  // Border width
    uint16_t borderColor;  // Border color
    int radius;  // Radius for rounded corners
    bool hidden;
    int topLine = 0;

  public:
    Label(String text, int row, int column, int rowspan, int colspan, uint16_t backgroundColor, uint16_t textColor, uint8_t textSize, bool centered, Adafruit_ILI9341* tft, Grid* grid, int padx, int pady, int border, uint16_t borderColor, int radius);
    void draw();
    void hide();
    void setTopLine(int line);
    int getTopLine();
    void setText(String t);
    String getText();
    void setRow(int r);
    int getRow();
    void setColumn(int c);
    int getColumn();
    void setRowspan(int rs);
    int getRowspan();
    void setColspan(int cs);
    int getColspan();
    void setBackgroundColor(uint16_t color);
    uint16_t getBackgroundColor();
    bool isHidden();
};

class ScrollableLabel : public Label {
  private:
    Adafruit_FT6206* cts;

  public:
    ScrollableLabel(String text, int row, int column, int rowspan, int colspan, uint16_t backgroundColor, uint16_t textColor, uint8_t textSize, bool centered, Adafruit_ILI9341* tft, Grid* grid, int padx, int pady, int border, uint16_t borderColor, int radius, Adafruit_FT6206* cts, int topLine = 0);
    void checkTouch();
};

class Button : public Label {
  private:
    void (*onClick)();
    Adafruit_FT6206* cts;
    uint16_t activeColor;
    uint16_t inactiveColor;
    int radius;

  public:
    bool wasPressed = false;
    bool isPressed;
    Button(String text, int row, int column, int rowspan, int colspan, uint16_t backgroundColor, uint16_t textColor, uint8_t textSize, bool centered, Adafruit_ILI9341* tft, Grid* grid, int padx, int pady, int border, uint16_t borderColor, int radius, void (*onClick)(), Adafruit_FT6206* cts, uint16_t activeColor);
    void checkTouch();
};

class Keyboard {
  private:
    Button* keys[5][10];
    Button* modeKey;
    Button* deleteKey;
    Button* spaceKey;
    Button* enterKey;
    Adafruit_ILI9341* tft;
    Grid* grid;
    Adafruit_FT6206* cts;
    bool mode;
    int lineSkip;
    bool upperCase = true;
    String numericSpecial[5][10] = {{"1", "2", "3", "4", "5", "6", "7", "8", "9", "0"},
      {"!", "@", "#", "$", "%", "^", "&", "*", "(", ")"},
      {"_", "+", "{", "}", "|", "~", "<", ">", "?", ":"},
      {"MODE", " ", "DEL", "ENT"}
    };

    String qwerty[5][10] = {{"Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P"},
      {"A", "S", "D", "F", "G", "H", "J", "K", "L", "-"},
      {"Z", "X", "C", "V", "B", "N", "M", ",", ".", "/"},
      {"MODE", " ", "DEL", "ENT"}
    };

  public:
    Keyboard(int skip, Adafruit_ILI9341* tft, Grid* grid, Adafruit_FT6206* cts);
    String input; // Store user input
    void draw();
    void readKeys();
    void toggleMode();
    void handleKeyPress(String key); // Function to handle key press events
    bool enterClicked();
    // Other methods can go here...
};



#endif
