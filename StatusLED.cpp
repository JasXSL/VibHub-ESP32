#include "StatusLED.h"
#include "Configuration.h"
#include <Arduino.h>
#include <ESP32Ticker.h>

Ticker ledTicker;               // Ticker for LED

const char StatusLED::STATE_BOOT = 0;
const char StatusLED::STATE_INIT = 1;
const char StatusLED::STATE_PORTAL = 2;
const char StatusLED::STATE_WIFI_ERR = 3;
const char StatusLED::STATE_SOCKET_ERR = 4;
const char StatusLED::STATE_RUNNING = 5;

#define RED     0x1
#define GREEN   0x2
#define BLUE    0x4

#define YELLOW  0x3
#define PURPLE  0x5
#define CYAN    0x6

#define OFF     0x0
#define WHITE   0x7


StatusLED::StatusLED() :
    programState(-1),
    ledTickerHigh(false),
    p_red(Configuration::PIN_SLED_R),
    p_green(Configuration::PIN_SLED_G),
    p_blue(Configuration::PIN_SLED_B)
{
    setLed(OFF);
}


void StatusLED::setLed( int color ){

    
    int red = color&RED ? Configuration::SLED_BRIGHTNESS : 0;
    int green = color&GREEN ? Configuration::SLED_BRIGHTNESS : 0;
    int blue = color&BLUE ? Configuration::SLED_BRIGHTNESS : 0;
    Serial.printf("R %i G %i B %i\n", red, green, blue);
    p_red.setPWM(255-red);
    p_green.setPWM(255-green);
    p_blue.setPWM(255-blue);

}



// Flash led on and off
void StatusLED::flashingTick(int color){

    statusLED.ledTickerHigh = !statusLED.ledTickerHigh;
    statusLED.setLed(color*statusLED.ledTickerHigh);
    statusLED.testColor = 0;

}

void StatusLED::setState( int state ){

    if( programState == state )
        return;

    programState = state;
    
    Serial.printf("setState: %i\n", state);
    
    ledTicker.detach();


    switch(state){
        // std::bind(&ApiClient::event_connect, this, _1, _2)
        case STATE_INIT :
            setLed(BLUE);
            Serial.println(" - STATE_INIT blue");
            break;
        case STATE_PORTAL :
            setLed(BLUE);
            ledTickerHigh = true;
            ledTicker.attach_ms(25, StatusLED::flashingTick, BLUE);
            Serial.println(" - STATE_PORTAL blue blink");
            break;
        case STATE_WIFI_ERR :
            setLed(RED);
            ledTickerHigh = true;
            ledTicker.attach_ms(500, StatusLED::flashingTick, RED);
            Serial.println(" - STATE_WIFI_ERR red blink");
            break;
        case STATE_SOCKET_ERR :
            setLed(YELLOW);
            Serial.println(" - STATE_SOCKET_ERR half green");
            break;
        case STATE_RUNNING :
            setLed(GREEN);
            Serial.println(" - STATE_RUNNING green");
            break;
        default : // STATE_BOOT
            setLed(CYAN);
            Serial.println(" - STATE_BOOT half blue");

    }

}


void StatusLED::testTick( int color ){
    
    statusLED.setLed(OFF);
    
    switch(statusLED.testColor) {

        case 0 : statusLED.setLed(RED); break;
        case 1 : statusLED.setLed(GREEN); break;
        case 2 : statusLED.setLed(BLUE); break;
        case 3 : statusLED.setLed(YELLOW); break;
        case 4 : statusLED.setLed(PURPLE); break;
        case 5 : statusLED.setLed(CYAN); break;
        case 6 : statusLED.setLed(OFF); break;
        case 7 : statusLED.setLed(WHITE); break;
        default : statusLED.setLed(GREEN);

    }
    
    statusLED.testColor++;
    if( statusLED.testColor >= 9 )
        ledTicker.detach();
    
}

void StatusLED::test(){

    testColor = 0;
    ledTicker.attach(2, StatusLED::testTick, 0);

}


StatusLED statusLED = StatusLED();
