#include <SD_Drivers.h>

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

  setupLEDS();
  setupGPS();

  //TODO
  //Initialize RFID Reader, LCD, SD, and oushbuttons if needed

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
    //LED = Green
    displayGreen();

    //Loop checking for high signal from RFID reader

    //If signal is high, make sure we haven't already detected that turtle

    //Check time to see if we need to update display screen (if Minute changed)

    //TODO: Figure out battery charge

  }
  //code if turtle found
  if (STATE == DETECTION_SCREEN)
  {
    detectionEventInfo detectionEvent;

    //LED = Red
    displayRed();

    //Sound buzzer for 5 seconds?

    //Get/store GPS coords, Timestamp, and tag ID

    //TODO:Get tag ID from RFID reader and save to struct

    if (gps.available())
    {
      //Read Fix
      fix = gps.read();
      //If location vaild
      if (fix.valid.location)
      {
        detectionEvent.latitude = fix.latitudeL();
        detectionEvent.longitude = fix.longitudeL();
      }
      if (fix.valid.time && fix.valid.date)
      {
        //Adjust time to EST
        adjustTime(fix.dateTime);
        //Convert from military time to conventional
        fix.dateTime.setAMorPM();

        //TODO:Convert struct to string and save to detectionEvent
      }



      //Draw Screen

      //Wait for either Yes or no to log data (check input push buttons)

      //If Yes: Log to SD card
      if (!(logDetectionEvent(detectionEvent.tagID, detectionEvent.timeStamp, detectionEvent.latitude, detectionEvent.longitude)))
      {
        DEBUG_PORT.println("Error logging to SD Card");
      }
      //If No: Go back to MAIN_SCREEN state
      STATE = MAIN_SCREEN;

    }

  }
}
