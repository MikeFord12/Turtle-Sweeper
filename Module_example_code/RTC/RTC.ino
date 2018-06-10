#include "RTClib.h"
#include <Wire.h>

RTC_DS3231 rtc;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

void setup() {
  
  // Setup Serial connection
  Serial.begin(9600);

  // Initialize the rtc object
  while(!rtc.begin())
  {
    Serial.println("Cannot start RTC!!!");
    delay(1000);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
    
    // sets the RTC to the date & time this sketch was compiled
    // based off computer clock, but could be used to set initial time beofre assembling
    // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    
    // sets the RTC with an explicit date & time
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }

}

void loop() {
  // obtains DateTime variable from rtc
  DateTime now = rtc.now();

  // Prints each component of DateTime obtained
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" (");
  Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
  Serial.print(") ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();

  delay(1000);
}
