// Science Heater Software      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 2022 Prometheus              ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// made by Grant Brinker        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// #RoveSoHard                  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Heater_Software.h"

void setup()
{
    ///////// Pin Assignments /////////

    // inputs to MOSFET circuits for enabling/disabling heaters
    pinMode(TOGGLE_PINS[0], OUTPUT);
    pinMode(TOGGLE_PINS[1], OUTPUT);
    pinMode(TOGGLE_PINS[2], OUTPUT);
    pinMode(TOGGLE_PINS[3], OUTPUT);
    pinMode(TOGGLE_PINS[4], OUTPUT);
    pinMode(TOGGLE_PINS[5], OUTPUT);
    pinMode(TOGGLE_PINS[6], OUTPUT);
    pinMode(TOGGLE_PINS[7], OUTPUT);
    pinMode(TOGGLE_PINS[8], OUTPUT);
    pinMode(TOGGLE_PINS[9], OUTPUT);
    pinMode(TOGGLE_PINS[10], OUTPUT);
    pinMode(TOGGLE_PINS[11], OUTPUT);

    // data from temperature sensors
    pinMode(THERMO_DATA_1, INPUT);
    pinMode(THERMO_DATA_2, INPUT);
    pinMode(THERMO_DATA_3, INPUT);
    pinMode(THERMO_DATA_4, INPUT);
    pinMode(THERMO_DATA_5, INPUT);
    pinMode(THERMO_DATA_6, INPUT);
    pinMode(THERMO_DATA_7, INPUT);
    pinMode(THERMO_DATA_8, INPUT);
    pinMode(THERMO_DATA_9, INPUT);
    pinMode(THERMO_DATA_10, INPUT);
    pinMode(THERMO_DATA_11, INPUT);
    pinMode(THERMO_DATA_12, INPUT);

    // sets overheat LED pins
    pinMode(HEATER_OVERHEAT_LEDS[0], OUTPUT);
    pinMode(HEATER_OVERHEAT_LEDS[1], OUTPUT);
    pinMode(HEATER_OVERHEAT_LEDS[2], OUTPUT);
    pinMode(HEATER_OVERHEAT_LEDS[3], OUTPUT);
    pinMode(HEATER_OVERHEAT_LEDS[4], OUTPUT);
    pinMode(HEATER_OVERHEAT_LEDS[5], OUTPUT);
    pinMode(HEATER_OVERHEAT_LEDS[6], OUTPUT);
    pinMode(HEATER_OVERHEAT_LEDS[7], OUTPUT);
    pinMode(HEATER_OVERHEAT_LEDS[8], OUTPUT);
    pinMode(HEATER_OVERHEAT_LEDS[9], OUTPUT);
    pinMode(HEATER_OVERHEAT_LEDS[10], OUTPUT);
    pinMode(HEATER_OVERHEAT_LEDS[11], OUTPUT);

    Serial.begin(115200);

    ///////// RoveComm, Serial, and Timing /////////

    // Set up RoveComm with the correct IP and the TCP server
    RoveComm.begin(RC_HEATERBOARD_FOURTHOCTET, &TCPServer, RC_ROVECOMM_HEATERBOARD_MAC);
    delay(100);
    Telemetry.begin(telemetry, 1500000);

    Serial.println("Started: ");
}



void loop()
{
    ///////// Basestation Command Receiver /////////
    
    packet = RoveComm.read();
    // monitoring temperature values and toggling individual heaters
    switch (packet.data_id)
    {
    case RC_HEATERBOARD_HEATERTOGGLE_DATA_ID:
        heater_enabled = (uint8_t)packet.data[0];

        for (uint8_t i = 0; i < 12; i++)
        {
            if (heater_enabled & (1 << i))
            {
                digitalWrite(TOGGLE_PINS[i], HIGH);
                Serial.println("Enabled");
            }

            else
            {
                digitalWrite(TOGGLE_PINS[i], LOW);
                Serial.println("Disabled");
            }
        }
        break;
    }

    ///////// Temperature Readings and Conversions /////////

    // temperature data from each sensor
    temp1 = analogRead(THERMO_DATA_1);
    temp2 = analogRead(THERMO_DATA_2);
    temp3 = analogRead(THERMO_DATA_3);
    temp4 = analogRead(THERMO_DATA_4);
    temp5 = analogRead(THERMO_DATA_5);
    temp6 = analogRead(THERMO_DATA_6);
    temp7 = analogRead(THERMO_DATA_7);
    temp8 = analogRead(THERMO_DATA_8);
    temp9 = analogRead(THERMO_DATA_9);
    temp10 = analogRead(THERMO_DATA_10);
    temp11 = analogRead(THERMO_DATA_11);
    temp12 = analogRead(THERMO_DATA_12);

    // changes ADC values from temperature sensors to Celsius
    tempsCelsius[0] = (map(temp1, TEMP_ADC_MIN, TEMP_ADC_MAX, TEMP_MIN, TEMP_MAX) / 1000);
    tempsCelsius[1] = (map(temp2, TEMP_ADC_MIN, TEMP_ADC_MAX, TEMP_MIN, TEMP_MAX) / 1000);
    tempsCelsius[2] = (map(temp3, TEMP_ADC_MIN, TEMP_ADC_MAX, TEMP_MIN, TEMP_MAX) / 1000);
    tempsCelsius[3] = (map(temp1, TEMP_ADC_MIN, TEMP_ADC_MAX, TEMP_MIN, TEMP_MAX) / 1000);
    tempsCelsius[0] = (map(temp1, TEMP_ADC_MIN, TEMP_ADC_MAX, TEMP_MIN, TEMP_MAX) / 1000);
    tempsCelsius[0] = (map(temp1, TEMP_ADC_MIN, TEMP_ADC_MAX, TEMP_MIN, TEMP_MAX) / 1000);
    tempsCelsius[0] = (map(temp1, TEMP_ADC_MIN, TEMP_ADC_MAX, TEMP_MIN, TEMP_MAX) / 1000);
    tempsCelsius[0] = (map(temp1, TEMP_ADC_MIN, TEMP_ADC_MAX, TEMP_MIN, TEMP_MAX) / 1000);
    tempsCelsius[0] = (map(temp1, TEMP_ADC_MIN, TEMP_ADC_MAX, TEMP_MIN, TEMP_MAX) / 1000);
    tempsCelsius[0] = (map(temp1, TEMP_ADC_MIN, TEMP_ADC_MAX, TEMP_MIN, TEMP_MAX) / 1000);
    tempsCelsius[0] = (map(temp1, TEMP_ADC_MIN, TEMP_ADC_MAX, TEMP_MIN, TEMP_MAX) / 1000);
    tempsCelsius[0] = (map(temp1, TEMP_ADC_MIN, TEMP_ADC_MAX, TEMP_MIN, TEMP_MAX) / 1000);

    ///////// Temperature Regulation Logic /////////

    for (uint8_t i = 0; i < 3; i++)
    {
        if (tempsCelsius[i] >= 105 && (heater_enabled & (1 << i)))
        {
            digitalWrite(TOGGLE_PINS[i], LOW);

            if (tempsCelsius[i] >= 115)
            {
                heater_overheat |= 1 << i; // OR the overheat var with 1 to make it 1
                Serial.println("over");
                digitalWrite(HEATER_OVERHEAT_LEDS[i], HIGH);
            }
        }

        if (tempsCelsius[i] <= 105)
        {
            heater_overheat &= !(1 << i);
            // if a heater is overheated, its value in the bit array is 1
            // !(1 << i) sets every 0 in the bit arrary to 1 and every one to 0
            // if heater_overheat == 1, !(1 << i) yields a 0 and AND with 0 always yields 0
            // if heater_overheat == 0, the AND will yield 0, turning off the overheat variable
            digitalWrite(HEATER_OVERHEAT_LEDS[i], LOW);
        }
    }
}

void telemetry()
{
    if (heater_overheat)
    {
        RoveComm.write(RC_HEATERBOARD_OVERHEAT_DATA_ID, RC_HEATERBOARD_OVERHEAT_DATA_COUNT, heater_overheat);
    }
    RoveComm.write(RC_HEATERBOARD_HEATERENABLED_DATA_ID, RC_HEATERBOARD_HEATERENABLED_DATA_COUNT, heater_enabled);
    RoveComm.write(RC_HEATERBOARD_THERMOVALUES_DATA_ID, RC_HEATERBOARD_THERMOVALUES_DATA_COUNT, tempsCelsius);
}