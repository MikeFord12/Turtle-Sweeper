#include "LCD.h"

extern Adafruit_ILI9341 tft;
int userSelection;

void drawMainScreen()
{
        tft.fillScreen(BLACK);
        tft.drawRect(0,0,70,25,WHITE);
        setupText(3,5,WHITE,2);
        tft.print("HH:MM");
        tft.drawRect(220,0,100,25,WHITE);
        setupText(245,5,WHITE,2);
        tft.print("XX%");
        setupText(90,80,GREEN,3);
        tft.print("Scanning");
        setupText(140,110,GREEN,3);
        tft.print("in");
        setupText(90,140,GREEN,3);
        tft.print("Progress");
}

void drawInitializationScreen()
{
        tft.fillScreen(BLACK);
        setupText(90,80,GREEN,3);
        tft.print("Waiting");
        setupText(140,110,GREEN,3);
        tft.print("for");
        setupText(90,140,GREEN,3);
        tft.print("GPS Fix");
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

void drawDetectionScreen(int ID, String timeStamp, float Long, float Lat)
{
        char IDString[20];
        char timeString[30];
        char LongString[20];
        char LatString[20];

        tft.fillScreen(BLACK);
        setupText(60,5,GREEN,2);
        tft.print("TURTLE DETECTED");
        tft.drawRect(10,25,300,90,WHITE);

        setupText(15,30,WHITE,2);
        sprintf(IDString,"ID: %d",ID);
        tft.print(IDString);

        setupText(15,50,WHITE,2);
        sprintf(timeString,"Time: %s",timeStamp.c_str());
        tft.print(timeString);

        setupText(15,70,WHITE,2);
        sprintf(LongString,"GPS Long: %s",String(Long,7).c_str());
        tft.print(LongString);

        setupText(15,90,WHITE,2);
        sprintf(LatString,"GPS Lat: %s",String(Lat,7).c_str());
        tft.print(LatString);

        setupText(8,150,WHITE,2);
        tft.print("Save this detection event?");

        setupText(80, 190, WHITE, 2);
        tft.print("Yes");
        setupText(208, 190, WHITE, 2);
        tft.print("No");

        tft.fillTriangle(61, 190, 61, 202, 73, 196, GREEN);
        tft.fillTriangle(188, 190, 188, 202, 200, 196, BLACK);
}

void writeTime(int hours, int minutes)
{
        char stringToWrite [7];
        tft.fillRect(1,1,68,23,BLACK);
        sprintf(stringToWrite,"%d:%d",hours,minutes);
        setupText(3,5,WHITE,2);
        tft.print(stringToWrite);
}

void writeCharge(int charge)
{
        char stringToWrite [5];
        tft.fillRect(221,1,98,23,BLACK);
        sprintf(stringToWrite," %d%%",charge);
        setupText(245,5,WHITE,2);
        //tft.print("%%");
        //  setupText(250,5,WHITE,2);
        tft.print(stringToWrite);
}

void setupLCD()
{
        tft.begin();
        tft.setRotation(3);
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
        tft.fillTriangle(61, 190, 61, 202, 73, 196, GREEN);
        tft.fillTriangle(188, 190, 188, 202, 200, 196, BLACK);
        userSelection = YES_SELECTED;
}

void drawNoSelection()
{
        tft.fillTriangle(61, 190, 61, 202, 73, 196, BLACK);
        tft.fillTriangle(188, 190, 188, 202, 200, 196, GREEN);
        userSelection = NO_SELECTED;
}

int optionSelected()
{
        int temp = userSelection;
        userSelection = YES_SELECTED;
        return temp;
}
