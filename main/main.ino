#include <SoftwareSerial.h>

#include <PushButtons.h>
#include <RFID_Drivers.h>
#include <SparkFun_UHF_RFID_Reader.h>
#include <SD_Drivers.h>
#include <LCD.h>
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include <SPI.h>
#include <SD.h>
#include <NeoHWSerial.h>
#include <NeoHWSerial_private.h>
#include <NMEAGPS.h>
#include <LEDs.h>
#include <GPS.h>
#include <VoltageReader.h>
#include "Globals.h"


//GPS object and fix object
NMEAGPS gps;
gps_fix fix;
//RFID object
RFID nano;
SoftwareSerial softSerial(12, 13); //RX, TX
//time tracking variables for updating time and checking batteries
int previousMinute = 0;
int previousBatteryCheckTime = 0;

// variable to keep track of button press
int buttonSelect = BUTTON_NONE;

// current voltage
double voltage;

//Program state variable
int STATE;

//display variable
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

//Data file object
File myFile;

void setup() {

  //Initialize RFID Reader, LCD, GPS, LEDS
  //TODO: add initialization error checking
  setupLEDS();
  setupGPS();
  setupLCD();
  setupNano(38400);
  setupCommunication();
  setupPushButtons();
   initializeSDCard();

  //TODO: Maybe modify or think through logic a little more
  //If battery is very low on powerup, display low battery and do not let the system go further until they replace or recharge battery
  if(getVoltage()<= 6.4)
  {
    drawCriticalBatteryScreen();
    while(1);
  }
  

  //initialize state to main screen
  STATE = MAIN_SCREEN;

  //Use for printing out to serial monitor
  //Can remove after debugging is no longer needed
  DEBUG_PORT.begin(9600);
  DEBUG_PORT.flush();
}

void loop() {
  // put your main code here, to run repeatedly:

  //code if system is in main state
  if (STATE == MAIN_SCREEN)
  {
    //LED = Green and display main screen
    displayGreen();
    drawMainScreen();

    //Begin scanning for tags
    nano.startReading();

    //Loop checking for high signal from RFID reader
    if (nano.check() == true) //Check to see if any new data has come in from module
    {
      byte responseType = nano.parseResponse(); //Break response into tag ID, RSSI, frequency, and timestamp

      //only check if we found a tag, dont care about anything else
      if (responseType == RESPONSE_IS_TAGFOUND)
      {
        STATE = DETECTION_SCREEN;
      }
    }

    //TODO: Design system to guard against repeat detections

    //Check time to see if we need to update display screen (if Minute changed)
    if (gps.available())
    {
      //Read Fix
      fix = gps.read();
      //If the minute has changed since last read
      if (fix.dateTime.minutes != previousMinute && fix.valid.time)
      {
        //update with new minute and write the new time to LCD
        previousMinute = fix.dateTime.minutes;
        writeTime(fix.dateTime.hours, fix.dateTime.minutes);
      }
    }

    //If we havent checked the battery charge in over 10 minutes
    if (millis() - previousBatteryCheckTime >= TEN_MINUTES_IN_MS)
    {
      //update new battery check time
      previousBatteryCheckTime = millis();

      //check battery charge
      voltage = getVoltage();
      //TODO: figure out what to display when voltage is the same until 6.8V
      //TODO: void writeCharge(int charge);
    }
  }

  //code if turtle found
  if (STATE == DETECTION_SCREEN)
  {
    //initilize detection event structure
    detectionEventInfo detectionEvent;
    char dateString[50];
    String myEPC = String();

    //LED = Red
    displayRed();

    //TODO: Implement sounding buzzer
    

    //TODO: This code is untested. See if it actually pulls out the tag ID
     byte tagEPCBytes = nano.getTagEPCBytes(); //Get the number of bytes of EPC from response
      //Print EPC bytes, this is a subsection of bytes from the response/msg array
      for (byte x = 0 ; x < tagEPCBytes ; x++)
      {
        if (nano.msg[31 + x] < 0x10)
        {
          myEPC.concat("0"); //Pretty print
        }
        
        myEPC.concat(nano.msg[31 + x]);
        //NeoSerial.print(nano.msg[31 + x], HEX);
        //myEPC.concat(" ");
        //NeoSerial.print(F(" "));
      }
      //set tag ID into struct
      detectionEvent.tagID = myEPC.toInt();

    //Wait for next GPS input and parse time and coordinates out and save to detectionEvent struct
    while (!gps.available());
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

        //Format date string and save
        sprintf(dateString, "%d:%d:%d %d-%d-%d", fix.dateTime.hours, fix.dateTime.minutes, fix.dateTime.seconds, fix.dateTime.month, fix.dateTime.date, fix.dateTime.year);

        detectionEvent.timeStamp = dateString;
      }



      //After data collection, Draw Screen
      drawDetectionScreen(detectionEvent.tagID, detectionEvent.timeStamp,detectionEvent.longitude,detectionEvent.latitude);

      //TODO: Wait for either Yes or no to log data (check input push buttons)
      while ((buttonSelect = buttonPressed()) != BUTTON_SELECT)
      {
        switch (buttonSelect)
        {
          case (BUTTON_LEFT):
            drawYesSelection();
            break;
          case (BUTTON_RIGHT):
            drawNoSelection();
            break;
        }
      }

      if (optionSelected() == YES_SELECTED)
      {
        //If Yes: Log to SD card
        if (!(logDetectionEvent(detectionEvent.tagID, detectionEvent.timeStamp, detectionEvent.latitude, detectionEvent.longitude)))
        {
          DEBUG_PORT.println("Error logging to SD Card");
        }
      }
      else
      {
        //If No: Go back to MAIN_SCREEN state
        STATE = MAIN_SCREEN;
      }
    }

  }
}

