///////// RoveComm /////////

#include "RoveComm.h"

RoveCommEthernet RoveComm;
rovecomm_packet packet;

//timekeeping variables
IntervalTimer Telemetry;

// declaring Ethernet connection
EthernetServer TCPServer(RC_ROVECOMM_HEATERBOARD_PORT);

///////// Pin Definitions /////////

// signals for gate drivers
#define HEATER_TOGGLE_PIN_1    2
#define HEATER_TOGGLE_PIN_2    6
#define HEATER_TOGGLE_PIN_3    9
#define HEATER_TOGGLE_PIN_4
#define HEATER_TOGGLE_PIN_5
#define HEATER_TOGGLE_PIN_6
#define HEATER_TOGGLE_PIN_7
#define HEATER_TOGGLE_PIN_8
#define HEATER_TOGGLE_PIN_9
#define HEATER_TOGGLE_PIN_10
#define HEATER_TOGGLE_PIN_11
#define HEATER_TOGGLE_PIN_12

uint16_t TOGGLE_PINS[12] = {HEATER_TOGGLE_PIN_1, HEATER_TOGGLE_PIN_2, HEATER_TOGGLE_PIN_3, HEATER_TOGGLE_PIN_4, HEATER_TOGGLE_PIN_5, HEATER_TOGGLE_PIN_6, HEATER_TOGGLE_PIN_7, HEATER_TOGGLE_PIN_8, HEATER_TOGGLE_PIN_9, HEATER_TOGGLE_PIN_10, HEATER_TOGGLE_PIN_11, HEATER_TOGGLE_PIN_12};

// data from sensors
#define THERMO_DATA_1   A17
#define THERMO_DATA_2   A16
#define THERMO_DATA_3   A15
#define THERMO_DATA_4   A17
#define THERMO_DATA_5   A17
#define THERMO_DATA_6   A17
#define THERMO_DATA_7   A17
#define THERMO_DATA_8   A17
#define THERMO_DATA_9   A17
#define THERMO_DATA_10   A17
#define THERMO_DATA_11   A17
#define THERMO_DATA_12   A17

// overheat LEDs
#define HEATER_1_OVERHEAT_LED 0
#define HEATER_2_OVERHEAT_LED 1
#define HEATER_3_OVERHEAT_LED 7
#define HEATER_4_OVERHEAT_LED 0
#define HEATER_5_OVERHEAT_LED 0
#define HEATER_6_OVERHEAT_LED 0
#define HEATER_7_OVERHEAT_LED 0
#define HEATER_8_OVERHEAT_LED 0
#define HEATER_9_OVERHEAT_LED 0
#define HEATER_10_OVERHEAT_LED 0
#define HEATER_11_OVERHEAT_LED 0
#define HEATER_12_OVERHEAT_LED 0

uint16_t HEATER_OVERHEAT_LEDS[12] = {HEATER_1_OVERHEAT_LED, HEATER_2_OVERHEAT_LED, HEATER_3_OVERHEAT_LED, HEATER_4_OVERHEAT_LED, HEATER_5_OVERHEAT_LED, HEATER_6_OVERHEAT_LED, HEATER_7_OVERHEAT_LED, HEATER_8_OVERHEAT_LED, HEATER_9_OVERHEAT_LED, HEATER_10_OVERHEAT_LED, HEATER_11_OVERHEAT_LED, HEATER_12_OVERHEAT_LED};

///////// Variables /////////

// conversions from ADC values to Celsius
float TEMP_ADC_MIN = 0;
float TEMP_ADC_MAX = 4095; //bits
float TEMP_MIN = 0;
float TEMP_MAX = 144000; // mdeg C

// enable command for individual heaters
uint16_t heater_enabled = 0;
float tempsCelsius[12];
float temp1 = 0;
float temp2 = 0;
float temp3 = 0;
float temp4 = 0;
float temp5 = 0;
float temp6 = 0;
float temp7 = 0;
float temp8 = 0;
float temp9 = 0;
float temp10 = 0;
float temp11 = 0;
float temp12 = 0;

// bitmask for overheating
uint16_t heater_overheat = 0;

void telemetry();