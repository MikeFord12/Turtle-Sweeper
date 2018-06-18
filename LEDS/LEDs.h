#ifndef _LEDs_h
#define _LEDs_h

#include <Arduino.h>

//LED Pins
#define RED_LED_PIN 5
#define GREEN_LED_PIN 6

//Initialize Pins
void setupLEDS();

//Turn LED Red
void displayRed();

//Turn LED Green
void displayGreen();

#endif
