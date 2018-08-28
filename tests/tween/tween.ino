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

void setup() {

	// Serial setup
    Serial.begin(115200);
    //Serial.setDebugOutput(true);
    delay(100);
    Serial.println("\nStarting...");

	program = TweenProgram(0);
	
	DynamicJsonBuffer jsonBuffer;
	JsonObject& root = jsonBuffer.createObject();
	JsonObject& r = root.createNestedObject("i");
	r["min"] = 0;
	r["max"] = 255;
	program.addStageFromJson(root);

	
	pinMode(15, OUTPUT);
	ledcSetup(0, 8000, 8);
  	ledcAttachPin(15, 0);
	ledcWrite(0, 0);

	program.start();
	

}

void loop(){
	if( program.loop() ){
		Serial.printf("Program value %f \n", program.value);
		ledcWrite(0, round(program.value));
		delay(100);
	}
	else
		delay(1000);
}