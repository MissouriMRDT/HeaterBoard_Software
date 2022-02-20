#include "RoveComm.h"


// conversions from ADC values to Celsius
float TEMP_ADC_MIN = 0;
float TEMP_ADC_MAX = 4096; //bits
float TEMP_MIN = 0;
float TEMP_MAX = 160000; // mdeg C

// signals for gate drivers
#define HEATER_TOGGLE_PIN_1    2
#define HEATER_TOGGLE_PIN_2    6
#define HEATER_TOGGLE_PIN_3    9

uint8_t TOGGLE_PINS[3] = {HEATER_TOGGLE_PIN_1, HEATER_TOGGLE_PIN_2, HEATER_TOGGLE_PIN_3};
uint8_t toggleOn = 0;

// data from sensors
#define THERMO_DATA_1   A17
#define THERMO_DATA_2   A16
#define THERMO_DATA_3   A15


/////// RoveComm ///////

RoveCommEthernet RoveComm;
rovecomm_packet packet;

uint8_t heater_enabled = 0;

//timekeeping variables
uint32_t last_update_time;

// declaring Ethernet connection
EthernetServer TCPServer(RC_ROVECOMM_HEATERBOARD_PORT);

