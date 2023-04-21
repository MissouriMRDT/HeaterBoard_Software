// Science Heater Software      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MRDT 2023                    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// made by Grant Brinker        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// #RoveSoHard                  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Heater_Software.h"

void setup()
{
    ///////// Pin Assignments /////////
    for (uint8_t i = 0; i < HEATER_COUNT; i++)
    {
        pinMode(TOGGLE_PINS[i], OUTPUT);            // inputs to MOSFET circuits for enabling/disabling heaters
        pinMode(THERMO_DATA_PINS[i], INPUT);        // data from temperature sensors
        pinMode(HEATER_OVERHEAT_LEDS[i], OUTPUT);   // sets overheat LED pins
    }
    

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
        heater_enabled = (uint16_t)packet.data[0];

        for (uint8_t i = 0; i < HEATER_COUNT; i++)
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

   
    for (uint8_t i = 0; i < HEATER_COUNT; i++)
    {
        // temperature data from each sensor
        TEMP_ADC_READINGS[i] = analogRead(THERMO_DATA_PINS[i]);
        // changes ADC values from temperature sensors to Celsius
        tempsCelsius[i] = (tempScalar * map(TEMP_ADC_READINGS[i], TEMP_ADC_MIN, TEMP_ADC_MAX, TEMP_MIN, TEMP_MAX) / 1000);
    }
    

    ///////// Temperature Regulation Logic /////////

    for (uint8_t i = 0; i < HEATER_COUNT; i++)
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
    if (heater_overheat) RoveComm.write(RC_HEATERBOARD_OVERHEAT_DATA_ID, RC_HEATERBOARD_OVERHEAT_DATA_COUNT, heater_overheat);
    RoveComm.write(RC_HEATERBOARD_HEATERENABLED_DATA_ID, RC_HEATERBOARD_HEATERENABLED_DATA_COUNT, heater_enabled);
    RoveComm.write(RC_HEATERBOARD_THERMOVALUES_DATA_ID, RC_HEATERBOARD_THERMOVALUES_DATA_COUNT, tempsCelsius);
}