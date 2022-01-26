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
 *  If statement inside of while loop? There's probably a better way to do this though
 *    While the mosfet is on, if the temperature is above some value, turn the mosfet off
 *    Same idea for when the mosfet is off
 *  
 */

void setup()
{
  
}

void loop()
{
  
}
