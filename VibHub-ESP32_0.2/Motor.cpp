/*
Driver for one channel of a DRV8833
http://www.ti.com/lit/ds/symlink/drv8833.pdf
*/

#include "Arduino.h"
#include "Motor.h"
#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson

#define DEBUG

Motor::Motor( uint8_t pin_in1, uint8_t pin_in2 ) :
    _duty(0),
	pwm_in1(pin_in1),
	pwm_in2(pin_in2)
{
    // Force initial state
    //pwm_in1 = Pwm(pin_in1);
    //pwm_in2 = Pwm(pin_in2);
	setPWM(0);
}

void Motor::loadProgram( JsonArray &stages, int repeats = 0 ){

	#ifdef DEBUG
		Serial.println();
		Serial.printf("Loading new program with #%i stages.\n", stages.size());
	#endif
	
    program.completed = true;
	program.reset(repeats);
	for( auto stage : stages )
		program.addStageFromJson(stage.to<JsonObject>());
	
    Serial.println("Calling program start");
	program.start();

}

void Motor::stopProgram(){
	program.completed = true;	// Makes sure we don't run loop on the program
	program.reset(0);			// Frees up some memory
}

void Motor::update(){

	if( !program.loop() )
		return;

	_duty = floor(program.value);
	if( _duty < 0 )
		_duty = 0;
	else if( _duty > 255 )
		_duty = 255;

	setPWM(_duty);

}

bool Motor::running(){
    return _duty > 0;
}

// fast_decay and forward are false by default
void Motor::setPWM( uint8_t duty, bool fast_decay, bool forward ){

    #ifdef DEBUG
        Serial.printf("Setting duty: %3i on channels %3i and %2i, fast_decay: %5s, forward: %s\n", duty, pwm_in1._channel, pwm_in2._channel, fast_decay ? "true" : "false", forward ? "true" : "false");
    #endif
    
	_duty = duty;

    /*
    http://www.ti.com/lit/ds/symlink/drv8833.pdf
    7.3.2 Bridge Control and Decay Modes
    
    The AIN1 and AIN2 input pins control the state of the AOUT1 and AOUT2 outputs; similarly, the BIN1 and BIN2
    input pins control the state of the BOUT1 and BOUT2 outputs. Table 1 shows the logic.
    
        Table 1. H-Bridge Logic
        xIN1    xIN2    xOUT1   xOUT2   FUNCTION
        0       0       Z       Z       Coast/fast decay
        0       1       L       H       Reverse
        1       0       H       L       Forward
        1       1       L       L       Brake/slow decay
    
    The inputs can also be used for PWM control of the motor speed. When controlling a winding with PWM, when
    the drive current is interrupted, the inductive nature of the motor requires that the current must continue to flow.
    This is called recirculation current. To handle this recirculation current, the H-bridge can operate in two different
    states: fast decay or slow decay. In fast decay mode, the H-bridge is disabled and recirculation current flows
    through the body diodes; in slow decay, the motor winding is shorted.
    To PWM using fast decay, the PWM signal is applied to one xIN pin while the other is held low; to use slow
    decay, one xIN pin is held high.
    
    To PWM using fast decay, the PWM signal is applied to one xIN pin while the other is held low; to use slow
    decay, one xIN pin is held high.

        Table 2: PWM Control of Motor Speed
        xIN1    xIN2    FUNCTION
        PWM     0       Forward PWM, fast decay
        1       PWM     Forward PWM, slow decay
        0       PWM     Reverse PWM, fast decay
        PWM     1       Reverse PWM, slow decay
    */
    
    if( forward ){

        if( fast_decay ) {

            pwm_in1.setPWM(duty);
            pwm_in2.setPWM(0);

        }
        else{ // slow decay

            pwm_in1.setPWM(255);
            pwm_in2.setPWM(255-duty);

        }

    }
    else { // reverse

        if( fast_decay ){

            pwm_in1.setPWM(0);
            pwm_in2.setPWM(duty);
        }
        else{ 

			// slow decay
            pwm_in1.setPWM(255-duty);
            pwm_in2.setPWM(255); 

        }

    }

}

