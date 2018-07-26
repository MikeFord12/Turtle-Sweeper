#ifndef _RFID_Drivers_h
#define _RFID_Drivers_h

#include "Arduino.h"
#include <SparkFun_UHF_RFID_Reader.h>
#include <NeoHWSerial.h>
#include "SoftwareSerial.h"


extern RFID nano;
extern SoftwareSerial softSerial;

boolean setupNano(long baudRate);

int setupCommunication();
int setupCommunicationWriteMode();
int writeToTag(int idToWrite);


#endif
