#include "RoveComm.h"


// conversions from ADC values to Celsius
float TEMP_ADC_MIN = 0;
float TEMP_ADC_MAX = 0;
float TEMP_MIN = 0;
float TEMP_MAX = 0;

// signals for gate drivers
#define TEMP_SENSE_PIN_1    2
#define TEMP_SENSE_PIN_2    6
#define TEMP_SENSE_PIN_3    9

int TEMP_PINS[3] = {TEMP_SENSE_PIN_1, TEMP_SENSE_PIN_2, TEMP_SENSE_PIN_3};

// data from sensors
#define TEMP_SENSE_DATA_1   A17
#define TEMP_SENSE_DATA_2   A16
#define TEMP_SENSE_DATA_3   A15


/////// RoveComm ///////

RoveCommEthernet RoveComm;
rovecomm_packet packet;


//timekeeping variables
uint32_t last_update_time;

// declaring Ethernet connection
EthernetServer TCPServer(RC_ROVECOMM_HEATERBOARD_PORT);

