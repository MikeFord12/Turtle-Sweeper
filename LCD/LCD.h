#ifndef _LCD_h
#define _LCD_h

#include <Arduino.h>
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

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

extern Adafruit_ILI9341 tft;


void drawMainScreen();
void drawCriticalBatteryScreen();
void drawDetectionScreenS(String ID, String timeStamp, String longitude, String latitude);

void setupLCD();
void setupText(uint16_t xCoord,uint16_t yCoord, uint16_t color, uint8_t fontSize);
void writeTime(int hours, int minutes);
void writeCharge(int charge);


#endif
