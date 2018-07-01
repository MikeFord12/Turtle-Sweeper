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

        if (!SD.begin(SD_CARD_SLAVE_SELECT_PIN))
        {
                NeoSerial.println("initialization failed!");
                while (1);
        }
        NeoSerial.println("initialization done.");

        if (SD.exists("Data.csv"))
        {
                NeoSerial.println("Data.csv exists.");
        }
        else
        {
                NeoSerial.println("Data.csv doesn't exist.");
                NeoSerial.println("Creating data.csv...");
                myFile = SD.open("Data.csv", FILE_WRITE);
                myFile.println("Tag ID,Time,Latitude,Longitude");
                myFile.close();
        }

// open a new file and immediately close it:


// Check to see if the file exists:
        if (SD.exists("Data.csv"))
        {
                NeoSerial.println("Data.csv exists.");
        } else
        {
                NeoSerial.println("Data.csv doesn't exist.");
        }
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
        dataString = String(idNum) + "," + timeStamp + "," + String(Lat,7) + "," + String(Long,7);

        //open file
        detectionData = SD.open("Data.csv", FILE_WRITE);
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
