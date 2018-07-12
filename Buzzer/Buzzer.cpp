#include "Buzzer.h"

void setupBuzzer()
{
	pinMode(BUZZER_PIN, OUTPUT);
}

void soundBuzzer()
{
	tone(BUZZER_PIN, FREQ, DURATION);
}

void soundKimPossibleTheme()
{
	tone(BUZZER_PIN, 3126, 150); // Send 1KHz sound signal...   
	delay(250);        // ...for 1 sec   
	tone(BUZZER_PIN, 3126, 150);     // Stop sound...   
	delay(250);        // ...for 1sec   
	tone(BUZZER_PIN, 3729, 100);     // Stop sound...   
	delay(150);   
	tone(BUZZER_PIN, 3126, 100);     // Stop sound...   
	delay(1500);
}