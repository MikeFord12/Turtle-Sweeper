#include "LCD.h"

void drawMainScreen()
{
        tft.fillScreen(BLACK);
        tft.drawRect(0,0,100,25,WHITE);
        setupText(3,5,WHITE,2);
        tft.print("HH:MM AM");
        tft.drawRect(220,0,100,25,WHITE);
        setupText(245,5,WHITE,2);
        tft.print("100%");
        setupText(90,80,GREEN,3);
        tft.print("Scanning");
        setupText(140,110,GREEN,3);
        tft.print("in");
        setupText(90,140,GREEN,3);
        tft.print("Progress");
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

void drawDetectionScreen()
{

        tft.fillScreen(BLACK);
        setupText(60,5,GREEN,2);
        tft.print("TURTLE DETECTED");
        tft.drawRect(10,25,300,90,WHITE);
        setupText(15,30,WHITE,2);
        tft.print("ID: XX-XX-XX-XX-XX-XX-XX");
        setupText(15,50,WHITE,2);
        tft.print("Time: HH:MM:SS AM");
        setupText(15,70,WHITE,2);
        tft.print("GPS Long: XX.XXXXX");
        setupText(15,90,WHITE,2);
        tft.print("GPS Lat:  XX.XXXXX");

        setupText(8,150,WHITE,2);
        tft.print("Save this detection event?");

        setupText(140,180,WHITE,2);
        tft.print("Yes");
        setupText(148,200,WHITE,2);
        tft.print("No");


}

void writeTime(int hours, int minutes)
{
        char stringToWrite [7];
        tft.drawRect(1,1,98,23,BLACK);
        sprintf(stringToWrite,"%d:%d",hours,minutes);
        setupText(3,5,WHITE,2);
        tft.print(stringToWrite);
}

void writeCharge(int charge)
{
        char stringToWrite [5];
        tft.drawRect(221,1,98,23,BLACK);
        sprintf(stringToWrite,"%%d",charge);
        setupText(245,5,WHITE,2);
        tft.print(stringToWrite);
}

void setupLCD()
{
        tft.begin();
        tft.setRotation(3);

}

void setupText(uint16_t xCoord,uint16_t yCoord, uint16_t color, uint8_t fontSize)
{
        tft.setCursor(xCoord, yCoord);
        tft.setTextColor(color);
        tft.setTextSize(fontSize);
        tft.setTextWrap(false);
}
