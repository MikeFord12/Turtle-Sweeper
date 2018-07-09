#include "SD_Drivers.h"



/**
 * Function: initializeSDCard()
 *------------------
 * Initializes SD card communication and data file.
 *
 * Return: 1: if SD card is present and file created successfully
 *         0: SD card cannot intialize properly
 */
int initializeSDCard()
{
        NeoSerial.print("Initializing SD card...");

        if (!SD.begin(SD_CARD_SLAVE_SELECT_PIN))
        {
                NeoSerial.println("initialization failed!");
                return 1;
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
                myFile.println("Tag ID,Date,Time,Latitude,Longitude");
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
		
	return 0;
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
int logDetectionEvent(char idNum[], char timeS[], char dateS[], float Lat, float Long)
{
        File detectionData;
        // make a string for assembling the data to log:
        char dataString[100];
        char longStr[11];
        char latStr[11];
        dtostrf(Lat,10,7,latStr);
        dtostrf(Long,10,7,longStr);
        //  NeoSerial.print(Lat,7);
        //NeoSerial.print(Long,7);
        // convert to CSV
        //NeoSerial.printf("%07f  %07f\n",Lat,Long );
        //  dataString = idNum + "," + timeStamp + "," + String(Lat,7) + "," + String(Long,7);
        sprintf(dataString,"%s,%s,%s,%s,%s",idNum,dateS,timeS,latStr,longStr);
        //dataString = String(idNum)+","+String(timeStamp);
        NeoSerial.println(dataString);
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
