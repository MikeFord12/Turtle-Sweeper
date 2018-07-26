#ifndef _SD_Drivers_h
#define _SD_Drivers_h

#include "Arduino.h"
#include <SD.h>
#include <NeoHWSerial.h>

//SD Card
#define SD_CARD_SLAVE_SELECT_PIN 4
//extern char* dataFile;
extern File myFile;

struct detectionEventInfo
{
        char tagID[50];
        char timeS[20];
        char dateS[20];
        long latitude;
        long longitude;
};

//Sets up SD card communication and data file
int initializeSDCard();

//Constructs string and logs data based on parameters passed in
int logDetectionEvent(char idNum[], char timeS[], char dateS[], long Lat, long Long);


//File object for data file
extern File detectionData;

#endif
