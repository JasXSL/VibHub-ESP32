/*
Driver for one channel of a DRV8833
http://www.ti.com/lit/ds/symlink/drv8833.pdf
*/

#include "Arduino.h"
#include "Motor.h"
#include "TweenDuino.h" // https://github.com/stickywes/TweenDuino
#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson
#include "Pwm.h"
#include "RandObject.h"

// #define DEBUG

Motor::Motor( uint8_t pin_in1, uint8_t pin_in2 ) :
    _duty(0),
	pwm_in1(pin_in1),
	pwm_in2(pin_in2),
	_repeats(0)
{
    // Force initial state
    //pwm_in1 = Pwm(pin_in1);
    //pwm_in2 = Pwm(pin_in2);
	setPWM(0);
}

// Plays cached program
void Motor::playProgram(){

	timeline.wipe();
	
	int size = _active_program.size();
	int lastpwm = _duty;
	#ifdef DEBUG
		Serial.printf("PlayProgram, received %i stages\n", size);
		long free = ESP.getFreeHeap();
		Serial.printf("Free memory: %i\n", free);
	#endif

	if(!size){
		stopProgram();
		return;
	}

	int totalDuration = 0;
	for(std::vector<VhProgramStage>::iterator iter = _active_program.begin(); iter != _active_program.end(); iter++) {

		int r;
		int intens = iter->intensity.getValue(lastpwm);
		int lastIntensity = lastpwm;
		int dur = iter->duration.getValue();
		int rep = iter->repeats.getValue();
		if( dur < 1 )
			dur = 1;
		if( rep < 0 )
			rep = 0;
		++rep;
		totalDuration += dur;
		for( r = 0; r < rep; ++r ){

			// Snapback repeats
			if( !iter->yoyo && r ) 
				timeline.addTo(_duty, (float)lastpwm, 1);

			int v = intens;
			if( iter->yoyo && r%2 == 1 )
				v = lastpwm;
			
			//Serial.printf("Added playback from %i to %i over %i ms | yFlip %i\n", lastIntensity, v, dur, yoyo && r%2 == 1);	
			lastIntensity = v;
			timeline.addTo(_duty, (float)v, dur, iter->ease, iter->easeType);

		}

		lastpwm = lastIntensity;

	}

	#ifdef DEBUG
		Serial.printf("Program built, total duration: %i!\n", totalDuration);
	#endif
	program_running = true;
	timeline.begin(millis());

}

void Motor::loadProgram( JsonArray &stages, int repeats = 0 ){

	#ifdef DEBUG
		Serial.println();
		Serial.printf("Loading new program with #%i stages.\n", stages.size());
	#endif
	_repeats = repeats;

	// Todo: This crashes the ESP. What de fook bork bork
	std::vector<VhProgramStage>().swap(_active_program);
	for( auto stage : stages )
		_active_program.push_back(VhProgramStage(stage.as<JsonObject>()));
	
	playProgram();

}

void Motor::stopProgram(){
	program_running = false;
}

void Motor::update(){

	if(!program_running)
		return;

	uint32_t time = millis();
	timeline.update(time);

	if( timeline.isComplete() ){
		// Handle repeats
		if(_repeats == -1 || _repeats > 0){
			#ifdef DEBUG
				Serial.println("Program completed, looping");
			#endif
			playProgram();	// needed for randObjects. Memory seems fine anyways.
			if( _repeats > 0 )
				--_repeats;
		}
		else{
			stopProgram();
			//Serial.printf("Program is complete, and duty is %i \n", _duty);
		}
	}

	//Serial.printf("Setting program duty: %f\n", _duty);
	setPWM(_duty);

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
            //Serial.printf("Setting duty: %i = %i, %i = %i \n", motor, 255-duty, motor+1, 255);
            pwm_in1.setPWM(255-duty);
            pwm_in2.setPWM(255); 

        }

    }

}

