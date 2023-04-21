///////// RoveComm /////////

#include "RoveComm.h"

RoveCommEthernet RoveComm;
rovecomm_packet packet;

//timekeeping variables
IntervalTimer Telemetry;

// declaring Ethernet connection
EthernetServer TCPServer(RC_ROVECOMM_HEATERBOARD_PORT);

///////// Pin Definitions /////////
const uint8_t HEATER_COUNT = 12;
// signals for gate drivers
#define HEATER_TOGGLE_PIN_1     32
#define HEATER_TOGGLE_PIN_2     31
#define HEATER_TOGGLE_PIN_3     30
#define HEATER_TOGGLE_PIN_4     29
#define HEATER_TOGGLE_PIN_5     28
#define HEATER_TOGGLE_PIN_6     A13
#define HEATER_TOGGLE_PIN_7     A12
#define HEATER_TOGGLE_PIN_8     A11
#define HEATER_TOGGLE_PIN_9     A10
#define HEATER_TOGGLE_PIN_10    12
#define HEATER_TOGGLE_PIN_11    11
#define HEATER_TOGGLE_PIN_12    10

uint8_t TOGGLE_PINS[HEATER_COUNT] =    {HEATER_TOGGLE_PIN_1, HEATER_TOGGLE_PIN_2, HEATER_TOGGLE_PIN_3, HEATER_TOGGLE_PIN_4,
                                        HEATER_TOGGLE_PIN_5, HEATER_TOGGLE_PIN_6, HEATER_TOGGLE_PIN_7, HEATER_TOGGLE_PIN_8,
                                        HEATER_TOGGLE_PIN_9, HEATER_TOGGLE_PIN_10, HEATER_TOGGLE_PIN_11, HEATER_TOGGLE_PIN_12};

// data from sensors
#define THERMO_DATA_1       A14
#define THERMO_DATA_2       A15
#define THERMO_DATA_3       A16
#define THERMO_DATA_4       A17
#define THERMO_DATA_5       A0
#define THERMO_DATA_6       A1
#define THERMO_DATA_7       A2
#define THERMO_DATA_8       A3
#define THERMO_DATA_9       A4
#define THERMO_DATA_10      A5
#define THERMO_DATA_11      A6
#define THERMO_DATA_12      A7

uint8_t THERMO_DATA_PINS[HEATER_COUNT] =  {THERMO_DATA_1, THERMO_DATA_2, THERMO_DATA_3,
                                            THERMO_DATA_4, THERMO_DATA_5, THERMO_DATA_6,
                                            THERMO_DATA_7, THERMO_DATA_8, THERMO_DATA_9,
                                            THERMO_DATA_10, THERMO_DATA_11, THERMO_DATA_12};
// overheat LEDs
#define HEATER_1_OVERHEAT_LED   9
#define HEATER_2_OVERHEAT_LED   8
#define HEATER_3_OVERHEAT_LED   7
#define HEATER_4_OVERHEAT_LED   6
#define HEATER_5_OVERHEAT_LED   5
#define HEATER_6_OVERHEAT_LED   4
#define HEATER_7_OVERHEAT_LED   3
#define HEATER_8_OVERHEAT_LED   2
#define HEATER_9_OVERHEAT_LED   1
#define HEATER_10_OVERHEAT_LED  0
#define HEATER_11_OVERHEAT_LED  A8
#define HEATER_12_OVERHEAT_LED  A9

uint8_t HEATER_OVERHEAT_LEDS[HEATER_COUNT] =  {HEATER_1_OVERHEAT_LED, HEATER_2_OVERHEAT_LED, HEATER_3_OVERHEAT_LED,
                                                HEATER_4_OVERHEAT_LED, HEATER_5_OVERHEAT_LED, HEATER_6_OVERHEAT_LED,
                                                HEATER_7_OVERHEAT_LED, HEATER_8_OVERHEAT_LED, HEATER_9_OVERHEAT_LED,
                                                HEATER_10_OVERHEAT_LED, HEATER_11_OVERHEAT_LED, HEATER_12_OVERHEAT_LED};

///////// Variables /////////

// conversions from ADC values to Celsius
const float TEMP_ADC_MIN = 0;
const float TEMP_ADC_MAX = 123; //bits
const float TEMP_MIN = 0;
const float TEMP_MAX = 23500; // mdeg C
const uint16_t tempScalar = 1060;

// enable command for individual heaters
uint16_t heater_enabled = 0;
float tempsCelsius[HEATER_COUNT];
float TEMP_ADC_READINGS[HEATER_COUNT];

// bitmask for overheating
uint16_t heater_overheat = 0;

void telemetry();
