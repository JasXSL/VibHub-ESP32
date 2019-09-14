#include "UserSettings.h"
#include <Arduino.h>
#include <FS.h>
#include <SPIFFS.h>
#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson
#include "Configuration.h"
#include "ApiClient.h"
#include "FirmwareUpdate.h"

UserSettings::UserSettings(void) :
    //server(Configuration::DEFAULT_HOST),
    port(Configuration::DEFAULT_PORT),
    sleep_after_min(60),
    last_action(0),
    initialized(false)
{
    strcpy(server, Configuration::DEFAULT_HOST);
}


void UserSettings::load( bool reset ){

    Serial.println("UserSettings::load");

    if(!SPIFFS.begin(true)){

        Serial.println("SPIFFS Mount Failed. Device may be damaged or incorrectly compiled without SPIFFS");
        return;

    }
    
    Serial.println("mounted file system");
    
    // Check if config exists
    if( SPIFFS.exists(Configuration::SETTINGS_FILE) ){

        // Reset button held
        if( reset )
            SPIFFS.remove(Configuration::SETTINGS_FILE);
        
        // No reset, load the file
        else{

            //file exists, reading and loading
            Serial.println("reading config file");
            File configFile = SPIFFS.open(Configuration::SETTINGS_FILE, "r");
            if( configFile ){

                Serial.println("opened config file:");
                String content = "";
                while(configFile.available())
                    content += char(configFile.read());

                Serial.println(content);
                DynamicJsonDocument jsonBuffer(4096);
                DeserializationError error = deserializeJson(jsonBuffer, content.c_str());
                

                if( !error ){
                    Serial.println("Parsed json:");
                    serializeJson(jsonBuffer, Serial);
                    Serial.println("");

                    strcpy(server, jsonBuffer["server"]);

                    port = jsonBuffer["port"];
                    strcpy(deviceid, jsonBuffer["deviceid"]);


                    initialized = jsonBuffer["initialized"];
                    sleep_after_min = jsonBuffer["sleep_after_min"];

                }
                else
                    Serial.println("failed to load json config");

            }
            
        }

    }
    
    // Output the device info into serial
    Serial.printf("DeviceID: %s\n", deviceid);
    Serial.printf("Server: %s\n", server);
	Serial.printf("Port: %i\n", port);
	Serial.printf("Sleep minutes: %i\n", sleep_after_min);

	if( deviceid[0] == '\0' || port == 0 || port > 65535 || server[0] == '\0' ){

		if( deviceid[0] == '\0' ){

			Serial.println("No device ID found, randomizing one");
			gen_random(deviceid, 12);

		}

		if( port == 0 || port > 65535 ){
			
			Serial.println("Invalid port, resetting to default");
			port = Configuration::DEFAULT_PORT;

		}

		if( server[0] == '\0' ){

			Serial.println("Invalid server, resetting to factory default");
			strcpy(server, Configuration::DEFAULT_HOST);

		}

		save();

	}
    
}

// Random device ID generator
void UserSettings::gen_random( char *s, const int len ){
    
	static const char alphanum[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

    for( int i = 0; i < len; ++i )
        s[i] = alphanum[esp_random() % (sizeof(alphanum) - 1)];
    
    s[len] = 0;

}

// Saves to the config file
void UserSettings::save(){

    Serial.println("UserSettings::save");
	DynamicJsonDocument jsonBuffer(1024);
	JsonObject json = jsonBuffer.to<JsonObject>();
	json["server"] = server;
	json["port"] = port;
	json["deviceid"] = deviceid;
	json["sleep_after_min"] = sleep_after_min;
    json["initialized"] = initialized;

	File configFile = SPIFFS.open(Configuration::SETTINGS_FILE, "w");
	if( !configFile )
		Serial.println("failed to open config file for writing");
	
    Serial.println("Saving config: ");
	serializeJson(json, Serial);
	serializeJson(json, configFile);
	configFile.close();
    
}


void UserSettings::reset(){

    Serial.println("UserSettings::reset");

    if( SPIFFS.begin(true) ){

        if( SPIFFS.exists(Configuration::SETTINGS_FILE) )
            SPIFFS.remove(Configuration::SETTINGS_FILE);
        
    }

}


void UserSettings::resetSleepTimer(){
    last_action = millis();
}

void UserSettings::loop(){

    if( sleep_after_min <= 0 )
        return;

    // Reset timer if the motor is running or we're updating firmware
    if( apiClient.motorRunning() || fwUpdate.running ){
        resetSleepTimer();
    }
    else if( millis() > sleep_after_min*60000+last_action ){
        Serial.println("Sleep timer hit. Shutting down.");
        esp_deep_sleep_start();
    }
}



UserSettings userSettings = UserSettings();
