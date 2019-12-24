
/*
	VibHub motor data
*/
#ifndef Motor_h
#define Motor_h
#include <vector>
// #include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson
// #include "TweenProgram.h"
#include "Pwm.h"
// Motor class extending PWM class
class Motor{

	public:
		Motor( uint8_t pin_in1, uint8_t pin_in2 );								// Creates the motor
		// void loadProgram( JsonArray stages, int repeats );						// Loads a program onto this
		// void update();															// Program loop
        void setPWM( uint8_t duty, bool fast_decay = false, bool forward = false );	// Sets the PWM
		// void stopProgram();														// Sets the program as completed
		// TweenProgram program;
		bool running();															// Returns true if the duty is > 0

    protected:
        uint8_t _duty;															// Duty cycle (0-255)
		Pwm pwm_in1;															// Pwm forward object
		Pwm pwm_in2;															// Pwm back object

};

#endif
