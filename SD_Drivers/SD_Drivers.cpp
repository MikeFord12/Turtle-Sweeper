#include "SD_Drivers.h"
#include "Globals.h"


/**
 * Function: initializeSDCard()
 *------------------
 * Initializes SD card communication and data file.
 *
 * Return: 1: if SD card is present and file created successfully
 *         0: If either SD card is not present or file not created successfully
 */
int initializeSDCard()
{
        Serial.print("Initializing SD card...");

        // see if the card is present and can be initialized:
        if (!SD.begin(SD_CARD_SLAVE_SELECT_PIN))
        {
                //Error initializing card
                Serial.println("SD Card failed, or not present");
                return 0;
        }

        //If card initialized correctly, check for or create data file
        Serial.println("Card initialized. Checking for DetectionEventData.csv");

        if (SD.exists(DATAFILE))
        {
                //If file already found, initialization complete.
                Serial.println("Data file found.");
        }
        else
        {
                Serial.println("DetectionEventData.csv not found. Creating File");
                //create csv file
                detectionData = SD.open(DATAFILE);

                if (detectionData)
                {
                        //Add row headers
                        logDetectionEvent("Turtle ID", "Time Stamp", "GPS Coordinates");
                        detectionData.close();
                }
                else
                {
                        //Error opening data file, return false
                        Serial.println("Error creating data file");
                        return 0;
                }
        }
        return 1;
}

/**
 * Function: logDetectionEvent(String idNum, String timeStamp, String GPSCoords)
 *------------------
 * Creates string from data passed in and logs to SD card.
 *
 * idNum: Turtle ID returned from RFID tag
 * timeStamp: GPS timestamp of detection event
 * GPSCoords: GPS coordinates of detection event
 *
 * Return: 1: if data written successfully
 *         0: if data not logged successfully
 */
int logDetectionEvent(String idNum, String timeStamp, String GPSCoords)
{
        // make a string for assembling the data to log:
        String dataString = "";
        // convert to CSV
        dataString = idNum + "," + timeStamp + "," + GPSCoords;

        //open file
        detectionData = SD.open(DATAFILE, FILE_WRITE);
        if (detectionData)
        {
                //write to and close file
                detectionData.println(dataString);
                detectionData.close();
                return 1;
        }
        else
        {
                Serial.println("Error writing to file !");
                return 0;
        }
}
