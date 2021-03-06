#include "LCD.h"

extern Adafruit_ILI9341 tft;
int userSelection;


/**
 * Function: drawMainScreen(int GPS_INITIALIZED, int SD_INITIALIZED)
 *---------------------
 * Draws main scanning screen
 *
 * Parameters: GPS_INITIALIZED - flag from main function if GPS found a fix or not
 * SD_INITIALIZED - flag from main function if SD card was initlaied correcty
 * Return: None
 */
void drawMainScreen(int GPS_INITIALIZED, int SD_INITIALIZED)
{
        tft.fillScreen(WHITE);
        tft.drawRect(220,0,100,25,BLACK);
        setupText(245,5,BLACK,2);
        tft.print("XX%");
        setupText(90,80,BLACK,3);
        tft.print("Scanning");
        setupText(140,110,BLACK,3);
        tft.print("in");
        setupText(90,140,BLACK,3);
        tft.print("Progress");


        if(!GPS_INITIALIZED && !SD_INITIALIZED)
        {
                setupText(90,200,BLACK,2);
                tft.print("Basic Mode");
        }
        else if(!GPS_INITIALIZED)
        {
                setupText(90,200,BLACK,2);
                tft.print("No GPS Mode");
        }
        else if(!SD_INITIALIZED)
        {
                setupText(90,200,BLACK,2);
                tft.print("No SD Mode");
        }
}

/**
 * Function: drawTurtlesFound(int turtlesFound)
 *---------------------
 * Draws number of turtles found so far on main screen
 *
 * Parameters: int turtlesFound - number of turtles found
 * Return: None
 */
void drawTurtlesFound(int turtlesFound)
{
        tft.fillRect(174,4,15,15,WHITE);
        setupText(3,5,BLACK,2);
        tft.print("Turtles Found:");
        setupText(175,5,BLACK,2);
        tft.print(turtlesFound);
}

/**
 * Function: drawInitializationScreen()
 *---------------------
 * Draws waiting for GPS fix screen
 *
 * Parameters: None
 * Return: None
 */
void drawInitializationScreen()
{
        tft.fillScreen(WHITE);
        setupText(90,80,BLACK,3);
        tft.print("Waiting");
        setupText(140,110,BLACK,3);
        tft.print("for");
        setupText(90,140,BLACK,3);
        tft.print("GPS Fix");
		
		setupText(20,220,BLACK,2);
        tft.print("Press select for no GPS");
}

/**
 * Function: gatheringGPSScreen()
 *---------------------
 * Draws the gathering GPS data screen
 *
 * Parameters: None
 * Return: None
 */
void gatheringGPSScreen()
{
        tft.fillScreen(WHITE);
        setupText(30,80,BLACK,3);
        tft.print("Turtle Found");
        setupText(60,110,BLACK,3);
        tft.print("Gathering");
        setupText(60,140,BLACK,3);
        tft.print("GPS Data");
}

/**
 * Function: drawCriticalBatteryScreen()
 *---------------------
 * Draws battery critically low screen
 *
 * Parameters: None
 * Return: None
 */
void drawCriticalBatteryScreen()
{
        tft.fillScreen(WHITE);
        setupText(100,80,BLACK,3);
        tft.print("BATTERY");
        setupText(70,110,BLACK,3);
        tft.print("CRITICALLY");
        setupText(130,140,BLACK,3);
        tft.print("LOW");
}

/**
 * Function: tenMinuteSplashScreen()
 *---------------------
 * 2 second splash screen to infrom user of <10 min of
 * battery life remaining
 *
 * Parameters: None
 * Return: None
 */
void tenMinuteSplashScreen()
{
        tft.fillScreen(WHITE);
        setupText(100,80,BLACK,3);
        tft.print("10 MINUTES");
        setupText(100,110,BLACK,3);
        tft.print("OF BATTERY");
        setupText(110,140,BLACK,3);
        tft.print("LIFE LEFT");
        delay(2000);
}

/**
 * Function: drawDetectionScreen()
 *---------------------
 * Draws detection screen with detetion event information
 *
 * Parameters: char ID[] - Id of tag found
 * char timeStamp[] - timestamp of when turtle was found
 * float Long - longitude of detection event
 * float Lat - latitude of detection event
 * Return: None
 */
int drawDetectionScreen(char ID[], char timeStamp[], float Long, float Lat)
{


        tft.fillScreen(WHITE);
        setupText(60,5,BLACK,2);
        tft.print("TURTLE DETECTED");
        tft.drawRect(10,70,300,70,WHITE);

        setupText(0,30,BLACK,2);
        tft.setTextWrap(true);
        tft.print(ID);


        setupText(15,75,BLACK,2);
        tft.print(timeStamp);

        setupText(15,95,WHITE,2);
        tft.print("GPS Long: ");
        setupText(150,95,BLACK,2);
        tft.print(Long,7);

        setupText(15,115,BLACK,2);
        tft.print("GPS Lat: ");
        setupText(150,115,BLACK,2);
        tft.print(Lat,7);

        setupText(0,150,WHITE,2);
        tft.print("Save this detection event?");

        setupText(80, 190, BLACK, 2);
        tft.print("Yes");
        setupText(208, 190, BLACK, 2);
        tft.print("No");

        tft.fillTriangle(61, 190, 61, 202, 73, 196, BLACK);
        tft.fillTriangle(188, 190, 188, 202, 200, 196, WHITE);

        return 1;
}

/**
 * Function: drawDetectionScreen()
 *---------------------
 * Draws detection screen with detetion event information
 *
 * Parameters: char ID[] - Id of tag found
 * char timeStamp[] - timestamp of when turtle was found
 * float Long - longitude of detection event
 * float Lat - latitude of detection event
 * Return: None
 */
int drawBasicDetectionScreen(char ID[])
{

        char lastPartOfID[4];
        memcpy(lastPartOfID, &ID[strlen(ID)-3],3);
        lastPartOfID[3] = '\0';
        tft.fillScreen(WHITE);
        setupText(60,5,BLACK,2);
        tft.print("TURTLE DETECTED");
        tft.drawRect(10,70,300,70,WHITE);

        setupText(30,30,BLACK,2);
        tft.print("ID:");
        setupText(70,30,BLACK,2);
        tft.print(lastPartOfID);
        setupText(30,70,BLACK,2);
        tft.print("Time Stamp: N/A");
        setupText(30,100,BLACK,2);
        tft.print("GPS Longitude: N/A");
        setupText(30,130,BLACK,2);
        tft.print("GPS Latitude: N/A");


        /*  setupText(0,30,BLACK,2);
           tft.setTextWrap(true);
           tft.print(ID);
         */
        setupText(0,190,BLACK,2);
        tft.print("Press any button to continue.");

        return 1;
}
/**
 * Function: writeCharge(int charge)
 *---------------------
 * Writes the remaining battery time left to the screen
 *
 * Parameters: int charge - battery time left
 * Return: None
 */
void writeCharge(int charge)
{
        if(charge == 100)
        {
                tft.fillRect(221,1,98,23,GREEN);
        }
        else
        {
                char stringToWrite [5];
                tft.fillRect(221,1,98,23,BLACK);
                sprintf(stringToWrite," %d ML",charge);
                setupText(245,5,WHITE,2);
                tft.print(stringToWrite);
        }
}

/**
 * Function: setupLCD()
 *---------------------
 * Sets the LCD and orientation
 *
 * Parameters: None
 * Return: None
 */
void setupLCD()
{
        tft.begin();
        tft.setRotation(1);
        userSelection = YES_SELECTED;
}

/**
 * Function: setupText(uint16_t xCoord,uint16_t yCoord, uint16_t color, uint8_t fontSize)
 *---------------------
 * Sets the LED pins defined in LEDs.h as outputs
 *
 * Parameters: uint16_t xCoord - x coordinate of text to be placed on screen
 * uint16_t yCoord - y coordinate of text to be placed on screen
 * uint16_t color - color of text to be written
 * uint8_t fontSize - font size of text to be written
 * Return: None
 */
void setupText(uint16_t xCoord,uint16_t yCoord, uint16_t color, uint8_t fontSize)
{
        tft.setCursor(xCoord, yCoord);
        tft.setTextColor(color);
        tft.setTextSize(fontSize);
        tft.setTextWrap(false);
}

/**
 * Function: drawYesSelection()
 *---------------------
 * Move arrow on screen to YES option
 *
 * Parameters: None
 * Return: None
 */
void drawYesSelection()
{
        tft.fillTriangle(61, 190, 61, 202, 73, 196, BLACK);
        tft.fillTriangle(188, 190, 188, 202, 200, 196, WHITE);
        userSelection = YES_SELECTED;
}

/**
 * Function: drawNoSelection()
 *---------------------
 * Move arrow on screen to NO option
 *
 * Parameters: None
 * Return: None
 */
void drawNoSelection()
{
        tft.fillTriangle(61, 190, 61, 202, 73, 196, WHITE);
        tft.fillTriangle(188, 190, 188, 202, 200, 196, BLACK);
        userSelection = NO_SELECTED;
}

/**
 * Function: optionSelected()
 *---------------------
 * Keeps track of currently selected YES or NO
 *
 * Parameters: None
 * Return: None
 */
int optionSelected()
{
        int temp = userSelection;
        userSelection = YES_SELECTED;
        return temp;
}

/**
 * Function: drawErrorScreen()
 *---------------------
 * Draws error screen if initialization fails
 *
 * Parameters: int errorCode - number passed in that indicates what module failed
 * Return: None
 */
void drawErrorScreen(int errorCode)
{
        tft.fillScreen(WHITE);
        setupText(20,80,BLACK,2);
        tft.print("INITIALIZATION FAIL");

        switch(errorCode)
        {
        // SD Initialization failed
        case 1:
                setupText(140,130,BLACK,3);
                tft.print("SD");
                break;

        // RFID reader initialization failed
        case 2:
                setupText(120,130,BLACK,3);
                tft.print("RFID");
                while(1);
                break;


        // RFID reader initialization failed
        case 3:
                setupText(120,130,BLACK,3);
                tft.print("GPS");
                break;
        }
}

/**
 * Function: printDesiredTagValue()
 *---------------------
 * Updates Desired tag value based on user input
 *
 * Parameters: int desiredTagID - number user is modifying
 * Return: None
 */
void printDesiredTagValue(int desiredTagID)
{
        tft.fillRect(139,129,75,50,WHITE);
        setupText(150,135,BLACK,3);
        tft.print(desiredTagID);
}

/**
 * Function: drawModeSelectScreen()
 *---------------------
 * Prompts user which mode they would like to start the system in
 *
 * Parameters: None
 * Return: None
 */
void drawModeSelectScreen()
{
        tft.fillScreen(WHITE);
        setupText(40,80,BLACK,3);
        tft.print("Select");
        setupText(190,80,BLACK,3);
        tft.print("Mode");

        setupText(80, 190, BLACK, 2);
        tft.print("Read");
        setupText(208, 190, BLACK, 2);
        tft.print("Write");

        tft.fillTriangle(61, 190, 61, 202, 73, 196, BLACK);
        tft.fillTriangle(188, 190, 188, 202, 200, 196, WHITE);
}

/**
 * Function: writeConfirmationScreen()
 *---------------------
 * Confirmation screen after user has selected value to write to tag
 *
 * Parameters: WriteVal - Value user has selected to write to tag
 * Return: None
 */
void writeConfirmationScreen(int writeVal)
{
        char stringToWrite [6];

        tft.fillScreen(WHITE);
        setupText(60,30,BLACK,3);
        tft.print("Write Value?");


        setupText(145,110,BLACK,3);
        tft.print(writeVal);

        setupText(80, 190, BLACK, 2);
        tft.print("Yes");
        setupText(208, 190, BLACK, 2);
        tft.print("No");

        tft.fillTriangle(61, 190, 61, 202, 73, 196, BLACK);
        tft.fillTriangle(188, 190, 188, 202, 200, 196, WHITE);
}

/**
 * Function: drawNumberScreen()
 *---------------------
 * Write screen for user to select ID tag to write
 *
 * Parameters: None
 * Return: None
 */
void drawNumberScreen()
{
        tft.fillScreen(WHITE);
        setupText(50,20,BLACK,2);
        tft.print("Scroll to desired ID");
        setupText(120,60,BLACK,2);
        tft.print("(0-255)");

        tft.fillTriangle( 99, 196, 110, 190, 110, 202, BLACK);
        tft.fillTriangle(210, 190, 210, 202, 222, 196, BLACK);
}

/**
 * Function: drawWriteSuccessful()
 *---------------------
 * Flash successful screen for successful tag write
 *
 * Parameters: None
 * Return: None
 */
void drawWriteSuccessful()
{
        tft.fillScreen(GREEN);
        setupText(20,100,BLACK,3);
        tft.print("Write Success!");

        delay(3000);
}

/**
 * Function: drawWriteFailure()
 *---------------------
 * Flash failure screen for unsuccessful tag write
 *
 * Parameters: None
 * Return: None
 */
void drawWriteFailure()
{
        tft.fillScreen(RED);
        setupText(20,100,BLACK,3);
        tft.print("Write Failure!");

        delay(3000);
}
