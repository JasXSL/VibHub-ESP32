#include "TweenProgramStage.h"
#include <vector>
#include "TweenEasing.h"
using namespace TweenEasing;


TweenProgramStage::TweenProgramStage(JsonObject &s){

	in = TweenRandObject();
	du = TweenRandObject();
	re = TweenRandObject();

	ease = Ease::LINEAR;
	easeType = EaseType::IN;
	yoyo = false;

	char easing[48] = "Linear.In";
	if( s.containsKey("i") ){
		in.load(s["i"]);
	}
	if( s.containsKey("d") )
		du.load(s["d"]);
	if( s.containsKey("e") )
		strcpy(easing, s["e"]);
	if( s.containsKey("r") )
		re.load(s["r"]);
	if( s.containsKey("y") )
		yoyo = s.get<int>("y") ? true : false;

	// Figure out the easing
	char *token = strtok(easing, ".");
	std::vector<char*> tokens;
	while( token ){

		tokens.push_back(token);
		token = strtok(NULL,".");

	}

	if( tokens.size() >= 2 ){

		// Translate into a const
		if( strcmp(tokens[0], "Quadratic") == 0 )
			ease = Ease::QUAD;
		else if( strcmp(tokens[0], "Cubic") == 0 )
			ease = Ease::CUBIC;
		else if( strcmp(tokens[0], "Quartic") == 0 )
			ease = Ease::QUART;
		else if( strcmp(tokens[0], "Quintic") == 0 )
			ease = Ease::QUINT;
		else if( strcmp(tokens[0], "Sinusoidal") == 0 )
			ease = Ease::SINE;
		else if( strcmp(tokens[0], "Exponential") == 0 )
			ease = Ease::EXPONENTIAL;
		else if( strcmp(tokens[0], "Circular") == 0 )
			ease = Ease::CIRCULAR;
		else if( strcmp(tokens[0], "Elastic") == 0 )
			ease = Ease::ELASTIC;
		else if( strcmp(tokens[0], "Back") == 0 )
			ease = Ease::BACK;
		else if( strcmp(tokens[0], "Bounce") == 0 )
			ease = Ease::BOUNCE;
		else if( strcmp(tokens[0], "None") == 0 )
			ease = Ease::NONE;
		

		if( strcmp(tokens[1], "Out") == 0 )
			easeType = EaseType::OUT;
		else if( strcmp(tokens[1], "InOut") == 0 )
			easeType = EaseType::INOUT;
		

	}
	
}


void TweenProgramStage::reset(){

	intensity = in.getValue(inValue);
	duration = du.getValue(inValue);
	repeats = re.getValue(inValue);
	if( duration < 0 )
		duration = 0;
	if( repeats < 0 )
		repeats = 0;
	if( intensity > 255 )
		intensity = 255;
	else if( intensity < 0 )
		intensity = 0;
	
}

long TweenProgramStage::getDuration(){

	return duration*(repeats+1);

}

float TweenProgramStage::outValue(){
	return yoyo && repeats%2 ? inValue : intensity;
}

float TweenProgramStage::getValueAtDelta( long internalDelta ){

	if( internalDelta >= getDuration() )
		return outValue();
	

	float b = inValue;					// Start value
	float c = intensity-b;				// Change
	// Reverse if yoyo and repeat is odd number
	if( (internalDelta/duration)%2 && yoyo ){
		b = intensity;
		c = inValue-b;
	}
	float t = internalDelta%duration;	// Time in MS of the stage
	float d = duration;

	//Serial.printf("Calculate tween value: Start %f, Change %f, Current Time %f, Total Duration %f \n", b, c, t, d);

	switch(ease){
		case Ease::NONE:
			return easeNone(b,c,t,d);		// Constent tween
		case Ease::BACK:
			return easeBack(b,c,t,d,easeType);
		case Ease::SINE:
			return easeSine(b,c,t,d,easeType);

	}
	// Linear case
	return easeLinear(b,c,t,d);	// Linear formula

}

