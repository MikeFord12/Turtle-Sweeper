#ifndef _GPS_h
#define _GPS_h

//Include Arduino library and the NOEGPS library h files
#include "Arduino.h"
#include <NMEAGPS.h>
#include <GPSport.h>
#include <Streamers.h>

//ISR for serial data processing
//c - char read from serial buffer
static void GPSisr( uint8_t c );

//Setup GPS serial port
void setupGPS();

//Convert UTS timestamp to EST
void adjustTime( NeoGPS::time_t & dt );



#endif
