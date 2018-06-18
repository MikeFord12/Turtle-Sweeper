#include <SPI.h>
#include <SD.h>
#include <NeoHWSerial.h>
#include <NeoHWSerial_private.h>
#include <NMEAGPS.h>
#include <LEDs.h>
#include <GPS.h>
#include "Globals.h"

//GPS object and fix object
NMEAGPS gps;
gps_fix fix;
//Program state variable
int STATE;

void setup() {
  //setup LED pins and GPS communication
  setupLEDS();
  setupGPS();
  //initialize state to main screen
  STATE = MAIN_SCREEN;

  //Use for printing out to serial monitor
  //Can remove after debugging is no longer needed
  DEBUG_PORT.begin(9600);
  DEBUG_PORT.flush();
}

void loop() {
  // put your main code here, to run repeatedly:

  //If a new fix is available to read
  if (gps.available()) {
    //Read Fix
    fix = gps.read();
    //If location vaild
    if (fix.valid.location)
    {
      //Print out long and lat
      //Eventually save these if turtle found, no need to print out
      DEBUG_PORT.println(fix.latitudeL());
      DEBUG_PORT.println(fix.longitudeL());
    }
    //If time stamp is valid
    if (fix.valid.time && fix.valid.date)
    {
      //Adjust time to EST
      adjustTime(fix.dateTime);
      //Convert from military time to conventional
      fix.dateTime.setAMorPM();

      //Print out time
      //Eventually save these if turtle found, no need to print out
      DEBUG_PORT << fix.dateTime;
      DEBUG_PORT.print(" "); DEBUG_PORT.println(fix.dateTime.AMorPM);
    }
  }
  //see if GPS overrun
  if (gps.overrun())
  {
    gps.overrun( false );
    DEBUG_PORT.println( F("DATA OVERRUN: took too long to print GPS data!") );
  }

  //code if system is in main state
  if (STATE == MAIN_SCREEN)
  {

  }
  //code if turtle found
  if (STATE == DETECTION_SCREEN)
  {

  }

}

