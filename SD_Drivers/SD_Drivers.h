#ifndef _SD_Drivers_h
#define _SD_Drivers_h

#include "Arduino.h"
#include <SD.h>

//SD Card
#define SD_CARD_SLAVE_SELECT_PIN 53
#define DATAFILE "DetectionEventData.csv"

//Sets up SD card communication and data file
int initializeSDCard();

//Constructs string and logs data based on parameters passed in
int logDetectionEvent(String idNum, String timeStamp, String GPSCoords);

//File object for data file
extern File detectionData;

#endif
