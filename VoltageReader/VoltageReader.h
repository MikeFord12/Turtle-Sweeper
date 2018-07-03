#ifndef _VOLTAGE_READER_h
#define _VOLTAGE_READER_h

#include "SPI.h"
#include "limits.h"

int getBatteryPercentage();

// Helper methods
long getVref();
int convertVrefToPercentage(long vref);
int isBatteryCriticallyLow();

#endif