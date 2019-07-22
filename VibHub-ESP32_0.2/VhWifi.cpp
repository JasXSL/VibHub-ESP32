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
#include <IotWebConf.h>
#include <stdint.h>


// -- Callback method declarations.
void handleRoot();
void wifiConnected();
void configSaved();
boolean formValidator();

DNSServer dnsServer;
WebServer server(80);
HTTPUpdateServer httpUpdater;


IotWebConf iotWebConf(Configuration::WIFI_SSID, &dnsServer, &server, Configuration::WIFI_PASS, Configuration::CONFIG_VERSION);

// The extra parameters to be configured
char serverhost[64];
char port[6];
char sleepTimerVal[6];
char enableBluetoothVal[2];

IotWebConfSeparator serverSep = IotWebConfSeparator("Server Settings");
IotWebConfParameter serverParam("Server Host", "srvParam", serverhost, 64, "text", "Default: vibhub.io", "vibhub.io");
IotWebConfParameter portParam = IotWebConfParameter("Server Port", "portParam", port, 6, "number", "Default: 80", "80", "min='1' max='65535' step='1'");

IotWebConfSeparator deviceSep = IotWebConfSeparator("Device Settings");
IotWebConfParameter sleepParam = IotWebConfParameter("Turn off after minutes of inactivity", "sleepParam", sleepTimerVal, 6, "number", "Default: 60", "80", "min='0' max='65535' step='1'");
IotWebConfParameter btParam = IotWebConfParameter("Bluetooth", "btParam", enableBluetoothVal, 2, "checkbox", NULL, "0");

boolean needReset = false;


void VhWifi::connect(){
    
    // Setup iotWebConf
    iotWebConf.setupUpdateServer(&httpUpdater);

    // -- Status indicator pin.
    //      First it will light up (kept LOW), on Wifi connection it will blink,
    //      when connected to the Wifi it will turn off (kept HIGH).
    iotWebConf.setStatusPin(Configuration::PIN_SLED_B);

    // -- When CONFIG_PIN is pulled to ground on startup, the Thing will use the initial
    //      password to buld an AP. (E.g. in case of lost password)
    iotWebConf.setConfigPin(Configuration::PIN_CONFIG_BUTTON);
    

    // Config Params
    strcpy(serverhost, userSettings.server);
    itoa(userSettings.port, port, 10);
    itoa(userSettings.sleep_after_min, sleepTimerVal, 10);
    itoa(userSettings.enable_bluetooth, enableBluetoothVal, 10);

    iotWebConf.addParameter(&serverSep);
    iotWebConf.addParameter(&serverParam);
    iotWebConf.addParameter(&portParam);
    
    iotWebConf.addParameter(&deviceSep);
    iotWebConf.addParameter(&sleepParam);
    iotWebConf.addParameter(&btParam);
    iotWebConf.getApTimeoutParameter()->visible = true;
    
    // Callbacks
    iotWebConf.setConfigSavedCallback(&configSaved);
    iotWebConf.setFormValidator(&formValidator);
    iotWebConf.setWifiConnectionCallback(&wifiConnected);

    // -- Initializing the configuration.
    iotWebConf.init();

    // -- Set up required URL handlers on the web server.
    server.on("/", handleRoot);
    server.on("/config", []{ iotWebConf.handleConfig(); });
    server.onNotFound([](){ iotWebConf.handleNotFound(); });
    
    Serial.println("VhWifi: Ready.");
}


void VhWifi::loop(){
    iotWebConf.doLoop();
    
    if (needReset)
    {
        Serial.println("VhWifi: Rebooting after 1 second.");
        iotWebConf.delay(1000);
        ESP.restart();
    }
}


VhWifi vhWifi = VhWifi();


/**
 * Handle web requests to "/" path.
 */
void handleRoot()
{
  // -- Let IotWebConf test and handle captive portal requests.
  if (iotWebConf.handleCaptivePortal())
  {
    // -- Captive portal request were already served.
    return;
  }
  String s = "<!DOCTYPE html><html lang=\"en\"><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1, user-scalable=no\"/>";
  s += "<title>VibHub 0.2 Config</title></head><body>VibHub 0.2 Config";
  s += "<ul>";
  s += "<li>Server Host: ";
  s += userSettings.server;
  s += "<li>Server Port: ";
  s += userSettings.port;
  s += "<li>Turn off after minutes of inactivity: ";
  s += userSettings.sleep_after_min;
  s += "<li>Bluetooth enabled: ";
  s += userSettings.enable_bluetooth;
  s += "</ul>";
  s += "Go to <a href='config'>configure page</a> to change values.";
  s += "</body></html>\n";

  server.send(200, "text/html", s);
}

void wifiConnected()
{
    Serial.println("VhWifi: Wifi Connected.");
    vhWifi.connected = true;
}

void configSaved()
{
    Serial.println("VhWifi: Configuration was updated.");

    strcpy(userSettings.server, serverhost);
    userSettings.port = atoi(port);
    userSettings.sleep_after_min = atoi(sleepTimerVal);
    userSettings.enable_bluetooth = atoi(enableBluetoothVal);
    userSettings.save();
  
    needReset = true;
}

boolean formValidator()
{
    Serial.println("VhWifi: Validating form.");
    boolean valid = true;

    int l = server.arg(serverParam.getId()).length();
    if (l < 3)
    {
        serverParam.errorMessage = "Please provide at least 3 characters!";
        valid = false;
    }

    return valid;
}

