#include "Heater_Software.h"

/*Outline of board functions:
 * When turned on: enable heater
 *  Use pins OUT2, OUT1D, and OUT1C on Teensy to turn on LT1910s
 *  Read data coming in from A15-17
 *  When temperature exceeds x degrees C:
 *    Turn off LT1910
 *  When temperature goes back below y degrees C:
 *    Turn on LT1910
 *  Always:
 *    Send temperature data to basestation via RoveComm
 */

/*Code things to do:
 * Establish temperature variable in header file
 *  Separate variables for each temperature sensor
 * Only send one signal when temperature goes outside of the range 
 *  If statements
 */

void setup()
{
  
}

void loop()
{
  if temp1 >= 105
  {
    // stop sending power to OUT2 pin
  }
  
  if temp2 >= 105
  {
    // stop sending power to OUT1C pin
  }
  
  if temp3 >= 105
  {
    // stop sending power to OUT1D pin
  }

  
  if temp1 <= 95
  {
    // start sending power to OUT2 pin
  }
  
  if temp2 <= 95
  {
    // start sending power to OUT1C pin
  }
  
  if temp3 <= 95
  {
    // start sending power to OUT1D pin
  }

  
}
