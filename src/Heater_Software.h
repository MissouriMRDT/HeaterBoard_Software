#include "RoveComm.h"

// temperature data from each sensor
float temp1 = analogRead(PIN_A17);
float temp2 = analogRead(PIN_A16);
float temp3 = analogRead(PIN_A15);

// conversions from ADC values to Celsius
float TEMP_ADC_MIN = 0;
float TEMP_ADC_MAX = 0;
float TEMP_MIN = 0;
float TEMP_MAX = 0;

// signals for gate drivers
pinMode(PIN_2, OUTPUT);
pinMode(PIN_6, OUTPUT);
pinMode(PIN_9, OUTPUT);
// data from sensors
pinMode(PIN_A17, INPUT);
pinMode(PIN_A16, INPUT);
pinMode(PIN_A15, INPUT);


