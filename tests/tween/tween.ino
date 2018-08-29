/*

	New tweening engine for VibHub.
	Needed unit tests: 

	- Memory test
	- RandObjects
	- Destructors?
	- Infinite loops

*/

#include <arduino.h>
#include <vector>
#include "TweenProgram.h"
#include <ArduinoJson.h>
TweenProgram program;
int step = 0;


void defaultProgramRepeatTwice(){
	Serial.println("Sinusoidal in out back and forth twice");
	program.reset(0);
	
	DynamicJsonBuffer jsonBuffer;
	JsonObject& root = jsonBuffer.createObject();
	root["i"] = 255;
	root["d"] = 1000;
	root["e"] = "Sinusoidal.InOut";
	root["r"] = 1;
	root["y"] = true;
	program.addStageFromJson(root);
		
	program.start();
}

void randomThreeStages(){
	Serial.println("3 random colors at 1s intervals");
	program.reset(0);
	
	DynamicJsonBuffer jsonBuffer;
	JsonObject& root = jsonBuffer.createObject();
	JsonObject& r = root.createNestedObject("i");
	r["min"] = 0;
	r["max"] = 255;
	root["e"] = "None.None";
	root["d"] = 1000;
	program.addStageFromJson(root);
	program.addStageFromJson(root);
	program.addStageFromJson(root);

	program.start();

}

void downFromLastValue(){

	Serial.println("Bouncing to 0 from last value");
	program.reset(0);
	
	DynamicJsonBuffer jsonBuffer;
	JsonObject& root = jsonBuffer.createObject();
	root["i"] = 0;
	root["e"] = "Bounce.Out";
	root["d"] = 2000;
	program.addStageFromJson(root);

	program.start();

}

void oneToThreeFlashes(){

	Serial.println("1-3 quartic in out at 1-3 seconds each");
	program.reset(0);
	
	DynamicJsonBuffer jsonBuffer;
	JsonObject& root = jsonBuffer.createObject();
	root["i"] = 0;
	program.addStageFromJson(root);

	JsonObject& d = root.createNestedObject("d");
	d["min"] = 1000;
	d["max"] = 3000;
	JsonObject& r = root.createNestedObject("r");
	r["min"] = 0;
	r["max"] = 2;
	r["offset"] = 1;
	r["multi"] = 2;
	root["i"] = 255;
	root["e"] = "Quartic.InOut";
	root["y"] = true;
	program.addStageFromJson(root);
	program.start();

}

void infiniteRandomSinusoid(){

	Serial.println("Random sinusoidal flashes infinite, starting at 0");
	program.reset(-1);
	
	DynamicJsonBuffer jsonBuffer;
	JsonObject& root = jsonBuffer.createObject();
	program.addStageFromJson(root);	// Reset by adding an empty object

	JsonObject& r = root.createNestedObject("i");
	r["min"] = 0;
	r["max"] = 255;
	root["e"] = "Sinusoidal.InOut";
	root["d"] = 1000;
	root["y"] = true;
	root["r"] = 1;
	program.addStageFromJson(root);
	program.start();

}


void setup() {

	// Serial setup
    Serial.begin(115200);
    //Serial.setDebugOutput(true);
    delay(100);
    Serial.println("\nStarting...");
	pinMode(15, OUTPUT);
	ledcSetup(0, 8000, 8);
  	ledcAttachPin(15, 0);
	ledcWrite(0, 0);
	
	defaultProgramRepeatTwice();

}

void loop(){
	if( program.loop() ){
		Serial.printf("Program value %f \n", program.value);
		ledcWrite(0, round(program.value));
		delay(17);
	}
	else{
		++step;
		if(step == 1)
			randomThreeStages();
		else if(step == 2)
			downFromLastValue();
		else if(step == 3)
			oneToThreeFlashes();
		else if(step == 4)
			Serial.println("Done");
			//infiniteRandomSinusoid();
		else
			delay(1000);
	}
}