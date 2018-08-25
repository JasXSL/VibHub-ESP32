
/*
	RandObject for VibHub motor programs
	https://github.com/JasXSL/VibHub-Client/wiki/REST-Program-API
*/
#ifndef RandObject_h
#define RandObject_h
#include <ArduinoJson.h>
#include "Configuration.h"
// Motor class extending PWM class
class RandObject{

	public:
		RandObject(){};
        void load(JsonVariant data){
			reset();
			if(data.is<bool>())
				useDefault = true;
			
			else if(data.is<int>())
				min = max = data.as<int>();
			
			else if(data.is<JsonObject>()){

				JsonObject &d = data.as<JsonObject&>();
				if(d.containsKey("min"))
					min = d["min"];
				if(d.containsKey("max"))
					max = d["max"];
				if(d.containsKey("offset"))
					offset = d["offset"];
				if(d.containsKey("multi"))
					multi = d["multi"];
					
			}
			else
				Serial.println("RandObject is INVALID");
			
		}

		// Returns a random value based on settings
		// If the imported value was false, 
		int getValue(int valueOnFalse = 0){
			if(useDefault)
				return valueOnFalse;
			//Serial.printf("Returning a random value between min %i, max %i, multi %i, offset %i: %i\n", min, max, multi, offset, out);
			return Configuration::espRandBetween(min, max)*multi+offset;
		}

	private:
		void reset(){
			min = 0;
			max = 0;
			offset = 0;
			multi = 1;
			useDefault = false;
		}
		int min = 0;
		int max = 0;
		int offset = 0;
		int multi = 1;
		bool useDefault = false;

};

#endif

