
/*
	RandObject for VibHub motor programs
	https://github.com/JasXSL/VibHub-Client/wiki/REST-Program-API
*/
#ifndef TweenRandObject_h
#define TweenRandObject_h
#include <ArduinoJson.h>
#include "Configuration.h"
// Motor class extending PWM class
class TweenRandObject{

	public:
		bool isFalse = false;		// If set, it should use the programStage inValue instead

		TweenRandObject(){};
        void load(JsonVariant data);

		// Returns a random value based on settings
		// If the imported value was false, 
		int getValue( uint8_t inValue );
	private:
		void reset();				// Sets things to their default value
		int min = 0;
		int max = 0;
		int offset = 0;
		int multi = 1;
		
		
};

#endif

