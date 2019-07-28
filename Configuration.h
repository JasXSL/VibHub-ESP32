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

    const char VH_VERSION[]         = "0.2.1";          // Software version
    const char VH_HWVERSION[]       = "ESP32_2.0a";     // Hardware type & version
    const char WIFI_SSID[]          = "VibHub";         // Network SSID
    const char DEFAULT_HOST[]       = "192.168.0.191";      // Default socket host
    const int  DEFAULT_PORT         = 80;               // Default socket port
    const char API_URL[]            = "/socket.io/?transport=websocket";
    const char SETTINGS_FILE[]      = "/config.json";   // Default SPIFFS config file

    // Websockets
    const int WEBSOCKET_RECONNECT_TIME = 3000;          // How long to wait after a connection failed

    // Status LED
    const uint8_t SLED_BRIGHTNESS   = 50;   // Status LED brightness, 0-255

    // ESP32 2.0a Board GPIO Assignments
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
    

    // PWM
    const int PWM_FREQ              = 16000; // PWM Frequency
    const int PWM_RESOLUTION        = 8;     // PWM resolution

    // Button config
    const bool BUTTON_UP            = HIGH;  // Value when the button is not pressed
    const bool BUTTON_DOWN          = LOW;   // Value when the button IS pressed
    const int BUTTON_HOLD_TIME      = 4000;  // Time before it enters config mode
    const int BUTTON_BT_MIN_TIME    = 2000;  // Enter bluetooth config mode if between this and max time
    const int BUTTON_BT_MAX_TIME    = 4000;
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
"MIIEkjCCA3qgAwIBAgIQCgFBQgAAAVOFc2oLheynCDANBgkqhkiG9w0BAQsFADA/\n" \
"MSQwIgYDVQQKExtEaWdpdGFsIFNpZ25hdHVyZSBUcnVzdCBDby4xFzAVBgNVBAMT\n" \
"DkRTVCBSb290IENBIFgzMB4XDTE2MDMxNzE2NDA0NloXDTIxMDMxNzE2NDA0Nlow\n" \
"SjELMAkGA1UEBhMCVVMxFjAUBgNVBAoTDUxldCdzIEVuY3J5cHQxIzAhBgNVBAMT\n" \
"GkxldCdzIEVuY3J5cHQgQXV0aG9yaXR5IFgzMIIBIjANBgkqhkiG9w0BAQEFAAOC\n" \
"AQ8AMIIBCgKCAQEAnNMM8FrlLke3cl03g7NoYzDq1zUmGSXhvb418XCSL7e4S0EF\n" \
"q6meNQhY7LEqxGiHC6PjdeTm86dicbp5gWAf15Gan/PQeGdxyGkOlZHP/uaZ6WA8\n" \
"SMx+yk13EiSdRxta67nsHjcAHJyse6cF6s5K671B5TaYucv9bTyWaN8jKkKQDIZ0\n" \
"Z8h/pZq4UmEUEz9l6YKHy9v6Dlb2honzhT+Xhq+w3Brvaw2VFn3EK6BlspkENnWA\n" \
"a6xK8xuQSXgvopZPKiAlKQTGdMDQMc2PMTiVFrqoM7hD8bEfwzB/onkxEz0tNvjj\n" \
"/PIzark5McWvxI0NHWQWM6r6hCm21AvA2H3DkwIDAQABo4IBfTCCAXkwEgYDVR0T\n" \
"AQH/BAgwBgEB/wIBADAOBgNVHQ8BAf8EBAMCAYYwfwYIKwYBBQUHAQEEczBxMDIG\n" \
"CCsGAQUFBzABhiZodHRwOi8vaXNyZy50cnVzdGlkLm9jc3AuaWRlbnRydXN0LmNv\n" \
"bTA7BggrBgEFBQcwAoYvaHR0cDovL2FwcHMuaWRlbnRydXN0LmNvbS9yb290cy9k\n" \
"c3Ryb290Y2F4My5wN2MwHwYDVR0jBBgwFoAUxKexpHsscfrb4UuQdf/EFWCFiRAw\n" \
"VAYDVR0gBE0wSzAIBgZngQwBAgEwPwYLKwYBBAGC3xMBAQEwMDAuBggrBgEFBQcC\n" \
"ARYiaHR0cDovL2Nwcy5yb290LXgxLmxldHNlbmNyeXB0Lm9yZzA8BgNVHR8ENTAz\n" \
"MDGgL6AthitodHRwOi8vY3JsLmlkZW50cnVzdC5jb20vRFNUUk9PVENBWDNDUkwu\n" \
"Y3JsMB0GA1UdDgQWBBSoSmpjBH3duubRObemRWXv86jsoTANBgkqhkiG9w0BAQsF\n" \
"AAOCAQEA3TPXEfNjWDjdGBX7CVW+dla5cEilaUcne8IkCJLxWh9KEik3JHRRHGJo\n" \
"uM2VcGfl96S8TihRzZvoroed6ti6WqEBmtzw3Wodatg+VyOeph4EYpr/1wXKtx8/\n" \
"wApIvJSwtmVi4MFU5aMqrSDE6ea73Mj2tcMyo5jMd6jmeWUHK8so/joWUoHOUgwu\n" \
"X4Po1QYz+3dszkDqMp4fklxBwXRsW10KXzPMTZ+sOPAveyxindmjkW8lGy+QsRlG\n" \
"PfZ+G6Z6h7mjem0Y+iWlkYcV4PIWL1iwBi8saCbGS5jN2p8M+X+Q7UNKEkROb3N6\n" \
"KOqkqm57TH2H3eDJAkSnh6/DNFu0Qg==\n" \
"-----END CERTIFICATE-----\n";
};



#endif