#include "LCD.h"

extern Adafruit_ILI9341 tft;
int userSelection;

void drawMainScreen(int GPS_INITIALIZED, int SD_INITIALIZED)
{
        /*  tft.fillScreen(WHITE);
           tft.drawRect(0,0,70,25,BLACK);
           setupText(3,5,WHITE,2);
           tft.print("HH:MM");*/
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

void drawTurtlesFound(int turtlesFound)
{
        tft.fillRect(174,4,15,15,WHITE);
        setupText(3,5,BLACK,2);
        tft.print("Turtles Found:");
        setupText(175,5,BLACK,2);
        tft.print(turtlesFound);
}

void drawInitializationScreen()
{
        tft.fillScreen(WHITE);
        setupText(90,80,BLACK,3);
        tft.print("Waiting");
        setupText(140,110,BLACK,3);
        tft.print("for");
        setupText(90,140,BLACK,3);
        tft.print("GPS Fix");
}

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

void drawCriticalBatteryScreen()
{
        tft.fillScreen(RED);
        setupText(100,80,BLACK,3);
        tft.print("BATTERY");
        setupText(70,110,BLACK,3);
        tft.print("CRITICALLY");
        setupText(130,140,BLACK,3);
        tft.print("LOW");
}

int drawDetectionScreen(char ID[], char timeStamp[], float Long, float Lat)
{


        tft.fillScreen(WHITE);
        setupText(60,5,BLACK,2);
        tft.print("TURTLE DETECTED");
        tft.drawRect(10,70,300,70,WHITE);

        setupText(0,30,BLACK,2);
        tft.setTextWrap(true);
        //sprintf(IDString,"ID: %s",ID.c_str());
        tft.print(ID);


        //NeoSerial.println(timeStamp.c_str());
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

void writeTime(int hours, int minutes)
{
        char stringToWrite [7];
        tft.fillRect(1,1,68,23,BLACK);
        sprintf(stringToWrite,"%02d:%02d",hours,minutes);
        setupText(3,5,WHITE,2);
        tft.print(stringToWrite);
}

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
                sprintf(stringToWrite," %d%%",charge);
                setupText(245,5,WHITE,2);
                //tft.print("%%");
                //  setupText(250,5,WHITE,2);
                tft.print(stringToWrite);
        }

}

void setupLCD()
{
        tft.begin();
        tft.setRotation(1);
        userSelection = YES_SELECTED;
}

void setupText(uint16_t xCoord,uint16_t yCoord, uint16_t color, uint8_t fontSize)
{
        tft.setCursor(xCoord, yCoord);
        tft.setTextColor(color);
        tft.setTextSize(fontSize);
        tft.setTextWrap(false);
}

void drawYesSelection()
{
        tft.fillTriangle(61, 190, 61, 202, 73, 196, BLACK);
        tft.fillTriangle(188, 190, 188, 202, 200, 196, WHITE);
        userSelection = YES_SELECTED;
}

void drawNoSelection()
{
        tft.fillTriangle(61, 190, 61, 202, 73, 196, WHITE);
        tft.fillTriangle(188, 190, 188, 202, 200, 196, BLACK);
        userSelection = NO_SELECTED;
}

int optionSelected()
{
        int temp = userSelection;
        userSelection = YES_SELECTED;
        return temp;
}

void drawErrorScreen(int errorCode)
{
        tft.fillScreen(RED);
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
