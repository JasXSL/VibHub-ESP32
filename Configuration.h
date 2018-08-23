/*
	This is the project configuration, these values don't change
*/
#ifndef _VH_CONFIGURATION
#define _VH_CONFIGURATION
#include <Arduino.h>
// Bug with arduino unless I undefine these
#undef min
#undef max

namespace Configuration{

	const char VH_VERSION[6] = "0.2.0";
	const char WIFI_SSID[21] = "VibHub Configuration";
	const char DEFAULT_HOST[10] = "vibhub.io";
	const int DEFAULT_PORT = 80;
	const char SETTINGS_FILE[13] = "/config.json";

	// Websockets
	const int WEBSOCKET_RECONNECT_TIME = 10000;


	// GPIO Assignment
	const uint8_t PIN_SLED_R = 23;
	const uint8_t PIN_SLED_G = 19;
	const uint8_t PIN_SLED_B = 22;
	const uint8_t PIN_CONFIG_BUTTON = 18;

	// PWM
	const int PWM_FREQ = 16000;
	const int PWM_RESOLUTION = 8;

	// Button config
	const bool BUTTON_UP = HIGH;
    const bool BUTTON_DOWN = LOW;
	const int BUTTON_HOLD_TIME = 3000;
	const int BUTTON_DEBOUNCE = 100;

};


#endif