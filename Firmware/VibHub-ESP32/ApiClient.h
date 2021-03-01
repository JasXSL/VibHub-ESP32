/*
	VibHub Client
*/
#ifndef ApiClient_h
#define ApiClient_h

#include <Arduino.h>
#include <SocketIoClient.h>
#include <functional>
#include "Motor.h"

using namespace std::placeholders;


class ApiClient {
    public:
        ApiClient(void);
        
        void setup();
        
        void connect();
        void disconnect();
        
        bool connected() {return _connected;}
        
        
        void loop();
        
        void setFlatPWM( uint8_t motor, uint8_t value );
        void event_vib(const char * payload, size_t length);
        bool motorRunning();    // Returns true if at least one motor is above 0

    private:
        SocketIoClient _socket;
        
        void event_connect(const char * payload, size_t length);
        void event_disconnect(const char * payload, size_t length);
        void event_p(const char * payload, size_t length);
        void event_ps(const char * payload, size_t length);
        void event_ota(const char * payload, size_t length);
        
        void output_enable();
        void output_disable();
        
        bool _connected;
        bool _running;
        std::vector<Motor> motors;		// Vector containing motors to be PWMed
        
};

extern ApiClient apiClient;

#endif //ApiClient_h
