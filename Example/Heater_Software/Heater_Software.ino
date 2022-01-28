#include "Heater_Software.h"

void setup()
{
  
}

void loop()
{
  // turns off heaters at high temperatures
  if temp1 >= 105
  {
    digitalWrite(PIN_2, LOW);
  }
  
  if temp2 >= 105
  {
    digitalWrite(PIN_6, LOW);
  }
  
  if temp3 >= 105
  {
    digitalWrite(PIN_9, LOW);
  }

  // turns on heaters at low temperatures
  if temp1 <= 95
  {
    digitalWrite(PIN_2, HIGH);
  }
  
  if temp2 <= 95
  {
    digitalWrite(PIN_6, HIGH);
  }
  
  if temp3 <= 95
  {
    digitalWrite(PIN_9, HIGH);
  }

  
}
