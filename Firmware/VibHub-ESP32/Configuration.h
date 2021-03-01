/*
    This is the project configuration, these values don't change
*/
#ifndef _VH_CONFIGURATION
#define _VH_CONFIGURATION
#include <FS.h>
#include <Arduino.h>
#include "Configuration.h"
// Bug with arduino unless I undefine these
#undef min
#undef max


// Capabilities
#define CTASK_PWM_BASIC "p"                     // Capable of handling motor updates in one go
#define CTASK_PWM_SPECIFIC "ps"                 // Capable of handling individual motor updates
#define CTASK_PROGRAMS "vib"                    // Capable of handling programs
#define CTASK_APP_OFFLINE "app_offline"         // Capable of doing something when app goes offline
#define CTASK_CUSTOM_TO_DEVICE "dCustom"        // Capable of receiving custom data from the app
#define CTASK_DEVICE_TO_CUSTOM "aCustom"        // Capable of sending custom data to the app
class Capability{
    public:
        Capability( char* t, bool modded = false ){
            type = t;
            modified = modded;            
        }
        char *type;
        bool modified;
};


namespace Configuration{

    const char VH_VERSION[]         = "0.3.0";          // Software version
    const char VH_HWVERSION[]       = "ESP32_2.1";      // Hardware type & version
    const char WIFI_SSID[]          = "VibHub";         // Network SSID
    const char DEFAULT_HOST[]       = "vibhub.io";      // Default socket host
    const int  DEFAULT_PORT         = 443;               // Default socket port
    const char API_URL[]            = "/socket.io/?transport=websocket";
    const char SETTINGS_FILE[]      = "/config.json";   // Default SPIFFS config file

    // Metadata
    const uint8_t NUM_MOTORS        = 4;
    const uint8_t NR_CAPABILITIES   = 3;
    const Capability CAPABILITIES[NR_CAPABILITIES] = {
        Capability(CTASK_PWM_BASIC),
        Capability(CTASK_PWM_SPECIFIC),
        Capability(CTASK_PROGRAMS)
    };

    // Websockets
    const int WEBSOCKET_RECONNECT_TIME = 3000;          // How long to wait after a connection failed

    // Status LED
    const uint8_t SLED_BRIGHTNESS   = 50;   // Status LED brightness, 0-255

    // ESP32 2.0a/2.1 Board GPIO Assignments
    const uint8_t PIN_SLED_R        = 14;   // GPIO pin for RGB (inverse, a PWM of 255 turns off)
    const uint8_t PIN_SLED_G        = 12;
    const uint8_t PIN_SLED_B        = 27;
    const uint8_t PIN_SLED2_R       = 32;   // 2.1 only
    const uint8_t PIN_SLED2_G       = 33;   // 2.1 only
    const uint8_t PIN_SLED2_B       = 25;   // 2.1 only
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
    const uint8_t PIN_POWEROFF      = 23;   // Signal high turns off main power
    

    // PWM
    const int PWM_FREQ              = 12000; // PWM Frequency
    const int PWM_RESOLUTION        = 8;     // PWM resolution

    // Button config
    const bool BUTTON_UP            = HIGH;  // Value when the button is not pressed
    const bool BUTTON_DOWN          = LOW;   // Value when the button IS pressed
    const int BUTTON_HOLD_TIME      = 4000;  // Time before it enters config mode
    const uint8_t BUTTON_DEBOUNCE   = 100;   // Debounce time


    // Global randomizer function
    // Returns a random value which can be min through and including max
    // min0 max3 would generate 0, 1, 2, or 3
    inline int espRandBetween(int minimum, int maximum){
        float r = (float)esp_random()/UINT32_MAX;
        return (minimum+floor((maximum+1-minimum)*r));
    }

    const char ROOTCA[] = \
        "-----BEGIN CERTIFICATE-----\n" \
        "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\n" \
        "nLRbwHOoq7hHwg==\n" \
        "-----END CERTIFICATE-----";
};



#endif