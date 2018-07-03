#ifndef _SD_Drivers_h
#define _SD_Drivers_h

#include "Arduino.h"
#include <SD.h>
#include <NeoHWSerial.h>

//SD Card
#define SD_CARD_SLAVE_SELECT_PIN 4
//extern char* dataFile;
extern File myFile;;

struct detectionEventInfo
{
        String tagID;
        String timeStamp;
        float latitude;
        float longitude;
};

//Sets up SD card communication and data file
int initializeSDCard();

//Constructs string and logs data based on parameters passed in
int logDetectionEvent(String idNum, String timeStamp, float Lat, float Long);


//File object for data file
extern File detectionData;

#endif
