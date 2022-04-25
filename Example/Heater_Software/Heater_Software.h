///////// RoveComm /////////

#include "RoveComm.h"

RoveCommEthernet RoveComm;
rovecomm_packet packet;

//timekeeping variables
uint32_t last_update_time;
IntervalTimer telemetry();

// declaring Ethernet connection
EthernetServer TCPServer(RC_ROVECOMM_HEATERBOARD_PORT);



///////// Pin Definitions /////////

// signals for gate drivers
#define HEATER_TOGGLE_PIN_1    2
#define HEATER_TOGGLE_PIN_2    6
#define HEATER_TOGGLE_PIN_3    9

uint8_t TOGGLE_PINS[3] = {HEATER_TOGGLE_PIN_1, HEATER_TOGGLE_PIN_2, HEATER_TOGGLE_PIN_3};

// data from sensors
#define THERMO_DATA_1   A17
#define THERMO_DATA_2   A16
#define THERMO_DATA_3   A15

// overheat LEDs
#define HEATER_1_OVERHEAT_LED 0
#define HEATER_2_OVERHEAT_LED 1
#define HEATER_3_OVERHEAT_LED 7

uint8_t HEATER_OVERHEAT_LEDS[3] = {HEATER_1_OVERHEAT_LED, HEATER_2_OVERHEAT_LED, HEATER_3_OVERHEAT_LED};



///////// Variables /////////

// conversions from ADC values to Celsius
float TEMP_ADC_MIN = 0;
float TEMP_ADC_MAX = 4096; //bits
float TEMP_MIN = 0;
float TEMP_MAX = 160000; // mdeg C

// enable command for individual heaters
uint8_t heater_enabled = 0;

// bitmask for overheating
uint8_t heater_overheat = 0;


void telemetry();