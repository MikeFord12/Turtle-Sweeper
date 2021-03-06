#ifndef _PushButtons_h
#define _PushButtons_h

#include <Arduino.h>
#include <string.h>
#include "NeoHWSerial.h"

// PushButton Pins
#define LEFT_BUTTON_PIN 30
#define RIGHT_BUTTON_PIN 32
#define SELECT_BUTTON_PIN 36

#define BUTTON_NONE 0
#define BUTTON_LEFT 1
#define BUTTON_RIGHT 2
#define BUTTON_SELECT 3

// Timeout for no button selection
#define BUTTON_TIMEOUT 10000

// Set delay time
#define DEBOUNCE_ITERATIONS 5

//Initialize Pins
void setupPushButtons();

//Determine if button was pressed
int buttonPressed();

#endif
