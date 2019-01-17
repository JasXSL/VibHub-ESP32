#include "UserSettings.h"
#include <Arduino.h>
#include <FS.h>
#include <SPIFFS.h>
#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson
#include "Configuration.h"

UserSettings::UserSettings(void) :
    //server(Configuration::DEFAULT_HOST),
    port(Configuration::DEFAULT_PORT),
    enable_bluetooth(false)
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

                Serial.println("opened config file");
                size_t size = configFile.size();
                // Allocate a buffer to store contents of the file.
                std::unique_ptr<char[]> buf(new char[size]);

                configFile.readBytes(buf.get(), size);
                DynamicJsonBuffer jsonBuffer;
                JsonObject& json = jsonBuffer.parseObject(buf.get());
                json.printTo(Serial);

                if( json.success() ){

                    Serial.println("\nparsed json");

                    strcpy(server, json["server"]);
                    char p[5];
                    strcpy(p, json["port"]);
                    port = atoi(p);
                    strcpy(deviceid, json["deviceid"]);
                    enable_bluetooth = json["enable_bluetooth"];
                    
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
	Serial.printf("Bluetooth: %i\n", enable_bluetooth);

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
	DynamicJsonBuffer jsonBuffer;
	JsonObject& json = jsonBuffer.createObject();
	json["server"] = server;
	json["port"] = port;
	json["deviceid"] = deviceid;
	json["enable_bluetooth"] = enable_bluetooth;

	File configFile = SPIFFS.open(Configuration::SETTINGS_FILE, "w");
	if( !configFile )
		Serial.println("failed to open config file for writing");
	
	json.printTo(Serial);
	json.printTo(configFile);
	configFile.close();
    
}


void UserSettings::reset(){

    Serial.println("UserSettings::reset");

    if( SPIFFS.begin(true) ){

        if( SPIFFS.exists(Configuration::SETTINGS_FILE) )
            SPIFFS.remove(Configuration::SETTINGS_FILE);
        
    }

}




UserSettings userSettings = UserSettings();
