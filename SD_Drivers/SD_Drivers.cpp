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
int logDetectionEvent(char idNum[], char timeS[], char dateS[], long Lat, long Long)
{

        File detectionData;
        // make a string for assembling the data to log:
        int make_room_at_lat = 2;
        int make_room_at_long = 3;
        int room_to_make = 1;
        char dataString[120];
        char longStr[15];
        char latStr[15];
        //  NeoSerial.print("Lat: ");
        //    NeoSerial.println(Lat);
        //  NeoSerial.print("Long: ");
        //  NeoSerial.println(Long);

        ltoa(Lat,latStr,10);
        ltoa(Long,longStr,10);

        memmove(
                longStr + make_room_at_long + room_to_make,
                longStr + make_room_at_long,
                15 - (make_room_at_long + room_to_make)
                );
        longStr[make_room_at_long] = '.';


        memmove(
                latStr + make_room_at_lat + room_to_make,
                latStr + make_room_at_lat,
                15 - (make_room_at_lat + room_to_make)
                );
        latStr[make_room_at_lat] = '.';



        sprintf(dataString,"%s,%s,%s,%s,%s",idNum,dateS,timeS,latStr,longStr);

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
