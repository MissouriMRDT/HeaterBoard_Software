#include "Heater_Software.h"

void setup()
{
  
}

void loop()
{
  // turns off heaters at high temperatures
  if (map(temp1, TEMP_ADC_MIN, TEMP_ADC_MAX, TEMP_MIN, TEMP_MAX) >= 105)
  {
    digitalWrite(PIN_2, LOW);
  }
  
  if (map(temp2, TEMP_ADC_MIN, TEMP_ADC_MAX, TEMP_MIN, TEMP_MAX) >= 105)
  {
    digitalWrite(PIN_6, LOW);
  }
  
  if (map(temp3, TEMP_ADC_MIN, TEMP_ADC_MAX, TEMP_MIN, TEMP_MAX) >= 105)
  {
    digitalWrite(PIN_9, LOW);
  }

  // turns on heaters at low temperatures
  if (map(temp1, TEMP_ADC_MIN, TEMP_ADC_MAX, TEMP_MIN, TEMP_MAX) <= 95)
  {
    digitalWrite(PIN_2, HIGH);
  }
  
  if (map(temp2, TEMP_ADC_MIN, TEMP_ADC_MAX, TEMP_MIN, TEMP_MAX) <= 95)
  {
    digitalWrite(PIN_6, HIGH);
  }
  
  if (map(temp3, TEMP_ADC_MIN, TEMP_ADC_MAX, TEMP_MIN, TEMP_MAX) <= 95)
  {
    digitalWrite(PIN_9, HIGH);
  }

  
}
