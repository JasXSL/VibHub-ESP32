/*

    This script handles the captive portal end of the device

*/
#include "VhWifi.h"
#include "StatusLED.h"
#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson
#include <qrcode.h>
#include "Configuration.h"
#include "UserSettings.h"
#include <stdint.h> 

void VhWifi::connect( bool force, bool reset ){

    // Start the wifimanager
    WiFiManager wifiManager;
    _wifiManager = &wifiManager;
    
    if( reset )
        clearSettings();   
    
    // Custom CSS shared among the whole site
    String head = FPSTR(CSS_SHARED);
    head += "<script>window.onload = () => {";
        head += getCustomJSPre();
        head += FPSTR(JS_SHARED);
        head += getCustomJSPost();
    head += "};</script>";
    wifiManager.setCustomHeadElement(head.c_str());
    
    // The extra parameters to be configured
    //WiFiManagerParameter devId("deviceid", "Device ID", userSettings.deviceid, 64);
    WiFiManagerParameter serverHost("server", "Server Host", userSettings.server, 64);
    char port[6];
    itoa(userSettings.port, port, 10);
    WiFiManagerParameter serverPort("port", "Server Port", port, 6);

    char enableBluetoothVal[2];
    itoa(userSettings.enable_bluetooth, enableBluetoothVal, 10);
    WiFiManagerParameter enableBluetooth("enable_bluetooth", "Bluetooth", enableBluetoothVal, 6);

    char sleepTimerVal[2];
    itoa(userSettings.sleep_after_min, sleepTimerVal, 10);
    WiFiManagerParameter sleepTimer("sleep_after_min", "Turn off after minutes of inactivity", sleepTimerVal, 6);
    
    //wifiManager.addParameter(&devId);
    wifiManager.addParameter(&serverHost);
    wifiManager.addParameter(&serverPort);
    wifiManager.addParameter(&sleepTimer);
    wifiManager.addParameter(&enableBluetooth);

    //set config save notify callback
    wifiManager.setSaveParamsCallback(std::bind(&VhWifi::saveConfigCallback, this));
    
    //set callback that gets called when connecting to previous WiFi fails, and enters Access Point mode
    wifiManager.setAPCallback(std::bind(&VhWifi::configModeCallback, this, _1));
    
    //set minimu quality of signal so it ignores AP's under that quality
    //defaults to 8%
    wifiManager.setMinimumSignalQuality();

    //sets timeout until configuration portal gets turned off
    //useful to make it all retry or go to sleep
    //in seconds
    //wifiManager.setConfigPortalTimeout(120);
    
    String ssid = Configuration::WIFI_SSID;
    if( force ){

        if( !wifiManager.startConfigPortal(ssid.c_str()) ){

            Serial.println("VhWifi: Failed to connect and hit timeout");
            handleFatalError();

        }

    }

    // Try to connect to AP, if that doesn't work, enter config mode
    else{
        
        WiFi.begin();
        int i = 0;
        while (WiFi.status() != WL_CONNECTED && i < 30) { // Wait 3 sec for the Wi-Fi to connect
            ++i;
            delay(100);
        }

        connected = WiFi.status() == WL_CONNECTED;

        //if( !wifiManager.autoConnect(ssid.c_str()) ){
        if( !userSettings.enable_bluetooth ){
            if( !wifiManager.startConfigPortal(ssid.c_str()) ){
                // Config mode failed to enter
                Serial.println("VhWifi: Failed to connect and hit timeout");
                handleFatalError();
            }
        }
        else{
            Serial.println("VhWifi: Failed to connect, but bluetooth is enabled.");
            return;
        }

    }

    Serial.print("VhWifi: local ip: ");
    Serial.println(WiFi.localIP());
    
    Serial.println("VhWifi: connected");

    if( force ){
        ESP.restart();
        delay(1000);
    }


}

// Unrecoverable connection error
void VhWifi::handleFatalError(){

    statusLED.setState( StatusLED::STATE_WIFI_ERR );
    delay(5000);
    esp_deep_sleep_start();

}

void VhWifi::clearSettings(){
    Serial.println("VhWifi::clearSettings(");
    if (_wifiManager){
        _wifiManager->resetSettings();
    }
}

String VhWifi::getParam(String name){
    //read parameter from server, for customhmtl input
    String value;
    if(_wifiManager->server->hasArg(name)){
        value = _wifiManager->server->arg(name);
    }
    return value;
}

//callback notifying us of the need to save config
void VhWifi::saveConfigCallback(){

    Serial.println("VhWifi: Configuration change detected, saving and rebootski");
    uint8_t was_enabled = userSettings.enable_bluetooth;

    // Read updated parameters
    strcpy(userSettings.server, getParam("server").c_str());
    char p[5];
    strcpy(p, getParam("port").c_str());
    userSettings.port = atoi(p);
    userSettings.enable_bluetooth = atoi(getParam("enable_bluetooth").c_str());
    userSettings.sleep_after_min = atoi(getParam("sleep_after_min").c_str());
    userSettings.initialized = true;
    userSettings.save();

    // Force a reboot if bluetooth was just turned on
    if( userSettings.enable_bluetooth && !was_enabled ){
        ESP.restart();
        delay(1000);
    }

}

//gets called when WiFiManager enters configuration mode
void VhWifi::configModeCallback( WiFiManager *myWiFiManager ){

    // Set portal LED state
    statusLED.setState(StatusLED::STATE_PORTAL);
    
    Serial.println("VhWifi: Entered config mode");
    Serial.println(WiFi.softAPIP());
    //if you used auto generated SSID, print it
    Serial.println(myWiFiManager->getConfigPortalSSID());

}


String VhWifi::getCustomJSPre(){

    String out = "const QR_SIZE = 25;";

    out+= "const QR_DATA ='";
    QRCode qrcode;
    uint8_t qrcodeData[qrcode_getBufferSize(2)];
    String text = "vibhub:";
    text+= userSettings.deviceid;
    qrcode_initText(&qrcode, qrcodeData, 2, ECC_MEDIUM, text.c_str());

    // For Kadah: Changed from uint8 to uint8_t. Wouldn't compile otherwise. Is this an esp8266 thing?
    for( uint8_t y = 0; y < qrcode.size; y++ ){
		for( uint8_t x = 0; x < qrcode.size; x++ )
            out+= qrcode_getModule(&qrcode, x, y) ? "1" : "0";
	}
    out += "';";
    return out;
    
}

String VhWifi::getCustomJSPost(){

    String out = "";
    // Anything with class VH_VERSION gets innerText set to the version
    out+= "document.querySelectorAll('.VH_VERSION').forEach(el => {";
        out+="el.innerText='";
        out+= Configuration::VH_VERSION;
        out+= "';";
    out+= "});";

    // Update with the DEVICE ID
    out+= "document.querySelectorAll('.VH_DEV_ID').forEach(el => {";
        out+="el.innerText='";
        out+= userSettings.deviceid;
        out+= "';";
    out+= "});";
    return out;
		
}



VhWifi vhWifi = VhWifi();
