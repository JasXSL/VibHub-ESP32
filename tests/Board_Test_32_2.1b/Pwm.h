
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
        Pwm();  // Don't initialize immediately
        void initialize(uint8_t pin);
        void setPWM(uint16_t duty);
        static void disableAll();
        int _channel;

    private:
		  static std::vector<Pwm*> _devices;

};

#endif

