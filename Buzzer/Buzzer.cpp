#include "Buzzer.h"

void setupBuzzer()
{
	pinMode(BUZZER_PIN, OUTPUT);
}

void soundBuzzer()
{
	tone(BUZZER_PIN, FREQ, DURATION);
}