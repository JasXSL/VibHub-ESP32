#include <ArduinoJson.h>

void setup(){

	Serial.begin(115200);
	Serial.println("BEGIN");
	DynamicJsonDocument jsonBuffer(1024);
	JsonObject json = jsonBuffer.to<JsonObject>();
	json["a"] = 1337;

	serializeJson(jsonBuffer, Serial);

	DynamicJsonDocument doc(1024);

	// create an object
	JsonObject object = doc.to<JsonObject>();
	object["hello"] = "world";

	// serialize the object and send the result to Serial
	serializeJson(doc, Serial);

}
void loop(){}
