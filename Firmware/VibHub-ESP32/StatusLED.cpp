#include "StatusLED.h"
#include "Configuration.h"
#include <Arduino.h>
#include <ESP32Ticker.h>
#include "VhWifi.h"

Ticker ledTicker;               // Ticker for LED

const uint8_t StatusLED::STATE_BOOT = 0;
const uint8_t StatusLED::STATE_INIT = 1;
const uint8_t StatusLED::STATE_PORTAL = 2;
const uint8_t StatusLED::STATE_WIFI_ERR = 3;
const uint8_t StatusLED::STATE_SOCKET_ERR = 4;
const uint8_t StatusLED::STATE_RUNNING = 5;


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
    ledTickerHigh(false)
    //p_red(),
    //p_green(),
    //p_blue()
{

}


void StatusLED::initialize(){

    p_red.initialize(Configuration::PIN_SLED_R);
    p_green.initialize(Configuration::PIN_SLED_G);
    p_blue.initialize(Configuration::PIN_SLED_B);

    p2_red.initialize(Configuration::PIN_SLED2_R);
    p2_green.initialize(Configuration::PIN_SLED2_G);
    p2_blue.initialize(Configuration::PIN_SLED2_B);

    Serial.println("Initialized StatusLED");
    setLed(OFF);

}

void StatusLED::setLed( int color ){

    uint32_t r = 255 - (color&RED ? Configuration::SLED_BRIGHTNESS : 0);
    uint32_t g = 255 - (color&GREEN ? Configuration::SLED_BRIGHTNESS : 0);
    uint32_t b = 255 - (color&BLUE ? Configuration::SLED_BRIGHTNESS : 0);

    // Serial.printf("C %i : R %i G %i B %i\n", color, r, g, b);

    p_red.setPWM(r);
    p_green.setPWM(g);
    p_blue.setPWM(b);

    p2_red.setPWM(r);
    p2_green.setPWM(g);
    p2_blue.setPWM(b);

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
            setLed(CYAN);
            Serial.println(" - STATE_INIT cyan too :D");
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
            setLed(vhWifi.connected ? GREEN : BLUE);
            Serial.printf(" - STATE_RUNNING %s\n", (vhWifi.connected ? "GREEN" : "BLUE(tooth)"));
            break;
        default : // STATE_BOOT
            setLed(CYAN);
            Serial.println(" - STATE_BOOT half blue");

    }

}


void StatusLED::testTick( int color ){
    
    statusLED.setLed(OFF);
    
    Serial.printf("Stage: %i\n", statusLED.testColor);

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
    
    ++statusLED.testColor;
    if( statusLED.testColor >= 9 )
        ledTicker.detach();
    
}

void StatusLED::test(){

    testColor = 0;
    ledTicker.attach(2, StatusLED::testTick, 0);

}


StatusLED statusLED = StatusLED();
