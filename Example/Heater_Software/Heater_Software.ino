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
    RoveComm.begin(RC_HEATERBOARD_FIRSTOCTET, RC_HEATERBOARD_SECONDOCTET, RC_HEATERBOARD_THIRDOCTET, RC_HEATERBOARD_FOURTHOCTET, &TCPServer);
    delay(100);
    Telemetry.begin(telemetry, 1500000);

    Serial.println("Started: ");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void loop()
{   
    packet = RoveComm.read();
    // monitoring temperature values and toggling individual heaters
    switch (packet.data_id)
    {
    case RC_HEATERBOARD_HEATERTOGGLE_DATA_ID:
        heater_enabled = (uint16_t)packet.data[0];
        toggleHeaters();
        break;
    }


    //readTemp();
    regulateTemp();
    telemetry();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void toggleHeaters()
{
    //numHeatersEnabled=0;
    for (uint8_t i = 0; i < HEATER_COUNT; i++)
        {
            if (heater_enabled & (1 << i))
            {
                /*if (numHeatersEnabled <= maxHeatersEnabled)
                {
                    digitalWrite(TOGGLE_PINS[i], HIGH);
                    numHeatersEnabled++;
                    Serial.print("Enabled heater ");
                    Serial.println(i+1);
                }
                else
                {
                    Serial.println("Warning: Running more than 6 heaters will kill the board!");
                }*/
                digitalWrite(TOGGLE_PINS[i], HIGH);
            }

            else
            {
                digitalWrite(TOGGLE_PINS[i], LOW);
                //numHeatersEnabled--;
                Serial.print("Disabled heater ");
                Serial.println(i+1);
            }
        }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void readTemp() // read temperature sensor data and convert it to Celcius
{
    for (uint8_t i = 0; i < HEATER_COUNT; i++)
    {
        // temperature data from each sensor
        TEMP_ADC_READINGS[i] = analogRead(THERMO_DATA_PINS[i]);
        // changes ADC values from temperature sensors to Celsius
        tempsCelsius[i] = (tempScalar * map(TEMP_ADC_READINGS[i], TEMP_ADC_MIN, TEMP_ADC_MAX, TEMP_MIN, TEMP_MAX) / 1000);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void regulateTemp() // turn off heaters if they get a little spicy and enabled overheat LEDs if they're very spicy
{
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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void telemetry()    // send enabled heaters, temperatures, and overheating blocks to basestation
{
    if (heater_overheat) RoveComm.write(RC_HEATERBOARD_OVERHEAT_DATA_ID, RC_HEATERBOARD_OVERHEAT_DATA_COUNT, heater_overheat);
    RoveComm.write(RC_HEATERBOARD_HEATERENABLED_DATA_ID, RC_HEATERBOARD_HEATERENABLED_DATA_COUNT, heater_enabled);
    RoveComm.write(RC_HEATERBOARD_THERMOVALUES_DATA_ID, RC_HEATERBOARD_THERMOVALUES_DATA_COUNT, tempsCelsius);
}