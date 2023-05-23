// Science Heater Software
// MRDT 2023
// made by Grant Brinker
// #RoveSoHard

#include "Heater_Software.h"


void setup() {
    Serial.begin(115200);

    for (uint8_t i = 0; i < HEATER_COUNT; i++) {
        pinMode(TOGGLE_PINS[i], OUTPUT);            // inputs to MOSFET circuits for enabling/disabling heaters
        pinMode(THERMO_DATA_PINS[i], INPUT);        // data from temperature sensors
        pinMode(OVERHEAT_LEDS[i], OUTPUT);   // sets overheat LED pins
    }
    
    Serial.println("Initializing RoveComm...");
    RoveComm.begin(RC_HEATERBOARD_FIRSTOCTET, RC_HEATERBOARD_SECONDOCTET, RC_HEATERBOARD_THIRDOCTET, RC_HEATERBOARD_FOURTHOCTET, &TCPServer);
    Serial.println("Complete");

    Telemetry.begin(telemetry, TELEMETRY_PERIOD);
}


void loop() {   
    readTemp();
    
    rovecomm_packet packet = RoveComm.read();
    switch (packet.data_id) {

        // Turn heaters on/off
        case RC_HEATERBOARD_HEATERTOGGLE_DATA_ID:
        {
            uint16_t data = ((uint16_t*) packet.data)[0];

            uint8_t enabledCount = 0;
            for (uint8_t i = 0; i < HEATER_COUNT; i++) {
                if (data & (1<<i)) enabledCount++;
            }

            if (enabledCount > MAX_HEATERS_ENABLED) heater_enable = 0;
            else heater_enable = data;

            break;
        }

        case RC_HEATERBOARD_HEATERSETTEMP_DATA_ID:
        {
            float* data = (float*) packet.data;

            for (int i = 0; i < HEATER_COUNT; i++) {
                heater_temp_set[i] = data[i];
            }

            break;
        }
    }

    // Check for Overheat
    for (uint8_t i = 0; i < HEATER_COUNT; i++) {
        if (heater_temp_read[i] > 115) heater_overheat |= (1<<i);
        else if (heater_temp_read[i] < 105) heater_overheat &= ~(1<<i);

        digitalWrite(OVERHEAT_LEDS[i], (heater_overheat & (1<<i)));
    }

    // Output to Heater Pins
    for (uint8_t i = 0; i < HEATER_COUNT; i++) {
        bool enable = (heater_enable & (1<<i)) && (heater_temp_read[i] < heater_temp_set[i]);

        digitalWrite(TOGGLE_PINS[i], enable);
    }
}


void readTemp() {
    for (uint8_t i = 0; i < HEATER_COUNT; i++) {
        uint16_t adc = analogRead(THERMO_DATA_PINS[i]);
        heater_temp_read[i] = (adc - ADC_MIN) * TEMP_SCALE + TEMP_MIN;
    }
}


void telemetry() {
    RoveComm.write(RC_HEATERBOARD_OVERHEAT_DATA_ID, RC_HEATERBOARD_OVERHEAT_DATA_COUNT, heater_overheat);
    RoveComm.write(RC_HEATERBOARD_HEATERENABLED_DATA_ID, RC_HEATERBOARD_HEATERENABLED_DATA_COUNT, heater_enable);
    RoveComm.write(RC_HEATERBOARD_THERMOVALUES_DATA_ID, RC_HEATERBOARD_THERMOVALUES_DATA_COUNT, heater_temp_read);
}
