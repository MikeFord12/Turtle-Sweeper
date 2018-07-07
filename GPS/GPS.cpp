#include "GPS.h"
#include "Arduino.h"

// Check configuration

#ifndef NMEAGPS_INTERRUPT_PROCESSING
  #error You must define NMEAGPS_INTERRUPT_PROCESSING in NMEAGPS_cfg.h!
#endif

//--------------------------
//Most code taken from NEOGPS_timezone example.
//---------------------------
static const int32_t zone_hours   = -5L;          // EST
static const int32_t zone_minutes =  0L;          // usually zero
static const NeoGPS::clock_t zone_offset  =       // offset in seconds
                                            zone_hours   * NeoGPS::SECONDS_PER_HOUR +
                                            zone_minutes * NeoGPS::SECONDS_PER_MINUTE;

//USA Daylight Savings Time
#define USA_DST

#if defined(USA_DST)
static const uint8_t springMonth =  3;
static const uint8_t springDate  = 14;   // latest 2nd Sunday
static const uint8_t springHour  =  2;
static const uint8_t fallMonth   = 11;
static const uint8_t fallDate    =  7;   // latest 1st Sunday
static const uint8_t fallHour    =  2;
  #define CALCULATE_DST
#endif


/**
 * Function: GPSisr()
 *------------------
 * This is the interrupt service handler.
 * This is called everytime a character comes into the serial buffer.
 * Character is then sent to gps object to process.
 *
 * c: char from buffer to be processed
 * Return: none
 */
static void GPSisr( uint8_t c )
{
        gps.handle( c );

}
//--------------------------


/**
 * Function: GPSisr()
 *------------------
 * This function sets up the GPS module communication.
 *
 * Return: none
 */
void setupGPS()
{
        gpsPort.attachInterrupt(GPSisr); //attatch GPSisr to Serial port
        gpsPort.begin(9600); //Start port at 9600 baud rate
        gpsPort.flush(); //Flush port just to start off fresh
}
//--------------------------

/**
 * Function: GPSisr()
 *------------------
 * This function converts the GPS time returned in UTC to EST
 * This function takes in UTC time structure, and converts it to seconds
 * The EST offset (in seconds) is subtracted from the UTC seconds
 * The seconds now in EST time are converted back to an EST time structure
 *
 * dt: UTC time structure (timestamp) from GPS receiver
 * Return: none
 */
void adjustTime( NeoGPS::time_t & dt )
{
        NeoGPS::clock_t seconds = dt; // convert date/time structure to seconds

#ifdef CALCULATE_DST
        //  Calculate DST changeover times once per reset and year!
        static NeoGPS::time_t changeover;
        static NeoGPS::clock_t springForward, fallBack;

        if ((springForward == 0) || (changeover.year != dt.year)) {

                //  Calculate the spring changeover time (seconds)
                changeover.year    = dt.year;
                changeover.month   = springMonth;
                changeover.date    = springDate;
                changeover.hours   = springHour;
                changeover.minutes = 0;
                changeover.seconds = 0;
                changeover.set_day();
                // Step back to a Sunday, if day != SUNDAY
                changeover.date -= (changeover.day - NeoGPS::time_t::SUNDAY);
                springForward = (NeoGPS::clock_t) changeover;

                //  Calculate the fall changeover time (seconds)
                changeover.month   = fallMonth;
                changeover.date    = fallDate;
                changeover.hours   = fallHour - 1;// to account for the "apparent" DST +1
                changeover.set_day();
                // Step back to a Sunday, if day != SUNDAY
                changeover.date -= (changeover.day - NeoGPS::time_t::SUNDAY);
                fallBack = (NeoGPS::clock_t) changeover;
        }
#endif

        //  First, offset from UTC to the local timezone
        seconds += zone_offset;

#ifdef CALCULATE_DST
        //  Then add an hour if DST is in effect
        if ((springForward <= seconds) && (seconds < fallBack))
                seconds += NeoGPS::SECONDS_PER_HOUR;
#endif

        dt = seconds; // convert seconds back to a date/time structure

} // adjustTime
//---------------------------
