// Libraries
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
    #include "VHBluetooth.h"

	/*
    #include <ArduinoOTA.h>
	*/

// Program begins
void setup() {

	// Serial setup
    Serial.begin(115200);
    //Serial.setDebugOutput(true);
    delay(1000);
    Serial.println("\nStarting...");

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
    vhWifi.connect(reset, reset);
    Serial.println("Connected");
    
	
    // Set socket loading state
    statusLED.setState(StatusLED::STATE_SOCKET_ERR);
    
    //Connect to server
    if( vhWifi.connected ){
        apiClient.connect();
        // Test if it works
        HTTPClient http;
        Serial.printf("Wifi Status: %i\n", WiFi.status() == WL_CONNECTED);
        http.begin("http://vibhub.io/api/"); //Specify the URL
        int httpCode = http.GET();                                        //Make the request
        Serial.printf("HTTP code %i\n", httpCode);
        http.end(); //Free the resources
    }

    // Init bluetooth
    if( userSettings.enable_bluetooth ){

        VHBluetooth* pMainBleServer = new VHBluetooth();
        pMainBleServer->setStackSize(20000);
        pMainBleServer->start();
        // We are in bluetooth only mode
        if( !vhWifi.connected ){
            statusLED.setState(StatusLED::STATE_RUNNING);
        }

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
    //ArduinoOTA.handle();
	//delay(100);
    
}

