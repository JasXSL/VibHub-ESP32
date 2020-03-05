#include <Arduino.h>
#include <SPIFFS.h>

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("Formating file system");
    #if defined(ESP8266)
      if (SPIFFS.begin()) SPIFFS.format();
      else Serial.println("Failed to mount file system");
    #elif defined(ESP32)
        if(SPIFFS.begin(true)) {
            SPIFFS.format();
            Serial.println("Formated.");
        }
        else {
            Serial.println("Failed to mount and/or format SPIFFS.");
        }
    #endif
}

void loop() {
}
