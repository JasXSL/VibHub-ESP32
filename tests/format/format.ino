#include <SPIFFS.h>

void setup() {
    Serial.println("Formating file system");
    #if defined(ESP8266)
      if (SPIFFS.begin()) SPIFFS.format();
      else Serial.println("Failed to mount file system");
    #elif defined(ESP32)
        if(SPIFFS.begin(true)) SPIFFS.format();
        else Serial.println("Failed to mount file system");
    #endif
}

void loop() {
  // put your main code here, to run repeatedly:

}
