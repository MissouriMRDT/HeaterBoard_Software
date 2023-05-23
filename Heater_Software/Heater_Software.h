#ifndef HEATERSOFTWARE_H
#define HEATERSOFTWARE_H

#include "PinAssignments.h"

#include <RoveComm.h>


// RoveComm
EthernetServer TCPServer(RC_ROVECOMM_ETHERNET_TCP_PORT);
RoveCommEthernet RoveComm;

// Telemetry
IntervalTimer Telemetry;
#define TELEMETRY_PERIOD 150000


const uint8_t HEATER_COUNT = 12;

uint8_t TOGGLE_PINS[HEATER_COUNT] =    {TOGGLE_PIN_1, TOGGLE_PIN_2, TOGGLE_PIN_3, TOGGLE_PIN_4,
                                        TOGGLE_PIN_5, TOGGLE_PIN_6, TOGGLE_PIN_7, TOGGLE_PIN_8,
                                        TOGGLE_PIN_9, TOGGLE_PIN_10, TOGGLE_PIN_11, TOGGLE_PIN_12};


uint8_t THERMO_DATA_PINS[HEATER_COUNT] =  {THERMO_DATA_1, THERMO_DATA_2, THERMO_DATA_3,
                                            THERMO_DATA_4, THERMO_DATA_5, THERMO_DATA_6,
                                            THERMO_DATA_7, THERMO_DATA_8, THERMO_DATA_9,
                                            THERMO_DATA_10, THERMO_DATA_11, THERMO_DATA_12};


uint8_t OVERHEAT_LEDS[HEATER_COUNT] =  {OVERHEAT_LED_1, OVERHEAT_LED_2, OVERHEAT_LED_3, OVERHEAT_LED_4, OVERHEAT_LED_5, OVERHEAT_LED_6,
                                        OVERHEAT_LED_7, OVERHEAT_LED_8, OVERHEAT_LED_9, OVERHEAT_LED_10, OVERHEAT_LED_11, OVERHEAT_LED_12};

// Temperature conversion variables
const uint16_t ADC_MIN = 230;
const uint16_t ADC_MAX = 450;
const float TEMP_MIN = 25.0; // degrees C
const float TEMP_MAX = 100.0; // degrees C
const float TEMP_SCALE = (TEMP_MAX - TEMP_MIN) / (ADC_MAX - ADC_MIN);


// Control variables
#define MAX_HEATERS_ENABLED 6
uint16_t heater_enable = 0; // bitmask
uint16_t heater_overheat = 0; // bitmask
float heater_temp_read[HEATER_COUNT];
float heater_temp_set[HEATER_COUNT] = { 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100 };


// Methods
void readTemp();
void telemetry();


#endif