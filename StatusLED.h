/*
    VibHub Led
*/
#ifndef StatusLED_h
#define StatusLED_h

#include <Arduino.h>
#include "Pwm.h"
#include <functional>
using namespace std::placeholders;
class StatusLED{

    public:
        StatusLED(void);

        static const char STATE_BOOT;
        static const char STATE_INIT;
        static const char STATE_PORTAL;
        static const char STATE_WIFI_ERR;
        static const char STATE_SOCKET_ERR;
        static const char STATE_RUNNING;
        
        bool ledTickerHigh;
        void setState( int state = STATE_BOOT );
        void setLed( int color );
        void test();
        static void testTick( int color );
        static void flashingTick( int color );
        
    private:
        int programState;           // State the program is currently in
        Pwm p_red;
        Pwm p_green;
        Pwm p_blue;
        uint8_t testColor;

};


extern StatusLED statusLED;

#endif //StatusLED_h