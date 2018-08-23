/*
	VibHub Client
*/
#ifndef ApiClient_h
#define ApiClient_h

#include <Arduino.h>
#include "SocketIoClient.h"
#include <functional>
//#include "VhMotor.h"

using namespace std::placeholders;


class ApiClient {
    public:
        ApiClient(void);
        
        void setup();
        
        void connect();
        void disconnect();
        
        bool connected() {return _connected;}
        
        void loop();
        
    private:
        SocketIoClient _socket;
        
        void event_connect(const char * payload, size_t length);
        void event_disconnect(const char * payload, size_t length);
        void event_vib(const char * payload, size_t length);
        void event_p(const char * payload, size_t length);
        
        bool _connected;
        bool _running;
        
        // Todo: Motors
        //std::vector<VhMotor> motors;		// Vector containing motors to be PWMed
};

extern ApiClient apiClient;

#endif //ApiClient_h
