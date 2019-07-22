// Libraries
    #include <Arduino.h>
    
	#include "VhWifi.h"

    #include <WiFi.h>
    #include <HTTPClient.h>

	#include "Configuration.h"
	#include "UserSettings.h"
	// #include "ConfigButton.h"
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
    
    
    // configButton.setup();
    apiClient.setup();
   
    // Reset config and wifi if config button is held on boot
	// bool reset = false;
    // if( configButton.isPressed() ){

        // Serial.println("Resetting everything");
        // VHBluetooth* pMainBleServer = new VHBluetooth();
        // pMainBleServer->setStackSize(20000);
        // pMainBleServer->reset = true;
        // pMainBleServer->start();
        
        // reset = true;

    // }
    

    // userSettings.load(reset);
    Serial.println("Connecting to wifi");
    vhWifi.connect();
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
}


// Main program lööp
void loop() {

    vhWifi.loop();

    if( vhWifi.connected ){
        apiClient.loop();
    }

    // configButton.loop();
    userSettings.loop();
}

