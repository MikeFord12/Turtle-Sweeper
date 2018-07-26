#ifndef _VOLTAGE_READER_h
#define _VOLTAGE_READER_h

#include "SPI.h"
#include "limits.h"

// returns time remaining on battery in minutes
int getBatteryTimeRemaining();

// Helper methods
long getVref();
int convertVrefToPercentage(long vref);
int isBatteryCriticallyLow();

#endif