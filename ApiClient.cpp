/*
	
    Handles socket events from the vibhub server
    
*/

#include "ApiClient.h"
#include "Configuration.h"
#include "StatusLED.h"
#include "UserSettings.h"
#include <ArduinoJson.h>

ApiClient::ApiClient(void) :
    _socket(),
    _connected(false),
    _running(false)
{
    
}


void ApiClient::setup(){

	motors.push_back(Motor(Configuration::PIN_MOTOR_A_FWD, Configuration::PIN_MOTOR_A_BACK));
	motors.push_back(Motor(Configuration::PIN_MOTOR_B_FWD, Configuration::PIN_MOTOR_B_BACK));
	motors.push_back(Motor(Configuration::PIN_MOTOR_C_FWD, Configuration::PIN_MOTOR_C_BACK));
	motors.push_back(Motor(Configuration::PIN_MOTOR_D_FWD, Configuration::PIN_MOTOR_D_BACK));
	

    // Attach event handlers
    // For simplicity, events are always attached regardless
    _socket.on("connect", std::bind(&ApiClient::event_connect, this, _1, _2));
    _socket.on("disconnect", std::bind(&ApiClient::event_disconnect, this, _1, _2));
    _socket.on("vib", std::bind(&ApiClient::event_vib, this, _1, _2));
    _socket.on("p", std::bind(&ApiClient::event_p, this, _1, _2));

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
    //pwm.enable(); TODO: Kadah, figure out the enable thing

}

void ApiClient::event_disconnect( const char * payload, size_t length ){

    if( _connected ){ // Fire only once

        Serial.println("ApiClient::event_disconnect");
        statusLED.setState(StatusLED::STATE_SOCKET_ERR);
        _connected = false;
        //pwm.disable(); TODO: Kadah, figure out the enable thing

    }

}


void ApiClient::event_vib( const char * payload, size_t length ){

    Serial.printf("ApiClient::event_vib: %s\n", payload);

    DynamicJsonBuffer jsonBuffer;
    JsonVariant variant = jsonBuffer.parse(payload);

    variant.printTo(Serial);

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

    int i;
    for( i = 0; i < 4; ++i ){

        motors[i].stopProgram();  // Stop any running program when this is received
        motors[i].setPWM(vibArray[i]);

    }

}


void ApiClient::loop() {

    if (_running){

        _socket.loop();

        for( int i=0; i<motors.size(); ++i )
            motors[i].update();

    }

}


ApiClient apiClient = ApiClient();
