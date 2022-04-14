/*
External libraries needed if not using platformio:
https://github.com/JasXSL/WiFiManager - It has a custom ajax endpoint
https://github.com/Kadah/arduinoWebSockets - Kadah's fork of arduinoWebSockets
https://github.com/Kadah/socket.io-client - Ditto for socket.io-client
https://arduinojson.org/ - ArduinoJson version 6.13.0, via Arduino IDE's Manage Libraries
*/ 

#include <Arduino.h>

#include "VhWifi.h"

#include <WiFi.h>
#include <HTTPClient.h> 

#include "Configuration.h"
#include "UserSettings.h"
#include "ConfigButton.h"
#include "ApiClient.h"
#include "StatusLED.h"
#include "Pwm.h"


// Program begins
void setup() {

	// Serial setup
    Serial.begin(115200);
    //Serial.setDebugOutput(true);
    delay(1000);
    Serial.println("\nStarting...");

    Serial.printf("Board Version: %s\n", Configuration::VH_HWVERSION);
    Serial.printf("Firmware Version: %s\n", Configuration::VH_VERSION);

    // Set LED state
    statusLED.initialize();
    statusLED.setState(StatusLED::STATE_BOOT);
    
    
    configButton.setup();
    apiClient.setup();
   
    // Reset config and wifi if config button is held on boot
	bool reset = false;
    if( configButton.isPressed() ){

        Serial.println("Resetting everything");        
        reset = true;

    }
    

    userSettings.load(reset);
    Serial.println("Connecting to wifi");
    Serial.printf("Reset: %i\n", reset || !userSettings.initialized);
    vhWifi.connect(reset || !userSettings.initialized, reset);
    Serial.println("Connected");
    
	
    // Set socket loading state
    statusLED.setState(StatusLED::STATE_SOCKET_ERR);

    // Set system time
    setClock();

    //Connect to server
    if( vhWifi.connected ){
        apiClient.connect();
        apiClient.loop();
        yield();
    }



    //TODO: dedicated programming mode
    /*
    Serial.println("OTA: Setting up");
    ArduinoOTA.setPort(8266);
    ArduinoOTA.setHostname("vh8266");
    ArduinoOTA.setPassword("vhota");
    
    ArduinoOTA.onStart([]() {
        String type;
        if (ArduinoOTA.getCommand() == U_FLASH)
            type = "sketch";
        else // U_SPIFFS
            type = "filesystem";

        // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
        Serial.println("Start updating " + type);
    });
    ArduinoOTA.onEnd([]() {
        Serial.println("\nEnd");
    });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("Progress: %u%%\n", (progress / (total / 100)));
    });
    ArduinoOTA.onError([](ota_error_t error) {
        Serial.printf("Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
        else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
        else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
        else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
        else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });
    ArduinoOTA.begin();
    Serial.println("OTA: Ready");
	*/
}


// Main program lööp
void loop() {

    apiClient.loop();
    configButton.loop();
    userSettings.loop();
    //ArduinoOTA.handle();

}

void setClock() {
  configTime(0, 0, "pool.ntp.org", "time.nist.gov");

  Serial.print("Waiting for NTP time sync: ");
  time_t nowSecs = time(nullptr);
  while (nowSecs < 8 * 3600 * 2) {
    delay(500);
    Serial.print(".");
    yield();
    nowSecs = time(nullptr);
  }

  Serial.println();
  struct tm timeinfo;
  gmtime_r(&nowSecs, &timeinfo);
  Serial.print("Current time: ");
  Serial.println(asctime(&timeinfo));
}
