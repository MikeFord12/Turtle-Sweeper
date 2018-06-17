#ifndef _SD_Drivers_h
#define _SD_Drivers_h

//Sets up SD card communication and data file
int initializeSDCard();

//Constructs string and logs data based on parameters passed in
int logDetectionEvent(String idNum, String timeStamp, String GPSCoords);

//File object for data file
extern File detectionData;

#endif
