#include "Heater_Software.h"

void setup()
{
  // inputs to MOSFET circuits
  pinMode(TEMP_SENSE_PIN_1, OUTPUT);
  pinMode(TEMP_SENSE_PIN_2, OUTPUT);
  pinMode(TEMP_SENSE_PIN_3, OUTPUT);
  
  // data from temperature sensors
  pinMode(TEMP_SENSE_DATA_1, INPUT);
  pinMode(TEMP_SENSE_DATA_2, INPUT);
  pinMode(TEMP_SENSE_DATA_3, INPUT);


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
  float temp1 = analogRead(TEMP_SENSE_DATA_1);
  float temp2 = analogRead(TEMP_SENSE_DATA_2);
  float temp3 = analogRead(TEMP_SENSE_DATA_3);

  float temp1Celsius = map(temp1, TEMP_ADC_MIN, TEMP_ADC_MAX, TEMP_MIN, TEMP_MAX) / 1000;
  float temp2Celsius = map(temp2, TEMP_ADC_MIN, TEMP_ADC_MAX, TEMP_MIN, TEMP_MAX) / 1000;
  float temp3Celsius = map(temp3, TEMP_ADC_MIN, TEMP_ADC_MAX, TEMP_MIN, TEMP_MAX) / 1000;

  float temps[3] = {temp1Celsius, temp2Celsius, temp3Celsius};
  
  // monitoring temperature values and toggling individual heaters
  

  for (int i == 0; i < 3; i++)
  {
    switch(temps[i])
    {
      case >= 105 :
        digitalWrite(TEMP_PINS[i], LOW);
        break;
      case <= 95  :
        digitalWrite(TEMP_PINS[i], HIGH);
        break;
      case >= 115 :
        Rovecomm.write(RC_HEATERBOARD_OVERHEAT_DATA_ID, temps); // sends warning to basestation
        break;
    }
  }

  switch (packet.data_id)
  {
    case RC_HEATERBOARD_HEATERTOGGLE_DATA_ID  :
      uint8_t* heatertoggle = packet.data[0];
      uint8_t* temp = 0;
      if ((heatertoggle & 1 << temp) == true)
      {
        digitalWrite(TEMP_SENSE_PIN_1, HIGH);
      }
      else
      {
        digitalWrite(TEMP_SENSE_PIN_1, LOW);
      }
      temp++;
      
      if ((heatertoggle & 1 << temp) == true)
      {
        digitalWrite(TEMP_SENSE_PIN_2, HIGH);
      }
      else
      {
        digitalWrite(TEMP_SENSE_PIN_2, LOW);
      }
      temp++;
      
      if ((heatertoggle & 1 << temp) == true)
      {
        digitalWrite(TEMP_SENSE_PIN_3, HIGH);
      }
      else
      {
        digitalWrite(TEMP_SENSE_PIN_3, LOW);
      }
      temp++;
  }
  
  // sends data to basestation
  if(millis()-last_update_time >= ROVECOMM_UPDATE_RATE)
  {
    Rovecomm.write(RC_HEATERBOARD_HEATERENABLED_DATA_ID, RC_HEATERBOARD_THERMO_VALUES_DATA_ID, RC_HEATERBOARD_THERMO_VALUES_DATA_COUNT, temps);
    last_update_time = millis();
  }
}
