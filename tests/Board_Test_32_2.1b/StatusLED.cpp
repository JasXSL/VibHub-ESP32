#include "StatusLED.h"
#include "Configuration.h"
#include <Arduino.h>
#include <ESP32Ticker.h>
// #include "VhWifi.h"

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

    // Serial.println("Initialized StatusLED");
    setLed(OFF);

}

void StatusLED::setLed( uint8_t color ){
    setLed( 3, color );
}

void StatusLED::setLed( uint8_t led, uint8_t color ){
    
    uint16_t r = 256 - (color&RED ? Configuration::SLED_BRIGHTNESS : 0);
    uint16_t g = 256 - (color&GREEN ? Configuration::SLED_BRIGHTNESS : 0);
    uint16_t b = 256 - (color&BLUE ? Configuration::SLED_BRIGHTNESS : 0);
    // Serial.printf("C %i : R %i G %i B %i\n", color, r, g, b);

    setLedColor( led, r, g, b );
}

void StatusLED::setLedColor( uint8_t led, uint16_t r, uint16_t g, uint16_t b ){
    if (led&1)
    {
        p_red.setPWM(r);
        p_green.setPWM(g);
        p_blue.setPWM(b);
    }
    
    if (led&2)
    {
        p2_red.setPWM(r);
        p2_green.setPWM(g);
        p2_blue.setPWM(b);
    }
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
            // setLed(vhWifi.connected ? GREEN : BLUE);
            // Serial.printf(" - STATE_RUNNING %s\n", (vhWifi.connected ? "GREEN" : "BLUE(tooth)"));
            setLed(GREEN);
            Serial.printf(" - STATE_RUNNING %s\n", "GREEN");
            break;
        default : // STATE_BOOT
            setLed(CYAN);
            Serial.println(" - STATE_BOOT half blue");

    }

}

long firstPixelHue = 0;

void StatusLED::testTick( int color ){

    for(int i=1; i<3; i++) { // For each pixel in strip...
        // Offset pixel hue by an amount to make one full revolution of the
        // color wheel (range of 65536) along the length of the strip
        // (strip.numPixels() steps):
        // int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
        // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
        // optionally add saturation and value (brightness) (each 0 to 255).
        // Here we're using just the single-argument hue variant. The result
        // is passed through strip.gamma32() to provide 'truer' colors
        // before assigning to each pixel:
        // strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
    
        uint8_t r, g, b;
        uint16_t hue = firstPixelHue + (i * 65536L / 16);
        uint8_t sat=255;
        uint8_t val=255;
        hue = (hue * 1530L + 32768) / 65536;
        
        // Convert hue to R,G,B (nested ifs faster than divide+mod+switch):
        if(hue < 510) {         // Red to Green-1
            b = 0;
            if(hue < 255) {       //   Red to Yellow-1
                r = 255;
                g = hue;            //     g = 0 to 254
            } else {              //   Yellow to Green-1
                r = 510 - hue;      //     r = 255 to 1
                g = 255;
            }
        } else if(hue < 1020) { // Green to Blue-1
            r = 0;
            if(hue <  765) {      //   Green to Cyan-1
                g = 255;
                b = hue - 510;      //     b = 0 to 254
            } else {              //   Cyan to Blue-1
                g = 1020 - hue;     //     g = 255 to 1
                b = 255;
            }
        } else if(hue < 1530) { // Blue to Red-1
        g = 0;
        if(hue < 1275) {      //   Blue to Magenta-1
            r = hue - 1020;     //     r = 0 to 254
            b = 255;
        } else {              //   Magenta to Red-1
            r = 255;
            b = 1530 - hue;     //     b = 255 to 1
        }
        } else {                // Last 0.5 Red (quicker than % operator)
            r = 255;
            g = b = 0;
        }

        // Apply saturation and value to R,G,B, pack into 32-bit result:
        // uint32_t v1 =   1 + val; // 1 to 256; allows >>8 instead of /255
        // uint16_t s1 =   1 + sat; // 1 to 256; same reason
        // uint8_t  s2 = 255 - sat; // 255 to 0
        // return ((((((r * s1) >> 8) + s2) * v1) & 0xff00) << 8) |
               // (((((g * s1) >> 8) + s2) * v1) & 0xff00)       |
               // (((((b * s1) >> 8) + s2) * v1)           >> 8);
        
        
        statusLED.setLedColor( i, r+1, g+1, b+1 );
        // statusLED.setLedColor( i, 
            // (((((r * s1) >> 8) + s2) * v1)), 
            // ((((g * s1) >> 8) + s2) * v1), 
            // ((((b * s1) >> 8) + s2) * v1) );
        // Serial.printf("%i : %i %i %i  |  ", i, r, g, b );
    }
    
    firstPixelHue += 256;
    if (firstPixelHue >= 5*65536)
    {
        firstPixelHue = 0;
    }
    // Serial.printf("firstPixelHue: %i\n", firstPixelHue);
    
    
    
    // statusLED.setLed(OFF);
    
    // // Serial.printf("Stage: %i\n", statusLED.testColor);

    // switch(statusLED.testColor) {

        // case 0 : statusLED.setLed(RED); break;
        // case 1 : statusLED.setLed(GREEN); break;
        // case 2 : statusLED.setLed(BLUE); break;
        // case 3 : statusLED.setLed(YELLOW); break;
        // case 4 : statusLED.setLed(PURPLE); break;
        // case 5 : statusLED.setLed(CYAN); break;
        // case 6 : statusLED.setLed(OFF); break;
        // case 7 : statusLED.setLed(WHITE); break;
        // default : statusLED.setLed(GREEN);

    // }
    
    // ++statusLED.testColor;
    // if( statusLED.testColor >= 9 ){
        // // ledTicker.detach();
        // statusLED.testColor = 0;
    // }
    
}

void StatusLED::test(){

    testColor = 0;
    // ledTicker.attach(2, StatusLED::testTick, 0);
    ledTicker.attach_ms(10, StatusLED::testTick, 0);

}


StatusLED statusLED = StatusLED();
