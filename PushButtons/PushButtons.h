#ifndef _PushButtons_h
#define _PushButtons_h

#include <Arduino.h>
#include <string.h>

// PushButton Pins
#define UP_BUTTON_PIN 38
#define DOWN_BUTTON_PIN 39
#define LEFT_BUTTON_PIN 40
#define RIGHT_BUTTON_PIN 41

#define BUTTON_NONE 0
#define BUTTON_UP 1
#define BUTTON_DOWN 2
#define BUTTON_LEFT 3
#define BUTTON_RIGHT 4

// Set delay time
#define DEBOUNCE_ITERATIONS 5

//Initialize Pins
void setupPushButtons();

//Determine if button was pressed
int buttonPressed();

#endif
