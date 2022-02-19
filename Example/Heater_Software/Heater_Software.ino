#include "Heater_Software.h"

void setup()
{
  // inputs to MOSFET circuits
  pinMode(HEATER_TOGGLE_PIN_1, OUTPUT);
  pinMode(HEATER_TOGGLE_PIN_2, OUTPUT);
  pinMode(HEATER_TOGGLE_PIN_3, OUTPUT);
  
  // data from temperature sensors
  pinMode(THERMO_DATA_1, INPUT);
  pinMode(THERMO_DATA_2, INPUT);
  pinMode(THERMO_DATA_3, INPUT);


  Serial.begin(9600);

  //Set up rovecomm with the correct IP and the TCP server
  RoveComm.begin(RC_HEATERBOARD_FOURTHOCTET, &TCPServer);
  delay(100);

  Serial.println("Started: ");
  
  //update timekeeping
  last_update_time = millis();
}

void loop()
{
  // temperature data from each sensor
  float temp1 = analogRead(THERMO_DATA_1);
  float temp2 = analogRead(THERMO_DATA_2);
  float temp3 = analogRead(THERMO_DATA_3);

  float temp1Celsius = map(temp1, TEMP_ADC_MIN, TEMP_ADC_MAX, TEMP_MIN, TEMP_MAX) / 1000;
  float temp2Celsius = map(temp2, TEMP_ADC_MIN, TEMP_ADC_MAX, TEMP_MIN, TEMP_MAX) / 1000;
  float temp3Celsius = map(temp3, TEMP_ADC_MIN, TEMP_ADC_MAX, TEMP_MIN, TEMP_MAX) / 1000;

  float temps[3] = {temp1Celsius, temp2Celsius, temp3Celsius};
  
  // monitoring temperature values and toggling individual heaters
  
  uint8_t heater_overheat = 0;
  for (uint8_t i = 0; i < 3; i++)
  {
    if (temps[i] >= 105)
    {
      digitalWrite(TOGGLE_PINS[i], LOW);
    }
    
    if (temps[i] <= 95)
    {
      digitalWrite(TOGGLE_PINS[i], HIGH);
    }

    if (temps[i] >= 115)
    {
      heater_overheat |= 1 << i;
    }
  }
  

  switch (packet.data_id)
  {
    case RC_HEATERBOARD_HEATERTOGGLE_DATA_ID  :

      for (uint8_t i = 0; i < 3; i++)
      {
        if ((packet.data[0] & 1 << i) == 1)
        {
          digitalWrite(TOGGLE_PINS[i], HIGH);
          heater_enabled ^= 1 << i;
        }
        else
        {
          digitalWrite(TOGGLE_PINS[i], LOW);
          heater_enabled ^= 1 << i;
        }
      }
      
  }
  
  // sends data to basestation
  if(millis()-last_update_time >= ROVECOMM_UPDATE_RATE)
  {
    if (heater_overheat)
    {
      RoveComm.write(RC_HEATERBOARD_OVERHEAT_DATA_ID, RC_HEATERBOARD_OVERHEAT_DATA_COUNT, heater_overheat);
    }
    RoveComm.write(RC_HEATERBOARD_HEATERENABLED_DATA_ID, RC_HEATERBOARD_HEATERENABLED_DATA_COUNT, heater_enabled);
    RoveComm.write(RC_HEATERBOARD_THERMOVALUES_DATA_ID, RC_HEATERBOARD_THERMOVALUES_DATA_COUNT, temps);
    last_update_time = millis();
  }
}
