#ifndef _VOLTAGE_READER_h
#define _VOLTAGE_READER_h

#include "SPI.h"
#include "limits.h"

double getVoltage();

// Helper methods
long getVref();
double convertVrefToVoltage(long vref);

#endif