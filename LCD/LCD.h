#ifndef _LCD_h
#define _LCD_h

#include <Arduino.h>
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "NeoHWSerial.h"

// For the Adafruit shield, these are the default.
#define TFT_DC 9
#define TFT_CS 10
#define TFT_RST 11

// Color definitions
#define BLACK    0x0000
#define BLUE     0x001F
#define RED      0xF800
#define GREEN    0x07E0
#define CYAN     0x07FF
#define MAGENTA  0xF81F
#define YELLOW   0xFFE0
#define WHITE    0xFFFF

#define NO_SELECTED 0
#define YES_SELECTED 1

extern Adafruit_ILI9341 tft;



void drawCriticalBatteryScreen();
int drawDetectionScreen(char ID[], char timeStamp[], float Long, float Lat);
void drawInitializationScreen();
void drawMainScreen(int noGPS, int noSD);
void drawTurtlesFound(int turtlesFound);
void setupLCD();
void setupText(uint16_t xCoord,uint16_t yCoord, uint16_t color, uint8_t fontSize);
void writeCharge(int charge);
void drawYesSelection();
void drawNoSelection();
int optionSelected();
void drawErrorScreen(int errorCode);
void gatheringGPSScreen();

#endif
