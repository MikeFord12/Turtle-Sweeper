#include <SparkFun_UHF_RFID_Reader.h>
#include <Buzzer.h>
#include <RFID_Drivers.h>
#include <SoftwareSerial.h>
#include <PushButtons.h>
#include <SPI.h>
#include <SD.h>
#include <NeoHWSerial.h>
#include <NeoHWSerial_private.h>
#include <NMEAGPS.h>
#include <LEDs.h>
#include <GPS.h>
#include <VoltageReader.h>
#include "Globals.h"
#include <LCD.h>
#include <SD_Drivers.h>
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

//GPS object and fix object
NMEAGPS gps;
gps_fix fix;

//log of turtles found
char tagIDS[20][50];
int turtlesFound = 0;

//RFID object
RFID nano;
SoftwareSerial softSerial(12, 13); //RX, TX

//time tracking variables for updating time and checking batteries
unsigned long previousBatteryCheckTime = 0;

//Timeout for waiting for GPS fix at start of program
unsigned long GPS_FIX_TIMEOUT = 0;
//Timeout for GPS gathering data when we have found a tag
unsigned long GPS_GATHERING_DATA_TIMEOUT = 0;
//Flag for GPS timeout out when we try to collect data after we find a tag
int GPS_GATHERING_TIMEOUT = 0;

// LED toggling variables
unsigned long TOGGLE_LED_TIMER = 0;
int LED_TOGGLE = 1;

// Variable for no select button timeout
unsigned long BUTTON_SELECT_TIMEOUT = 0;

//Variables for GPS and SD initialization
int SD_INITIALIZED_CORRECTLY = 1;
int GPS_INITIALIZED_CORRECTLY = 1;

// variable to keep track of which button press
int buttonSelect = BUTTON_NONE;

// variable to determine which mode the system is in
int isInReadingMode = 0;

// current voltage
double voltage;

//Program state variable
//STATE can be GPS_FIX, MAIN_SCREEN, or DETECTION_SCREEN
int STATE;

//display object
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

//Data file object
File myFile;

//ID to write to tag if User chooses WRITE mode
int tagIDToWrite = 0;


void setup() {
  //Serial port to print out debug messages (dont need if not printing anything out)
  NeoSerial.begin(115200);
  while (!NeoSerial);

  //variable to keep track of which module failed in initialization
  int statusCode = 0;

  //initialize LCD and Buttons for mode selection
  setupLCD();
  NeoSerial.println("LCD initalized");

  setupPushButtons();
  NeoSerial.println("Buttons initalized");


  // Ask user for read/write mode
  drawModeSelectScreen();
  //Loop until user presses SELECT pin
  while ((buttonSelect = buttonPressed()) != BUTTON_SELECT)
  {
    switch (buttonSelect)
    {
      case (BUTTON_LEFT):
        //Move arrow on screen to yes
        drawYesSelection();
        break;
      case (BUTTON_RIGHT):
        //Move arrow on screen to no
        drawNoSelection();
        break;
    }
  }

  //Because READ is on the left side of the screen and WRITE is on the right
  //They are where YES and NO were for other screens.
  //YES_SELECTED means the user chose READ, NO_SELECTED means user chose WRITE
  if (optionSelected() == YES_SELECTED)
  {
    isInReadingMode = 1;
  }
  else
  {
    isInReadingMode = 0;
  }

  //If user chose reading mode, initialize GPS, LEDS, Reader in Read mode, Buzzer, and SD card
  if (isInReadingMode)
  {

    setupLEDS();
    //NeoSerial.println("LEDS initalized");
    setupGPS();
    //NeoSerial.println("GPS initalized");

    //initialize reader
    if (statusCode = setupCommunication())
    {
      drawErrorScreen(statusCode);
      while (1);
    }
    else
    {
      NeoSerial.println("RFID initalized");
    }

    //initialize buzzer
    setupBuzzer();
    NeoSerial.println("Buzzer initialized");

    //initialize SD card
    if (statusCode = initializeSDCard())
    {
      //If SD is not initialized correctly
      SD_INITIALIZED_CORRECTLY = 0;
      drawErrorScreen(statusCode);
      delay(1000);
    }
    else
    {
      NeoSerial.println("SD initalized");
    }

    //If battery is very low on powerup, display low battery and do not let the system go further until they replace or recharge battery
    if (getBatteryPercentage() <= 5)
    {
      drawCriticalBatteryScreen();
      while (1);
    }

    //Enter into state that tries to obtain GPS fix
    STATE = GPSFIX_SCREEN;

    //Stay in while until we get a valid GPS fix
    drawInitializationScreen();
    GPS_FIX_TIMEOUT = millis();

    while (STATE == GPSFIX_SCREEN)
    {
      if (gps.available())
      {
        //Read Fix
        fix = gps.read();

        if (fix.valid.time && fix.valid.location)
        {
          //If we got a fix with valid time and location, proceed
          STATE = MAIN_SCREEN;
        }
      }

      //If we have waited 6 minutes and there is still no fix, disable GPS and logging functionality
      //The user can also press SELECT to bypass this initialization, system will proceed with no GPS ability
      if (millis() - GPS_FIX_TIMEOUT > SIX_MINUTES_IN_MS || (buttonPressed() == BUTTON_SELECT))
      {
        GPS_INITIALIZED_CORRECTLY = 0;
        STATE = MAIN_SCREEN;
      }
    }
  }
  //If user chose to go to Write mode
  else if (!isInReadingMode)
  {
    //initialize reader in Write mode
    if (statusCode = setupCommunicationWriteMode())
    {
      drawErrorScreen(statusCode);
      while (1);
    }
  }
}

void loop() {

  //if user chose READ mode
  if (isInReadingMode)
  {

    //code if system is in main state
    if (STATE == MAIN_SCREEN)
    {
      //reset turtle already logged
      int turtleAlreadyLogged = 0;

      //display green LED and draw main screen
      displayGreen();
      drawMainScreen(GPS_INITIALIZED_CORRECTLY, SD_INITIALIZED_CORRECTLY);
      drawTurtlesFound(turtlesFound);
      writeCharge(getBatteryPercentage());

      //if gps fix is available, read it in
      if (GPS_INITIALIZED_CORRECTLY)
      {
        if (gps.available())
        {
          //Read Fix
          fix = gps.read();
        }
      }

      //Begin scanning for tags
      nano.startReading();

      while (STATE == MAIN_SCREEN)
      {
        char myEPC[50] = "";

        //Toggle LED green every 2 seconds to ensure system is not frozen
        if (LED_TOGGLE)
        {
          displayGreen();
        }
        else
        {
          turnOff();
        }

        if (millis() - TOGGLE_LED_TIMER >= TWO_SECONDS_IN_MS)
        {
          TOGGLE_LED_TIMER = millis();

          LED_TOGGLE = !LED_TOGGLE;
        }


        //Check and update batery display every 1 minute
        if (millis() - previousBatteryCheckTime >= ONE_MINUTE_IN_MS)
        {
          //update new battery check time
          previousBatteryCheckTime = millis();

          //check battery charge
          writeCharge(getBatteryPercentage());
        }
        //if battery is critically low
        if (getBatteryPercentage() <= 5)
        {
          nano.stopReading();
          drawCriticalBatteryScreen();
          while (1);
        }

        //if GPS enabled and gps fix is available, read it in
        if (GPS_INITIALIZED_CORRECTLY)
        {
          if (gps.available())
          {
            //Read Fix
            fix = gps.read();
          }
        }

        //Loop checking for high signal from RFID reader
        if (nano.check() == true) //Check to see if any new data has come in from module
        {
          byte responseType = nano.parseResponse(); //Break response into tag ID, RSSI, frequency, and timestamp

          //only check if we found a tag, dont care about anything else
          if (responseType == RESPONSE_IS_TAGFOUND)
          {
            //sound buzzer is tag found
            soundBuzzer();

            byte tagEPCBytes = nano.getTagEPCBytes(); //Get the number of bytes of EPC from response
            //Store EPC bytes, this is a subsection of bytes from the response/msg array
            for (byte x = 0 ; x < tagEPCBytes ; x++)
            {
              char arrayByte[3];

              if (nano.msg[31 + x] < 0x10)
              {
                strcat(myEPC, "0"); //if hex value is only one digit, put a 0 in front
              }
              //convert int to char array
              itoa (nano.msg[31 + x], arrayByte, 10);
              //add it to the end of our ID char array
              strcat(myEPC, arrayByte);

              strcat(myEPC, " "); //put space in between each byte
            }

            //See if we already found tag
            for (int i = 0; i < turtlesFound; i++)
            {
              if (!strcmp(myEPC, tagIDS[i]))
              {
                //if we have, dont treat it as a new detection event
                turtleAlreadyLogged = 1;
                break;
              }
            }
            //If tag found for the first time
            if (!turtleAlreadyLogged)
            {
              //If GPS is working, go to detection state
              //If not, no point in going as we cant get coordinates or timestamp
              if (GPS_INITIALIZED_CORRECTLY)
              {
                STATE = DETECTION_SCREEN;
              }
              
              //add turtle to log
              strcpy(tagIDS[turtlesFound] , myEPC);

              //if neither initialized, show user the tag found, user must press select to advance
              if (!GPS_INITIALIZED_CORRECTLY && !SD_INITIALIZED_CORRECTLY)
              {
                displayRed();
                //Detection screen with no Long/Lat or timestamp data
                drawBasicDetectionScreen(myEPC);
                BUTTON_SELECT_TIMEOUT = millis();
                //Wait until user presses select to move past basic detection screen
                while ((buttonSelect = buttonPressed() == BUTTON_NONE) && (millis() - BUTTON_SELECT_TIMEOUT < BUTTON_TIMEOUT));

                //If they hit select draw main screen again
                drawMainScreen(GPS_INITIALIZED_CORRECTLY, SD_INITIALIZED_CORRECTLY);
                writeCharge(getBatteryPercentage());
              }

              //if no GPS but there is SD, log the tag ID found
              if (!GPS_INITIALIZED_CORRECTLY && SD_INITIALIZED_CORRECTLY)
              {
                displayRed();
                drawBasicDetectionScreen(myEPC);
                BUTTON_SELECT_TIMEOUT = millis();
                while ((buttonSelect = buttonPressed() == BUTTON_NONE) && (millis() - BUTTON_SELECT_TIMEOUT < BUTTON_TIMEOUT));

                //After they hit select, log data and go back to main screen
                logDetectionEvent(myEPC, "GPS Data Timeout", "GPS Data Timeout", 1234567, 1234567);
                drawMainScreen(GPS_INITIALIZED_CORRECTLY, SD_INITIALIZED_CORRECTLY);
                writeCharge(getBatteryPercentage());
              }

              //update number of turtles found
              turtlesFound++;
              drawTurtlesFound(turtlesFound);
            }
            //reset flag
            turtleAlreadyLogged = 0;
          }
        }
      }
    }

    //code if turtle found
    if (STATE == DETECTION_SCREEN)
    {
      //NeoSerial.println("Detection state");
      //initilize detection event structure
      detectionEventInfo detectionEvent;
      char timeStamp[25];
      char dateString[25];
      float detectionScreenLong = 0;
      float detectionScreenLat = 0;

      char epcFound[50] = "";

      //Turn LED RED
      displayRed();
      //sound buzzer
      soundBuzzer();

      //set tag ID into struct
      strcpy(detectionEvent.tagID , tagIDS[turtlesFound - 1]);
      sprintf(epcFound, "ID: %s", detectionEvent.tagID);

      //Wait for next GPS input and parse time and coordinates out and save to detectionEvent struct
      gatheringGPSScreen();

      //make sure we have valid GPS data
      if (!fix.valid.location || !fix.valid.time || !fix.valid.date)
      {
        while (!gps.available());
        fix = gps.read();
        GPS_GATHERING_DATA_TIMEOUT = millis();
        //wait until we have valid data from GPS to save
        while (!fix.valid.location || !fix.valid.time || !fix.valid.date)
        {
          if (gps.available())
          {
            //Read Fix
            fix = gps.read();
          }
          //If it takes more than 30 seconds to gather GPS data, proceed
          if ((millis() - GPS_GATHERING_DATA_TIMEOUT) >= THIRTY_SECONDS_IN_MS)
          {
            GPS_GATHERING_TIMEOUT = 1;
            break;
          }
        }
      }

      //If location valid
      if (fix.valid.location)
      {
        //data to log to SD
        detectionEvent.latitude = fix.latitudeL();
        detectionEvent.longitude = fix.longitudeL();
        //data to display to screen
        detectionScreenLong = (detectionEvent.longitude) / (10000000.00);
        detectionScreenLat = (detectionEvent.latitude) / (10000000.00);
      }

      if (fix.valid.time && fix.valid.date)
      {
        //Adjust time to EST
        adjustTime(fix.dateTime);
        //Convert from military time to conventional
        fix.dateTime.setAMorPM();

        //Format date string and time string
        sprintf(timeStamp, "%d:%02d:%02d %02d-%02d-%d", fix.dateTime.hours, fix.dateTime.minutes, fix.dateTime.seconds, fix.dateTime.month, fix.dateTime.date, fix.dateTime.year);
        sprintf(detectionEvent.timeS, "%d:%02d:%02d", fix.dateTime.hours, fix.dateTime.minutes, fix.dateTime.seconds);
        sprintf(detectionEvent.dateS, "%02d-%02d-%d", fix.dateTime.month, fix.dateTime.date, fix.dateTime.year);
        sprintf(dateString, "Time: %s", timeStamp);
      }

      if (!GPS_GATHERING_TIMEOUT)
      {
        //if GPS data collection did not timeout, draw screen with valid data
        drawDetectionScreen(epcFound, dateString, detectionScreenLong, detectionScreenLat);
      }
      else
      {
        //if it did timeout, show screen without valid GPS data
        drawDetectionScreen(epcFound, "GPS Data Timout", 00.000, 00.000);
      }


      //Set button timeout variable
      BUTTON_SELECT_TIMEOUT = millis();

      //Wait for either Yes or no to log data (check input push buttons)
      //Timeout after set time of no button pressed
      while (((buttonSelect = buttonPressed()) != BUTTON_SELECT) &&
             (millis() - BUTTON_SELECT_TIMEOUT < BUTTON_TIMEOUT))
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

      //IF yes is selected and timeout is not met, log data 
      if ((optionSelected() == YES_SELECTED))
      {
        //If Yes and SD card initialized correctly: Log to SD card
        if (SD_INITIALIZED_CORRECTLY)
        {
          if (!GPS_GATHERING_TIMEOUT)
          {
            if (!(logDetectionEvent(detectionEvent.tagID, detectionEvent.timeS, detectionEvent.dateS, detectionEvent.latitude, detectionEvent.longitude)))
            {
              DEBUG_PORT.println("Error logging to SD Card");
            }
          }
          //If GPS timeout
          else
          {
            if (!(logDetectionEvent(detectionEvent.tagID, "GPS Data Timout", "GPS Data Timout", 1234567, 1234567)))
            {
              DEBUG_PORT.println("Error logging to SD Card");
            }
            GPS_GATHERING_TIMEOUT = 0;
          }
        }
        STATE = MAIN_SCREEN;
      }
      else
      {
        //If No: Go back to MAIN_SCREEN state
        STATE = MAIN_SCREEN;
      }
    }
  }

  //If user chose WRITE mode
  if (!isInReadingMode)
  {
    //initialize tagID
    tagIDToWrite = 0;

    //draw number selection screen and initial 0 to it
    drawNumberScreen();

    printDesiredTagValue(tagIDToWrite); //shows 0 on screen

    //if user presses right button, increase tag ID by one
    //if user presses left, decrement tag ID by one
    while ((buttonSelect = buttonPressed()) != BUTTON_SELECT)
    {
      if ((buttonSelect == BUTTON_LEFT) && tagIDToWrite > 0)
      {
        tagIDToWrite--;
        printDesiredTagValue(tagIDToWrite); //decrement variable and value shown on screen
      }

      if ((buttonSelect == BUTTON_RIGHT) && tagIDToWrite < 255)
      {
        tagIDToWrite++;
        printDesiredTagValue(tagIDToWrite);  //increment variable and value shown on screen
      }
    }

    //display are you sure screen
    writeConfirmationScreen(tagIDToWrite);

    //Same logic as our other "yes or no screen"
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
      //if thy chose ys, write to tag the ID they specified
      if (writeToTag(tagIDToWrite))
      {
        //display tag successful screen, do you want to write another tag
        drawWriteSuccessful();
      }
      else
      {
        //display tag not written successfully do you want to try again
        drawWriteFailure();
      }
    }
    //if no go back to number input screen
    else
    {
      isInReadingMode = 0;
    }
  }
}

