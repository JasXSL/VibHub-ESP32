/*
    This is the project configuration, these values don't change
*/
#ifndef _VH_CONFIGURATION
#define _VH_CONFIGURATION
#include <FS.h>
#include <Arduino.h>
// Bug with arduino unless I undefine these
#undef min
#undef max

namespace Configuration{

    const char VH_VERSION[6]        = "0.2.0";          // Software version
    const char VH_DESC[10]          = "ESP32_1.0";      // Hardware type & version
    const char WIFI_SSID[21]        = "VibHub";         // Network SSID
    const char DEFAULT_HOST[10]     = "vibhub.io";      // Default socket host
    const int  DEFAULT_PORT         = 80;               // Default socket port
    const char SETTINGS_FILE[13]    = "/config.json";   // Default SPIFFS config file

    // Websockets
    const int WEBSOCKET_RECONNECT_TIME = 10000;             // ???

    // StatusLED
    const uint8_t SLED_BRIGHTNESS = 50;                     // Status LED brightness, 0-255

#ifdef FALSE
    // Breadboard GPIO Assignments
    const uint8_t PIN_SLED_R = 23;                          // GPIO pin for RGB (inverse, a PWM of 255 turns off)
    const uint8_t PIN_SLED_G = 19;
    const uint8_t PIN_SLED_B = 22;
    const uint8_t PIN_CONFIG_BUTTON = 18;                   // Configuration pin. Use a pull-up resistor
    const uint8_t PIN_NSLEEP = 13;
    const uint8_t PIN_MOTOR_A_IN1 = 15;
    const uint8_t PIN_MOTOR_A_IN2 = 2;
    const uint8_t PIN_MOTOR_B_IN1 = 0;
    const uint8_t PIN_MOTOR_B_IN2 = 4;
    const uint8_t PIN_MOTOR_C_IN1 = 17;
    const uint8_t PIN_MOTOR_C_IN2 = 16;
    const uint8_t PIN_MOTOR_D_IN1 = 5;
    const uint8_t PIN_MOTOR_D_IN2 = 21;
#else
    // ESP32 1.0 Board GPIO Assignments
    const uint8_t PIN_SLED_R        = 14;   // GPIO pin for RGB (inverse, a PWM of 255 turns off)
    const uint8_t PIN_SLED_G        = 12;
    const uint8_t PIN_SLED_B        = 27;
    const uint8_t PIN_CONFIG_BUTTON = 26;   // Configuration pin. Use a pull-up resistor
    const uint8_t PIN_NSLEEP        = 2;    // Disables motor drivers when low
    const uint8_t PIN_MOTOR_A_IN1   = 13;
    const uint8_t PIN_MOTOR_A_IN2   = 15;
    const uint8_t PIN_MOTOR_B_IN1   = 4;
    const uint8_t PIN_MOTOR_B_IN2   = 16;
    const uint8_t PIN_MOTOR_C_IN1   = 17;
    const uint8_t PIN_MOTOR_C_IN2   = 5;
    const uint8_t PIN_MOTOR_D_IN1   = 19;
    const uint8_t PIN_MOTOR_D_IN2   = 18;
    const uint8_t PIN_MOTOR_FAULT1  = 35;   // Currently unused
    const uint8_t PIN_MOTOR_FAULT2  = 34;   // Currently unused
    const uint8_t PIN_SDA           = 21;   // Currently unused
    const uint8_t PIN_SCL           = 22;   // Currently unused
#endif
    

    // PWM
    const int PWM_FREQ = 16000;                             // PWM Frequency
    const int PWM_RESOLUTION = 8;                           // PWM resolution

    // Button config
    const bool BUTTON_UP = HIGH;                            // Value when the button is not pressed
    const bool BUTTON_DOWN = LOW;                           // Value when the button IS pressed
    const int BUTTON_HOLD_TIME = 3000;                      // Time before it enters config mode
    const int BUTTON_DEBOUNCE = 100;                        // Debounce time


    // Global randomizer function
    // Returns a random value which can be min through and including max
    // min0 max3 would generate 0, 1, 2, or 3
    inline int espRandBetween(int minimum, int maximum){
        float r = (float)esp_random()/UINT32_MAX;
        return (minimum+floor((maximum+1-minimum)*r));
    }


};



#endif