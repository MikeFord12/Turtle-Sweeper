#ifndef _RFID_Drivers_h
#define _RFID_Drivers_h

#include "Arduino.h"
#include <SparkFun_UHF_RFID_Reader.h>
#include <NeoHWSerial.h>


extern RFID nano;

boolean setupNano(long baudRate);

void setupCommunication();


#endif
