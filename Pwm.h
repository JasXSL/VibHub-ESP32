
/*
  VibHub motor data
*/
#ifndef Pwm_h
#define Pwm_h

#include "Arduino.h"
#include <vector>

class Pwm{

    public:
        Pwm(uint8_t pin);
        void setPWM(uint8_t duty);
        static void disableAll();
        
    private:
		int _channel;
		static std::vector<Pwm> _devices;

};

#endif

