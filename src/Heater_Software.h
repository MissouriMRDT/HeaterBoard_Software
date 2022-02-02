#include "RoveComm.h"

// temperature data from each sensor
float temp1 = analogRead(PIN_A17);
float temp2 = analogRead(PIN_A16);
float temp3 = analogRead(PIN_A15);
float temp1Celsius = map(temp1, TEMP_ADC_MIN, TEMP_ADC_MAX, TEMP_MIN, TEMP_MAX);
float temp2Celsius = map(temp2, TEMP_ADC_MIN, TEMP_ADC_MAX, TEMP_MIN, TEMP_MAX);
float temp3Celsius = map(temp3, TEMP_ADC_MIN, TEMP_ADC_MAX, TEMP_MIN, TEMP_MAX);

// conversions from ADC values to Celsius
float TEMP_ADC_MIN = 0;
float TEMP_ADC_MAX = 0;
float TEMP_MIN = 0;
float TEMP_MAX = 0;

// signals for gate drivers
#define TEMP_SENSE_PIN_1    2
#define TEMP_SENSE_PIN_2    6
#define TEMP_SENSE_PIN_3    9

// data from sensors
#define TEMP_SENSE_DATA_1   A17
#define TEMP_SENSE_DATA_2   A16
#define TEMP_SENSE_DATA_3   A15


/////// RoveComm ///////

RoveCommEthernet RoveComm;
rovecomm_packet packet;

int16_t motor_speed[6] = {-500, 200, 740, -720, 10, -182};

//timekeeping variables
uint32_t last_update_time;

//declare the Ethernet Server in the top level sketch with the requisite port ID any time you want to use RoveComm
EthernetServer TCPServer(RC_ROVECOMM_DRIVEBOARD_PORT);

last_update_time = millis();
