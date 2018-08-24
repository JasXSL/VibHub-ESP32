
/*
	VibHub motor data
*/
#ifndef Motor_h
#define Motor_h
#include <vector>
#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson
#include "TweenDuino.h" // https://github.com/stickywes/TweenDuino
#include "VhProgramStage.h"
#include "Pwm.h"
// Motor class extending PWM class
class Motor{

	public:
		Motor( uint8_t pin_fwd, uint8_t pin_back );										// Creates the motor
		void loadProgram( JsonArray &stages, int repeats );						// Loads a program onto this
		void update();															// Program loop
        void setPWM( uint8_t duty, bool fast_decay = false, bool forward = false );	// Sets the PWM
		void stopProgram();														// Wipes the program
	private:
		TweenDuino::Timeline timeline;											// Tweenduino timeline
		int _repeats;															// Repeats remaining
		bool program_running = false;											// Whether a program is running or not
		std::vector<VhProgramStage> _active_program;							// Vector of program stages
		void playProgram();														// Plays the loaded program
		
    protected:
        float _duty;															// Duty cycle (0-255)
		Pwm pwm_fwd;															// Pwm forward object
		Pwm pwm_back;															// Pwm back object

};

#endif
