#include "LEDs.h"

/**
 * Function: setupLEDS()
 *---------------------
 * Sets the LED pins defined in LEDs.h as outputs
 *
 * Parameters: None
 * Return: None
 */
void setupLEDS()
{
        pinMode(RED_LED_PIN, OUTPUT);
        pinMode(GREEN_LED_PIN, OUTPUT);
}

/**
 * Function: displayRed()
 *---------------------
 * Set LED to glow red. Turn Green pin OFF in case it is on.
 *
 * Parameters: None
 * Return: None
 */
void displayRed()
{
        digitalWrite(RED_LED_PIN, HIGH);
        digitalWrite(GREEN_LED_PIN, LOW);
}

/**
 * Function: displayGreen()
 *---------------------
 * Set LED to glow green. Turn red pin OFF in case it is on.
 *
 * Parameters: None
 * Return: None
 */
void displayGreen()
{
        digitalWrite(RED_LED_PIN, LOW);
        digitalWrite(GREEN_LED_PIN, HIGH);
}

/**
 * Function: TurnOff()
 *---------------------
 * Turns off both red and green LED pins
 *
 * Parameters: None
 * Return: None
 */
void turnOff()
{
        digitalWrite(RED_LED_PIN, LOW);
        digitalWrite(GREEN_LED_PIN, LOW);
}
