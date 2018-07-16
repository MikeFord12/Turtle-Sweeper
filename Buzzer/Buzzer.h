#ifndef _BUZZER_h
#define _BUZZER_h

#include <Arduino.h>

#define BUZZER_PIN 8

#define FREQ 3000
#define DURATION 500

void setupBuzzer();

void soundBuzzer();

void soundKimPossibleTheme();

#endif
