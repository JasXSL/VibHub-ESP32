#include "Arduino.h"
#include "Motor.h"
#include "TweenDuino.h" // https://github.com/stickywes/TweenDuino
#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson
#include "Pwm.h"
#include "RandObject.h"

//#define DEBUG

Motor::Motor( uint8_t pin_fwd, uint8_t pin_back ) :
    _duty(0),
	pwm_fwd(pin_fwd),
	pwm_back(pin_back),
	_repeats(0)
{
    // Force initial state
    //pwm_fwd = Pwm(pin_fwd);
    //pwm_back = Pwm(pin_back);
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

	//Serial.printf("Setting duty: %i on channel %i and 0 on channel %i \n", duty, pwm_fwd._channel, pwm_back._channel);
	_duty = duty;

	pwm_fwd.setPWM(duty);
	pwm_back.setPWM(0);

	/*
	Todo: Kadah I'm not 100% sure how this works, could you look into it?
    if( forward ){

        if( fast_decay ) {

            pwmdriver.set_duty(motor, duty);
            pwmdriver.set_duty(motor+1, 0);

        }
        else{ // slow decay

            pwmdriver.set_duty(motor, 255);
            pwmdriver.set_duty(motor+1, 255-duty);

        }

    }
    else { // reverse

        if( fast_decay ){

            pwmdriver.set_duty(motor, 0);
            pwmdriver.set_duty(motor+1, duty);
        }
        else{ 

			// slow decay
            //Serial.printf("Setting duty: %i = %i, %i = %i \n", motor, 255-duty, motor+1, 255);
            pwmdriver.set_duty(motor, 255-duty);
            pwmdriver.set_duty(motor+1, 255); 

        }

    }
	*/

}

