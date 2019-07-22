/*
	
    Handles socket events from the vibhub server
    
*/

#include "ApiClient.h"
#include "Configuration.h"
#include "StatusLED.h"
#include "UserSettings.h"
#include "FirmwareUpdate.h"
#include <ArduinoJson.h>
#include "VhWifi.h"

ApiClient::ApiClient(void) :
    _socket(),
    _connected(false),
    _running(false)
{
    
}


void ApiClient::setup(){

	motors.push_back(Motor(Configuration::PIN_MOTOR_A_IN1, Configuration::PIN_MOTOR_A_IN2));
	motors.push_back(Motor(Configuration::PIN_MOTOR_B_IN1, Configuration::PIN_MOTOR_B_IN2));
	motors.push_back(Motor(Configuration::PIN_MOTOR_C_IN1, Configuration::PIN_MOTOR_C_IN2));
	motors.push_back(Motor(Configuration::PIN_MOTOR_D_IN1, Configuration::PIN_MOTOR_D_IN2));

    // Attach event handlers
    // For simplicity, events are always attached regardless
    _socket.on("connect", std::bind(&ApiClient::event_connect, this, _1, _2));
    _socket.on("disconnect", std::bind(&ApiClient::event_disconnect, this, _1, _2));
    _socket.on("vib", std::bind(&ApiClient::event_vib, this, _1, _2));
    _socket.on("p", std::bind(&ApiClient::event_p, this, _1, _2));
    _socket.on("ota", std::bind(&ApiClient::event_ota, this, _1, _2));

    pinMode(Configuration::PIN_NSLEEP, OUTPUT);

}

bool ApiClient::motorRunning(){
    uint8_t i;
    for( i = 0; i < 4; ++i ){
        if( motors[i].running() )
            return true;
    }
    return false;
}


void ApiClient::connect(){

    Serial.printf("ApiClient::connect -> %s:%i\n", userSettings.server, userSettings.port);
    _running = true; // Start loop
    _socket.begin(userSettings.server, userSettings.port);

}

void ApiClient::disconnect(){

    Serial.println("ApiClient::disconnect");
    _connected = false; // we don't need the disconnect event
    _running = false; // Don't run loop (will cause reconnect)
    _socket.disconnect();

}

void ApiClient::event_connect( const char * payload, size_t length ){

    Serial.println("ApiClient::event_connect");
    _connected = true;
    _socket.emit("id", ("\"" + (String)userSettings.deviceid + "\"").c_str());
    statusLED.setState(StatusLED::STATE_RUNNING);
    output_enable();

    // KC: Force test OTA
    // fwUpdate.start("0.0.1/Board_Test_32.bin", "ba23a5ca48356df4aac57df6a2634dbe");
}

void ApiClient::event_disconnect( const char * payload, size_t length ){

    if( _connected ){ // Fire only once

        Serial.println("ApiClient::event_disconnect");
        statusLED.setState(StatusLED::STATE_SOCKET_ERR);
        _connected = false;
        output_disable();

    }

}


void ApiClient::event_vib( const char * payload, size_t length ){

    Serial.printf("ApiClient::event_vib: %s\n", payload);

    userSettings.resetSleepTimer();

    DynamicJsonBuffer jsonBuffer;
    JsonVariant variant = jsonBuffer.parse(payload);

    //variant.printTo(Serial);

    if( variant.success() ){

        // Convert to JSON array
        int i;
        DynamicJsonBuffer buf;
        JsonArray& js = buf.createArray();
        if( !variant.is<JsonArray>() ){

            JsonObject& obj = variant;
            js.add(obj);

        }
        else{
            
            JsonArray &arr = variant;
            for( i=0; i<arr.size(); ++i )
                js.add(arr[i]);

        }

        // Cycle through all programs
        for( i=0; i<js.size(); ++i ){

            JsonObject& j = js[i];

            bool mo[4] = {true, true, true, true};
            
            if( j.containsKey("port") ){

                int port = atoi(j["port"]);
                if( port > 0 ){

                    for( int i = 0; i<4; ++i )
                        mo[i] = port&(1<<i);

                }

            }

            int repeats = 0;
            if( j.containsKey("repeats") )
                repeats = atoi(j["repeats"]);

            int i;
            for( i=0; i<4; ++i ){

                if( mo[i] )
                    motors[i].loadProgram(j["stages"], repeats);

            }

        }
        

    }
    else
        Serial.println("failed to load json config");

}


void ApiClient::event_p( const char * payload, size_t length ){

    unsigned long int data = strtoul(payload, 0, 16);
    uint8_t vibArray[4];
    vibArray[0] = (int)((data & 0xFF000000) >> 24 );
    vibArray[1] = (int)((data & 0x00FF0000) >> 16 );
    vibArray[2] = (int)((data & 0x0000FF00) >> 8 );
    vibArray[3] = (int)((data & 0X000000FF));
    Serial.printf("ApiClient::event_p - v0: %u, v1: %u, v2: %u, v3: %u\n", vibArray[0], vibArray[1], vibArray[2], vibArray[3]);

    userSettings.resetSleepTimer();

    int i;
    for( i = 0; i < 4; ++i ){

        setFlatPWM(i, vibArray[i]);

    }

}


void ApiClient::event_ota( const char * payload, size_t length ){
    
    Serial.printf("ApiClient::event_ota - payload: %s\n", payload);
    
    userSettings.resetSleepTimer();
    DynamicJsonBuffer jsonBuffer;
    JsonObject& root = jsonBuffer.parse(payload);
    
    const char* file = root["file"];
    const char* md5 = root["md5"];
    
    fwUpdate.start(file, md5);
    
}

void ApiClient::setFlatPWM( uint8_t motor, uint8_t value = 0 ){
    motors[motor].stopProgram();  // Stop any running program when this is received
    motors[motor].setPWM(value);
}


void ApiClient::loop() {

    if (_running){

        _socket.loop();

        for( int i=0; i<motors.size(); ++i )
            motors[i].update();

    }

}

// Enables/Disables motor drivers (sets DRV8833's nSLEEP pin high/low)
void ApiClient::output_enable(){
    digitalWrite(Configuration::PIN_NSLEEP, HIGH);
}
void ApiClient::output_disable(){  
    digitalWrite(Configuration::PIN_NSLEEP, LOW);
}


ApiClient apiClient = ApiClient();
