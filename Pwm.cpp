/*

    This class handles PWM in a raw form
    The LedC channels are handled automatically when a new object is created
    You just need to supply a pin

*/
#include <Arduino.h>
#include "Pwm.h"
#include "Configuration.h"

std::vector<Pwm> Pwm::_devices;

Pwm::Pwm( uint8_t pin ){

    _channel = Pwm::_devices.size();
    pinMode(pin, OUTPUT);
    ledcSetup(_channel, Configuration::PWM_FREQ, Configuration::PWM_RESOLUTION);
    ledcAttachPin(pin, _channel);
    
    Serial.printf("Bound pin %i to channel %i\n", pin, _channel);

    Pwm::_devices.push_back(*this);
    setPWM(0);
    
    
}

void Pwm::setPWM( uint8_t duty ){

    //Serial.printf("Setting PWM of channel %i to %i\n", _channel, duty);
    ledcWrite(_channel, duty);

}

/*
Todo: Move to motor
void Pwm::setPWM( uint8_t duty, bool fast_decay, bool forward ){

    // double motor index to give us the pwm pin offset
    // Double are for if you want to reverse to make it "choppy"
    motor = motor*2;
    
    if (forward) {
        if (fast_decay) {
            pwmdriver.set_duty(motor, duty);
            pwmdriver.set_duty(motor+1, 0);
        }
        else { // slow decay
            pwmdriver.set_duty(motor, 255);
            pwmdriver.set_duty(motor+1, 255-duty);
        }
    }
    else { // reverse
        if (fast_decay) {
            pwmdriver.set_duty(motor, 0);
            pwmdriver.set_duty(motor+1, duty);
        }
        else { // slow decay
            //Serial.printf("Setting duty: %i = %i, %i = %i \n", motor, 255-duty, motor+1, 255);
            pwmdriver.set_duty(motor, 255-duty);
            pwmdriver.set_duty(motor+1, 255); 
        }
    }
}
*/
void Pwm::disableAll(){

    for(char i =0; i<Pwm::_devices.size(); ++i )
        Pwm::_devices[i].setPWM(0);

}
