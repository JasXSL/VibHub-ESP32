
/*
	RandObject for VibHub motor programs
	https://github.com/JasXSL/VibHub-Client/wiki/REST-Program-API
*/
#ifndef VhProgramStage_h
#define VhProgramStage_h
#include <ArduinoJson.h>
#include "RandObject.h"
// Motor class extending PWM class
class VhProgramStage{

	public:
		VhProgramStage(JsonObject &s){
			intensity = RandObject();
			duration = RandObject();
			repeats = RandObject();
			ease = TweenDuino::Tween::LINEAR;
			easeType = TweenDuino::Tween::IN;
			yoyo = false;

			#ifdef DEBUG
				Serial.printf("Building stage object\n");
			#endif
			char easing[48] = "Linear.In";

			if( s.containsKey("i") )
				intensity.load(s["i"]);
			if( s.containsKey("d") )
				duration.load(s["d"]);
			if( s.containsKey("e") )
				strcpy(easing, s["e"]);
			if( s.containsKey("r") )
				repeats.load(s["r"]);
			if( s.containsKey("y") )
				yoyo = s.get<int>("y") ? true : false;

			char *token = strtok(easing, ".");
			std::vector<char*> tokens;
			while( token ){

				tokens.push_back(token);
				token = strtok(NULL,".");

			}

			if( tokens.size() >= 2 ){

				#ifdef DEBUG
					Serial.printf("Building Easing: %s.%s\n", tokens[0], tokens[1]);
				#endif
				// Translate into a const
				if( strcmp(tokens[0], "Quadratic") == 0 )
					ease = TweenDuino::Tween::QUAD;
				else if( strcmp(tokens[0], "Cubic") == 0 )
					ease = TweenDuino::Tween::CUBIC;
				else if( strcmp(tokens[0], "Quartic") == 0 )
					ease = TweenDuino::Tween::QUART;
				else if( strcmp(tokens[0], "Quintic") == 0 )
					ease = TweenDuino::Tween::QUINT;
				else if( strcmp(tokens[0], "Sinusoidal") == 0 )
					ease = TweenDuino::Tween::SINE;
				else if( strcmp(tokens[0], "Exponential") == 0 )
					ease = TweenDuino::Tween::EXPONENTIAL;
				else if( strcmp(tokens[0], "Circular") == 0 )
					ease = TweenDuino::Tween::CIRCULAR;
				else if( strcmp(tokens[0], "Elastic") == 0 )
					ease = TweenDuino::Tween::ELASTIC;
				else if( strcmp(tokens[0], "Back") == 0 )
					ease = TweenDuino::Tween::BACK;
				else if( strcmp(tokens[0], "Bounce") == 0 )
					ease = TweenDuino::Tween::BOUNCE;
				#ifdef DEBUG
				else{
					Serial.printf("Unknown easing function: %s\n", tokens[0]);
				}
				#endif
				
				if( strcmp(tokens[1], "In") == 0 )
					easeType = TweenDuino::Tween::IN;
				else if( strcmp(tokens[1], "Out") == 0 )
					easeType = TweenDuino::Tween::OUT;
				else if( strcmp(tokens[1], "InOut") == 0 )
					easeType = TweenDuino::Tween::INOUT;
				

			}

		};

		RandObject intensity;
		RandObject duration;
		TweenDuino::Tween::Ease ease = TweenDuino::Tween::LINEAR;
		TweenDuino::Tween::EaseType easeType = TweenDuino::Tween::IN;
		RandObject repeats;
		bool yoyo = false;

};

#endif

