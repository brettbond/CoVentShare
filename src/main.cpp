#include <Arduino.h>
#include <Wire.h>
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "breather.h"

// NOTE: WROVER_KIT_LCD.cpp modified for LiveSpark torch board pin mapping
// Here are the pins we're using
// #define WROVER_CS               5
// #define WROVER_DC               3
// #define WROVER_RST              15
// #define WROVER_SCLK             18
// #define WROVER_MOSI             23
// #define WROVER_MISO             19
// #define WROVER_BL               1

#include "WROVER_KIT_LCD.h"

#include <Fonts/FreeSansBold18pt7b.h>
#include <Fonts/FreeSansBoldOblique24pt7b.h>

#define WROVER_BL                       1
#define SERIAL2_RXPIN                   16
#define SERIAL2_TXPIN                   17
#define PRESSURE_ANALOG_PIN             32
#define LEVEL_SHIFTER_OUTPUT_ENABLE_PIN 25

WROVER_KIT_LCD tft;
int16_t screenWidth = 0, screenHeight = 0;

void initDisplay() {
  tft.begin();

  // Landscape
  tft.setRotation(1);

  // turn the backlight on
  digitalWrite(WROVER_BL, HIGH);

  // get the screen total width for later use
  screenWidth = tft.width();
  screenHeight = tft.height();
}

// Use a large font to display a headine at the top of the screen
void displayHeadlineText(String headline) {
  tft.setCursor(25, 50);
  tft.setFont(&FreeSansBold18pt7b);
  tft.setTextSize(1);
  tft.println(headline);
}

// Draw a pressure value in the center of the screen followed by a cmH2O units label
void displayCenteredText(String message, uint32_t color, float fontSize) {
  tft.setFont(&FreeSansBoldOblique24pt7b);
  tft.setTextSize(2);
  tft.setTextColor(color);
  tft.setTextWrap(true);
  tft.setCursor(25, 150);
  tft.println(message);
  tft.setFont(NULL);
  tft.setCursor(25, 175);
  // units labels
  tft.println("lpm");
}

void flashMessage(String message) {
  static String lastMessage;

  if(message == lastMessage) {
    return;
  }
  // not double-buffered, fill in the last message with background color
  displayCenteredText (lastMessage, ILI9341_BLUE, 1.1);
  lastMessage = message;
  // then draw the new mesage
  displayCenteredText (message, ILI9341_WHITE, 1.1);
}

Breather *g_breather = NULL;
Breather *getBreather() {
  if(g_breather == NULL) {
    Serial.println("Creating Breather.");
    g_breather = new Breather(10);
  }
  return g_breather;
}

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, SERIAL2_RXPIN, SERIAL2_TXPIN);
  pinMode(PRESSURE_ANALOG_PIN, INPUT);
  pinMode(LEVEL_SHIFTER_OUTPUT_ENABLE_PIN, OUTPUT);
  digitalWrite(LEVEL_SHIFTER_OUTPUT_ENABLE_PIN, HIGH);

  initDisplay();
  tft.fillScreen(ILI9341_BLUE);
  displayHeadlineText("Flow Rate: ");
}

boolean breathing = false;
void loop() {
  // uint32_t reading = map(analogRead(PRESSURE_ANALOG_PIN), 0, 4000, 40, 0);
  if(!breathing) {
    getBreather()->start();
    breathing = true;
  }
  uint8_t flowRate = getBreather()->breathe();
  flashMessage(String(flowRate));

 usleep(100000);
}
