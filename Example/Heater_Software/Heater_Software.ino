#include "Heater_Software.h"

void setup()
{
  // inputs to MOSFET circuits
  pinMode(PIN_2, OUTPUT);
  pinMode(PIN_6, OUTPUT);
  pinMode(PIN_9, OUTPUT);
  
  // data from temperature sensors
  pinMode(PIN_A17, INPUT);
  pinMode(PIN_A16, INPUT);
  pinMode(PIN_A15, INPUT);
  float temps[3] = {temp1Celsius, temp2Celsius, temp3Celsius};

  Serial.begin(9600);
  
  RoveComm.begin(RC_HEATERBOARD_FOURTHOCTET, &TCPSERVER);
}

void loop()
{
  for (int i == 0; i < 3; i++)
  {
      switch(temps[i])
    {
      case >= 105 :
        digitalWrite(TEMP_SENSE_PIN_1, LOW);
        break;
      case <= 95  :
        digitalWrite(TEMP_SENSE_PIN_1, HIGH);
        break;
    }
  }
  ////// Still Needed: //////
  // switch case for RoveComm needed
  // communication with basestation
  // overheat warning for basestation
  
  
  if(millis()-last_update_time >= ROVECOMM_UPDATE_RATE)
  {

  }
  
}
