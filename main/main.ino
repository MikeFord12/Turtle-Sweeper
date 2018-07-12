#include <Buzzer.h>
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
char tagIDS[50][50];
int turtlesFound = 0;

//RFID object
RFID nano;
SoftwareSerial softSerial(12, 13); //RX, TX

//time tracking variables for updating time and checking batteries
int previousMinute = 0;
int previousBatteryCheckTime = 0;
int buttonPressedTimer = 0;

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
  NeoSerial.begin(115200);
  while (!NeoSerial);

  int statusCode = 0;

  //Initialize RFID Reader, LCD, GPS, LEDS
  //TODO: add initialization error checking
  setupLEDS();
  NeoSerial.println("LEDS initalized");
  setupGPS();
  NeoSerial.println("GPS initalized");
  setupLCD();
  NeoSerial.println("LCD initalized");

  if (statusCode = setupCommunication())
  {
    drawErrorScreen(statusCode);
    while (1);
  }
  NeoSerial.println("RFID initalized");
  setupPushButtons();
  NeoSerial.println("Buttons initalized");
  setupBuzzer();
  NeoSerial.println("Buzzer initialized");

  if (statusCode = initializeSDCard())
  {
    drawErrorScreen(statusCode);
    while (1);
  }
  NeoSerial.println("SD initalized");

  //If battery is very low on powerup, display low battery and do not let the system go further until they replace or recharge battery
  if (getBatteryPercentage() <= 15)
  {
    drawCriticalBatteryScreen();
    while (1);
  }



  //initialize state to main screen
  STATE = GPSFIX_SCREEN;

  //Stay in while until we get a valid GPS fix
  drawInitializationScreen();
  while (STATE == GPSFIX_SCREEN)
  {
    if (gps.available())
    {
      //Read Fix
      fix = gps.read();

      if (fix.valid.time && fix.valid.location)
      {
        STATE = MAIN_SCREEN;
      }
    }
  }
}

void loop() {
  // put your main code here, to run repeatedly:

  //code if system is in main state
  if (STATE == MAIN_SCREEN)
  {
    char myEPC[50] = "45-67-766-456-88";
    //LED = Green and display main screen
    int turtleAlreadyLogged = 0;
    displayGreen();
    drawMainScreen();
    if (gps.available())
    {
      //Read Fix
      fix = gps.read();
    }
    if (fix.valid.time)
    {
      writeTime(fix.dateTime.hours, fix.dateTime.minutes);
      adjustTime(fix.dateTime);
      fix.dateTime.setAMorPM();
    }
    //Begin scanning for tags
    nano.startReading();

    while (STATE == MAIN_SCREEN)
    {
      //NeoSerial.println("In while");
      //If we havent checked the battery charge in over 10 minutes
      if (millis() - previousBatteryCheckTime >= TEN_MINUTES_IN_MS)
      {
        //update new battery check time
        previousBatteryCheckTime = millis();

        //check battery charge
        writeCharge(getBatteryPercentage());
      }


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
          adjustTime(fix.dateTime);
          fix.dateTime.setAMorPM();
          writeTime(fix.dateTime.hours, fix.dateTime.minutes);
        }
      }

      //Loop checking for high signal from RFID reader
      if (nano.check() == true) //Check to see if any new data has come in from module
      {
        byte responseType = nano.parseResponse(); //Break response into tag ID, RSSI, frequency, and timestamp

        //only check if we found a tag, dont care about anything else
        if (responseType == RESPONSE_IS_TAGFOUND)
        {

          byte tagEPCBytes = nano.getTagEPCBytes(); //Get the number of bytes of EPC from response
          //Print EPC bytes, this is a subsection of bytes from the response/msg array

          for (byte x = 0 ; x < tagEPCBytes ; x++)
          {
            char arrayByte[3];

            if (nano.msg[31 + x] < 0x10)
            {
              strcat(myEPC, "0"); //Pretty print
            }

            itoa (nano.msg[31 + x], arrayByte, 10);
            strcat(myEPC, arrayByte);

            strcat(myEPC, " ");

          }

          for (int i = 0; i < turtlesFound; i++)
          {
            // NeoSerial.println(strcmp(myEPC, tagIDS[i]));
            // NeoSerial.println(myEPC);
            // NeoSerial.println(tagIDS[i]);
            if (!strcmp(myEPC, tagIDS[i]))
            {
              turtleAlreadyLogged = 1;
              break;
            }
          }
          if (!turtleAlreadyLogged)
          {
            STATE = DETECTION_SCREEN;
            strcpy(tagIDS[turtlesFound] , myEPC);
            turtlesFound++;
          }
          turtleAlreadyLogged = 0;

        }
      }
    }
  }

  //TODO: Design system to guard against repeat detecti




  //code if turtle found
  if (STATE == DETECTION_SCREEN)
  {
    //initilize detection event structure
    detectionEventInfo detectionEvent;
    char timeStamp[25];
    char dateString[25];

    char epcFound[50] = "";

    //Turn LED RED
    displayRed();

    //TODO: Implement sounding buzzer
    //soundBuzzer();

    //set tag ID into struct
    strcpy(detectionEvent.tagID , tagIDS[turtlesFound - 1]);
    sprintf(epcFound, "ID: %s", detectionEvent.tagID);

    //Wait for next GPS input and parse time and coordinates out and save to detectionEvent struct
    //NeoSerial.println("waiting for GPS");
    while (!gps.available());
    if (gps.available())
    {
      //Read Fix
      fix = gps.read();
      //If location vaild
      if (fix.valid.location)
      {
        detectionEvent.latitude = (fix.latitudeL()) / (10000000.0);
        detectionEvent.longitude = (fix.longitudeL()) / (10000000.0);
      }
      if (fix.valid.time && fix.valid.date)
      {
        //Adjust time to EST
        adjustTime(fix.dateTime);
        //Convert from military time to conventional
        fix.dateTime.setAMorPM();

        //Format date string and save

        sprintf(timeStamp, "%d:%02d:%02d %02d-%02d-%d", fix.dateTime.hours, fix.dateTime.minutes, fix.dateTime.seconds, fix.dateTime.month, fix.dateTime.date, fix.dateTime.year);
        sprintf(detectionEvent.timeS, "%d:%02d:%02d", fix.dateTime.hours, fix.dateTime.minutes, fix.dateTime.seconds);
        sprintf(detectionEvent.dateS, "%02d-%02d-%d", fix.dateTime.month, fix.dateTime.date, fix.dateTime.year);
        sprintf(dateString, "Time: %s", timeStamp);
      }
    }

    //After data collection, Draw Screen
    drawDetectionScreen(epcFound, dateString, detectionEvent.longitude, detectionEvent.latitude);

    // Set timer variable for button timeout
    //buttonPressedTimer = millis();

    //Wait for either Yes or no to log data (check input push buttons)
    //Timeout after set time of no button pressed
    while (((buttonSelect = buttonPressed()) != BUTTON_SELECT)) //&&
      //(millis() - buttonPressedTimer < BUTTON_TIMEOUT))
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

    //IF yes is selected and timeout is not met, log data and display on UI
    if ((optionSelected() == YES_SELECTED))// && (millis() - buttonPressedTimer < BUTTON_TIMEOUT))
    {
      //If Yes: Log to SD card
      if (!(logDetectionEvent(detectionEvent.tagID, detectionEvent.timeS, detectionEvent.dateS, detectionEvent.latitude, detectionEvent.longitude)))
      {
        DEBUG_PORT.println("Error logging to SD Card");
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
