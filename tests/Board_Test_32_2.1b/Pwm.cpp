/*

    This class handles PWM in a raw form
    The LedC channels are handled automatically when a new object is created
    You just need to supply a pin

*/
#include <Arduino.h>
#include "Pwm.h"
#include "Configuration.h"

std::vector<Pwm*> Pwm::_devices;
int devices_bound = 0;

Pwm::Pwm(){}
Pwm::Pwm( uint8_t pin ){
    initialize(pin);  
}

void Pwm::initialize( uint8_t pin ){

    _channel = Pwm::_devices.size();
    pinMode(pin, OUTPUT);
    ledcSetup(_channel, Configuration::PWM_FREQ, Configuration::PWM_RESOLUTION);
    ledcAttachPin(pin, _channel);
    ++devices_bound;
    // Serial.printf("BOUND PIN %i to channel %i (Total devices: %i, %i)\n", pin, _channel, Pwm::_devices.size(), devices_bound);

    Pwm::_devices.push_back(this);
    setPWM(0);

}

void Pwm::setPWM( uint16_t duty ){

    //Serial.printf("PWM of channel %i = %i\n", _channel, duty);
    ledcWrite(_channel, duty);

}

void Pwm::disableAll(){


    for(char i =0; i<Pwm::_devices.size(); ++i )
        Pwm::_devices[i]->setPWM(0);

}
