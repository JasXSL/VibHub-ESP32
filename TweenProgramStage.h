
/*
	RandObject for VibHub motor programs
	https://github.com/JasXSL/VibHub-Client/wiki/REST-Program-API
*/
#ifndef TweenProgramStage_h
#define TweenProgramStage_h
#include <ArduinoJson.h>
#include "TweenRandObject.h"
#include "TweenEasing.h"

using namespace TweenEasing;

// Motor class extending PWM class
class TweenProgramStage{

	public:
		// Randobjects that get triggered when TweenProgramStage::reset() is ran 
		TweenRandObject in;
		TweenRandObject du;
		TweenRandObject re;
		int intensity;
		int duration;
		int repeats;

		Ease ease;
		EaseType easeType;
		
		bool yoyo = false;
		TweenProgramStage(JsonObject &s);		// Constructor
		void reset();							// Resets at the start of a program
		long getDuration();						// Gets the total duration of this stage
		float inValue;							// Set when the program starts with the out value of the previous stage
		float outValue();						// Gets the final outValue, requires inValue to be set
		float getValueAtDelta( long internalDelta );	// Gets the value at delta relative to the program start

};

#endif

