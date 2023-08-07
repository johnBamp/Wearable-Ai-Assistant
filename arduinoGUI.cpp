#include "arduinoGUI.h"

Grid::Grid(int rows, int columns, Adafruit_ILI9341* tft): rows(rows), columns(columns), tft(tft) {}

int Grid::getRows() {
  return rows;
}

int Grid::getColumns() {
  return columns;
}

void Grid::setRows(int r) {
  rows = r;
}

void Grid::setColumns(int c) {
  columns = c;
}

Label::Label(String text, int row, int column, int rowspan, int colspan, uint16_t backgroundColor, uint16_t textColor, uint8_t textSize, bool centered, Adafruit_ILI9341* tft, Grid* grid, int padx, int pady, int border, uint16_t borderColor, int radius)
  : text(text), row(row), column(column), rowspan(rowspan), colspan(colspan), backgroundColor(backgroundColor), textColor(textColor), textSize(textSize), centered(centered), tft(tft), grid(grid), padx(padx), pady(pady), border(border), borderColor(borderColor), radius(radius) {}

void Label::draw() {
  hidden = false;

  int x = (tft->width() / grid->getColumns()) * column;
  int y = (tft->height() / grid->getRows()) * row;
  int w = (tft->width() / grid->getColumns()) * colspan;
  int h = (tft->height() / grid->getRows()) * rowspan;

  // Draw the border
  tft->fillRoundRect(x, y, w, h, radius, borderColor);
  // Draw the background color inside the border
  tft->fillRoundRect(x + border, y + border, w - 2 * border, h - 2 * border, radius, backgroundColor);

  tft->setTextColor(textColor);
  tft->setTextSize(textSize);

  // Adjust the drawing area for padding
  x += padx + border;
  y += pady + border;
  w -= 2 * (padx + border);
  h -= 2 * (pady + border);

  // Estimate max characters per line
  int maxCharsPerLine = w / (6 * textSize);

  // Calculate the maximum lines that can fit in the box
  int maxLines = h / (8 * textSize);

  String line = "";
  String word = "";
  int lineCount = 0;

  // Simple text wrapping and line counting
  for (int i = 0; i < text.length() && lineCount < maxLines + topLine; i++) {
    char c = text[i];
    if (c == ' ' || c == '\n' || i == text.length() - 1) {
      if (i == text.length() - 1 && c != ' ' && c != '\n') {
        word += c;
      }
      if (line.length() + word.length() <= maxCharsPerLine) {
        line += word;
        if (c == ' ') {
          line += " ";
        }
      } else {
        line = word + " ";
        lineCount++;
      }
      word = "";
    } else {
      word += c;
    }
  }

  // Add one to lineCount if there is a line that hasn't been counted
  if (line.length() > 0) {
    lineCount++;
  }

  // Adjust the y coordinate to center the text vertically
  y += max(0, (h - lineCount * 8 * textSize) / 2);

  // Reset the line and word
  line = "";
  word = "";

  // Reset lineCount for the drawing loop
  lineCount = 0;

  Serial.print("Text: ");
  Serial.print(text);

  // Draw the text centered on x and y axis
  for (int i = 0; i < text.length() && lineCount < maxLines + topLine; i++) {
    char c = text[i];
    if (c == ' ' || c == '\n' || i == text.length() - 1) {
      if (i == text.length() - 1 && c != ' ' && c != '\n') {
        word += c;
      }
      if (line.length() + word.length() <= maxCharsPerLine) {
        line += word;
        if (c == ' ') {
          line += " ";
        }
      } else {
        if (lineCount >= topLine) {
          int newX = centered ? x + (w - line.length() * 6 * textSize) / 2 : x;
          Serial.print(" X: ");
          Serial.print(newX);
          Serial.print(" Y: ");
          Serial.print(y);
          tft->setCursor(newX, y);
          tft->println(line);
          y += 8 * textSize;
        }
        line = word + " ";
        lineCount++;
      }
      word = "";
    } else {
      word += c;
    }
  }

  Serial.print(" Line count: ");
  Serial.print(lineCount);
  Serial.print(" Line length: ");
  Serial.print(line.length());
  Serial.print(" Top Line: ");
  Serial.print(topLine);
  Serial.print(" Max lines: ");
  Serial.print(maxLines);

  // Print the last line if it's not empty and doesn't exceed maxLines
  if (line.length() > 0 && lineCount >= topLine && lineCount < maxLines + topLine) {
    int newX = centered ? x + (w - line.length() * 6 * textSize) / 2 : x;
    Serial.print(" X: ");
    Serial.print(newX);
    Serial.print(" Y: ");
    Serial.print(y);
    tft->setCursor(newX, y);
    tft->println(line);
  }

  Serial.println();
}


void Label::hide() {
  int x = (tft->width() / grid->getColumns()) * column;
  int y = (tft->height() / grid->getRows()) * row;
  int w = (tft->width() / grid->getColumns()) * colspan;
  int h = (tft->height() / grid->getRows()) * rowspan;

  hidden = true;

  // Draw over the area with the background color
  tft->fillRoundRect(x, y, w, h, radius, ILI9341_BLACK);
}

void Label::setTopLine(int line) {
  topLine = line;
}

int Label::getTopLine() {
  return topLine;
}

void Label::setText(String t) {
  text = t;
  draw();
}

String Label::getText() {
  return text;
}

void Label::setRow(int r) {
  row = r;
}

int Label::getRow() {
  return row;
}

void Label::setColumn(int c) {
  column = c;
}

int Label::getColumn() {
  return column;
}

void Label::setRowspan(int rs) {
  rowspan = rs;
}

int Label::getRowspan() {
  return rowspan;
}

void Label::setColspan(int cs) {
  colspan = cs;
}

int Label::getColspan() {
  return colspan;
}

void Label::setBackgroundColor(uint16_t color) {
  backgroundColor = color;
}

uint16_t Label::getBackgroundColor() {
  return backgroundColor;
}

bool Label::isHidden() {
  return hidden;
}

ScrollableLabel::ScrollableLabel(String text, int row, int column, int rowspan, int colspan, uint16_t backgroundColor, uint16_t textColor, uint8_t textSize, bool centered, Adafruit_ILI9341* tft, Grid* grid, int padx, int pady, int border, uint16_t borderColor, int radius, Adafruit_FT6206* cts, int topLine)
  : Label(text, row, column, rowspan, colspan, backgroundColor, textColor, textSize, centered, tft, grid, padx, pady, border, borderColor, radius), cts(cts) {}

void ScrollableLabel::checkTouch() {
  if (cts->touched()) {
    TS_Point touchPoint = cts->getPoint();
    int x = (tft->width() / grid->getColumns()) * column;
    int y = (tft->height() / grid->getRows()) * row;
    int w = (tft->width() / grid->getColumns()) * colspan;
    int h = (tft->height() / grid->getRows()) * rowspan;

    int swap = touchPoint.x;
    touchPoint.x = touchPoint.y;
    touchPoint.y = swap;

    touchPoint.y = map(touchPoint.y, 240, 0, 0, 240);

    if (touchPoint.x >= x && touchPoint.x <= x + w && touchPoint.y >= y && touchPoint.y <= y + h) {
      // Estimate max characters per line
      int maxCharsPerLine = w / (6 * textSize);

      // Calculate the maximum lines that can fit in the box
      int maxLines = h / (8 * textSize);

      // Count lines in text
      int lineCount = 0;
      for (int i = 0; i < text.length(); i++) {
        char c = text[i];
        if (c == ' ' || c == '\n') {
          lineCount++;
        }
      }

      if (touchPoint.y > (y + h / 2)) {
        topLine++;
        draw();
      }
      if (touchPoint.y < (y + h / 2) && topLine != 0) {
        topLine--;
        draw();
      }
    }
  }
}

Button::Button(String text, int row, int column, int rowspan, int colspan, uint16_t backgroundColor, uint16_t textColor, uint8_t textSize, bool centered, Adafruit_ILI9341* tft, Grid* grid, int padx, int pady, int border, uint16_t borderColor, int radius, void (*onClick)(), Adafruit_FT6206* cts, uint16_t activeColor)
  : Label(text, row, column, rowspan, colspan, backgroundColor, textColor, textSize, centered, tft, grid, padx, pady, border, borderColor, radius),
    onClick(onClick), cts(cts), activeColor(activeColor), inactiveColor(backgroundColor), isPressed(false), radius(radius) {}

bool buttonJustPressed = false;

void Button::checkTouch() {
  if (cts->touched()) {
    TS_Point touchPoint = cts->getPoint();
    int x = (tft->width() / grid->getColumns()) * column;
    int y = (tft->height() / grid->getRows()) * row;
    int w = (tft->width() / grid->getColumns()) * colspan;
    int h = (tft->height() / grid->getRows()) * rowspan;

    int swap = touchPoint.x;
    touchPoint.x = touchPoint.y;
    touchPoint.y = swap;

    touchPoint.y = map(touchPoint.y, 240, 0, 0, 240);

    if (touchPoint.x >= x && touchPoint.x <= x + w && touchPoint.y >= y && touchPoint.y <= y + h) {
      if (!isPressed && !buttonJustPressed && !wasPressed) {
        setBackgroundColor(activeColor);
        draw();
        //onClick();
        isPressed = true;
        buttonJustPressed = true;
        wasPressed = true;
      }
    } else {
      if (isPressed) {
        setBackgroundColor(inactiveColor);
        draw();
      }
      isPressed = false;
      buttonJustPressed = false;
      wasPressed = false;
    }
  } else {
    if (isPressed) {
      setBackgroundColor(inactiveColor);
      draw();
    }
    isPressed = false;
    buttonJustPressed = false;
    wasPressed = false;
  }
}

void placeholder() {

}


void Keyboard::toggleMode() {
  // Toggle mode between alphanumeric and numeric/special
  mode = !mode;

  // Update buttons
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 10; j++) {
      keys[i][j]->setText(mode ? numericSpecial[i][j] : qwerty[i][j]);
      keys[i][j]->draw(); // Update the displayed button
    }
  }
  // Redraw the mode button to reflect the current mode
  modeKey->setText(mode ? "ALPHA" : "MODE");
  modeKey->draw();
}

Keyboard::Keyboard(int skip, Adafruit_ILI9341* tft, Grid* grid, Adafruit_FT6206* cts)
  : lineSkip(skip), tft(tft), grid(grid), cts(cts) {

  input = "";
  // Initialize QWERTY keys
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 10; j++) {
      keys[i][j] = new Button(qwerty[i][j], i + 1 + lineSkip, j, 1, 1, ILI9341_WHITE, ILI9341_BLACK, 2, true, tft, grid, 2, 2, 1, ILI9341_BLACK, 5, &placeholder, cts, ILI9341_BLUE);
    }
  }

  // Initialize bottom row keys
  modeKey = new Button("mode", 4 + lineSkip, 0, 1, 2, ILI9341_WHITE, ILI9341_BLACK, 2, true, tft, grid, 2, 2, 1, ILI9341_BLACK, 5, &placeholder, cts, ILI9341_BLUE);
  spaceKey = new Button(" ", 4 + lineSkip, 2, 1, 4, ILI9341_WHITE, ILI9341_BLACK, 2, true, tft, grid, 2, 2, 1, ILI9341_BLACK, 5, &placeholder, cts, ILI9341_BLUE);
  deleteKey = new Button("Del", 4 + lineSkip, 6, 1, 2, ILI9341_WHITE, ILI9341_BLACK, 2, true, tft, grid, 2, 2, 1, ILI9341_BLACK, 5, &placeholder, cts, ILI9341_BLUE);
  enterKey = new Button("Enter", 4 + lineSkip, 8, 1, 2, ILI9341_WHITE, ILI9341_BLACK, 2, true, tft, grid, 2, 2, 1, ILI9341_BLACK, 5, &placeholder, cts, ILI9341_BLUE);
}

void Keyboard::draw() {
  // Draw QWERTY keys
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 10; j++) {
      keys[i][j]->draw();
    }
  }

  // Draw bottom row keys
  modeKey->draw();
  spaceKey->draw();
  deleteKey->draw();
  enterKey->draw();
}

bool wasPressed[3][10] = {false};
bool wasModeKeyPressed = false;
bool wasSpaceKeyPressed = false;
bool wasDeleteKeyPressed = false;
bool wasEnterKeyPressed = false;

void Keyboard::readKeys() {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 10; j++) {
      keys[i][j]->checkTouch();

      if (keys[i][j]->getText() == "/" && keys[i][j]->isPressed && !wasPressed[i][j]) {
        upperCase = !upperCase;
        // Redraw the keyboard with the new case.
        for (int m = 0; m < 3; m++) {
          for (int n = 0; n < 10; n++) {
            String keyText = qwerty[m][n];
            if (upperCase) {
              keyText.toUpperCase();
            } else {
              keyText.toLowerCase();
            }
            keys[m][n]->setText(keyText);
            keys[m][n]->draw(); // Update the displayed button
          }
        }
      } else if (keys[i][j]->isPressed && !wasPressed[i][j]) {
        String text = keys[i][j]->getText();
        input += text;
        Serial.println(input);
        wasPressed[i][j] = true;
      } else if (!keys[i][j]->isPressed) {
        wasPressed[i][j] = false;
      }
    }
  }

  modeKey->checkTouch();
  if (modeKey->isPressed && !wasModeKeyPressed) {
    toggleMode();
    wasModeKeyPressed = true;
  } else if (!modeKey->isPressed) {
    wasModeKeyPressed = false;
  }

  spaceKey->checkTouch();
  if (spaceKey->isPressed && !wasSpaceKeyPressed) {
    input += " ";
    wasSpaceKeyPressed = true;
  } else if (!spaceKey->isPressed) {
    wasSpaceKeyPressed = false;
  }

  deleteKey->checkTouch();
  if (deleteKey->isPressed && !wasDeleteKeyPressed) {
    if (input.length() > 0) {
      input = input.substring(0, input.length() - 1);
      Serial.println(input);
      wasDeleteKeyPressed = true;
    }
  } else if (!deleteKey->isPressed) {
    wasDeleteKeyPressed = false;
  }
}

bool Keyboard::enterClicked(){
  enterKey->checkTouch();
  if (enterKey->isPressed && !wasEnterKeyPressed) {
    wasEnterKeyPressed = true;
    return true;
  } else if (!enterKey->isPressed) {
    wasEnterKeyPressed = false;
    return false;
  }
}
