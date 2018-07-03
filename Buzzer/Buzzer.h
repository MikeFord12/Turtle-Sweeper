#ifndef _BUZZER_h
#define _BUZZER_h

#include <Arduino.h>

#define BUZZER_PIN 42

#define FREQ 1000
#define DURATION 500

void setupBuzzer();

void soundBuzzer();

#endif