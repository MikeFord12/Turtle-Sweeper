#include "voltageReader.h"

double getVoltage()
{
	return convertVrefToVoltage(getVref());
}

long getVref()
{
  // Read 1.1V reference against AVcc
  // set the reference to Vcc and the measurement to the internal 1.1V reference
  #if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
    ADMUX = _BV(MUX5) | _BV(MUX0);
  #elif defined (__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
    ADMUX = _BV(MUX3) | _BV(MUX2);
  #else
    ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #endif  

  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Start conversion
  while (bit_is_set(ADCSRA,ADSC)); // measuring

  uint8_t low  = ADCL; // must read ADCL first - it then locks ADCH  
  uint8_t high = ADCH; // unlocks both

  long result = (high<<8) | low;

  result = 1125300L / result; // Calculate Vcc (in mV); 1125300 = 1.1*1023*1000
  return result; // Vcc in millivolts
}

double convertVrefToVoltage(long vref)
{
  switch(vref)
  {
  case 0 ... 4295:
    return 6.0;

  case 4296 ... 4387:
    return 6.1;

  case 4388 ... 4484:
    return 6.2;

  case 4485 ... 4584:
    return 6.3;

  case 4585 ... 4677:
    return 6.4;
    
  case 4678 ... 4777:
    return 6.5;
    
  case 4778 ... 4880:
    return 6.6;
    
  case 4881 ... 4988:
    return 6.7;
    
  case 4989 ... 5046:
    return 6.8;
    
  case 5047 ... LONG_MAX:
    return 8.4;
    
  default:
    return 0.0;
  }
}