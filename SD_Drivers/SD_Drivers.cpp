#include "SD_Drivers.h"



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
        NeoSerial.print("Initializing SD card...");

        // see if the card is present and can be initialized:
        if (!SD.begin(SD_CARD_SLAVE_SELECT_PIN))
        {
                //Error initializing card
                NeoSerial.println("SD Card failed, or not present");
                return 0;
        }

        //If card initialized correctly, check for or create data file
        NeoSerial.println("Card initialized. Checking for DetectionEventData.csv");

        if (SD.exists(DATAFILE))
        {
                //If file already found, initialization complete.
                NeoSerial.println("Data file found.");
        }
        else
        {
                NeoSerial.println("DetectionEventData.csv not found. Creating File");
                //create csv file
                detectionData = SD.open(DATAFILE);

                if (detectionData)
                {
                        //Add row headers
                        logHeader();
                        detectionData.close();
                }
                else
                {
                        //Error opening data file, return false
                        NeoSerial.println("Error creating data file");
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
int logDetectionEvent(int idNum, String timeStamp, float Lat, float Long)
{
File detectionData;
        // make a string for assembling the data to log:
        String dataString = "";
        // convert to CSV
        dataString = String(idNum,DEC) + "," + timeStamp + "," + String(Lat,7) + "," + String(Long,7);

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
                NeoSerial.println("Error writing to file !");
                return 0;
        }
}

int logHeader()
{
	File detectionData;
        // make a string for assembling the data to log:
        String dataString = "";
        // convert to CSV
        dataString = "Tag ID,Time,Latitude,Longitude";

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
                NeoSerial.println("Error writing to file !");
                return 0;
        }
}
