#include "PushButtons.h"

// Flag so that button is not "repeatedly pressed"
int buttonResetFlag = 0;

/**
   Function: setupPushButtons()
   ---------------------
   Sets the push button pins as defined in PushButtons.h

   Parameters: None
   Return: None
 */
void setupPushButtons()
{
        pinMode(LEFT_BUTTON_PIN, INPUT);
        pinMode(RIGHT_BUTTON_PIN, INPUT);
        pinMode(SELECT_BUTTON_PIN, INPUT);
}

/**
   Function: buttonPressed()
   ---------------------
   Determines if push button was pressed, returns associated value based on any buttons pressed

   Parameters: None
   Return: 0 if no buttons pressed, associated button value if pressed
 */
int buttonPressed()
{
        // Variables for current and previous button reads
        int i, reading, previousReading;

        // Reset readings before each new read
        reading = BUTTON_NONE;
        previousReading = BUTTON_NONE;

        // Iterate debounce states as many times as defined in PushButtons.h
        for (i = 0; i < DEBOUNCE_ITERATIONS; i++)
        {
                // Determine if select button is pressed, and no other buttons are pressed
                if (digitalRead(SELECT_BUTTON_PIN) &&
                    !(digitalRead(LEFT_BUTTON_PIN) || digitalRead(RIGHT_BUTTON_PIN)))
                {
                        reading = BUTTON_SELECT;
                }
                // Determine if left button is pressed, and no other buttons are pressed
                else if (digitalRead(LEFT_BUTTON_PIN) &&
                         !(digitalRead(SELECT_BUTTON_PIN) || digitalRead(RIGHT_BUTTON_PIN)))
                {
                        reading = BUTTON_LEFT;
                }
                // Determine if right button is pressed, and no other buttons are pressed
                else if (digitalRead(RIGHT_BUTTON_PIN) &&
                         !(digitalRead(SELECT_BUTTON_PIN) || digitalRead(LEFT_BUTTON_PIN)))
                {
                        reading = BUTTON_RIGHT;
                }
                // Determine if no button is pressed
                else
                {
                        // Enable another pressing of button, return no button pressed
                        buttonResetFlag = 0;
                        return BUTTON_NONE;
                }

                // If reading does not match previous reading, exit method
                if (reading != previousReading && previousReading != BUTTON_NONE)
                {
                        // Enable another pressing of button, return no button pressed
                        buttonResetFlag = 0;
                        return BUTTON_NONE;
                }

                // Set current reading to previous for next iteration
                previousReading = reading;
        }

        if(!buttonResetFlag)
        {
                // Disable another pressing of button, return button pressed
                buttonResetFlag = 1;
                return reading;
        }
        else
        {
                return BUTTON_NONE;
        }
}
