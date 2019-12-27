#include <Arduino.h>

#include "Configuration.h"
#include "StatusLED.h"
#include "Pwm.h"
#include "Motor.h"
#include <WiFi.h>
#include <SPIFFS.h>

#define LEDTESTDELAY 150
#define FORMAT_SPIFFS_IF_FAILED true

// LED colors
#define RED     0x1
#define GREEN   0x2
#define BLUE    0x4
#define YELLOW  0x3
#define PURPLE  0x5
#define CYAN    0x6
#define OFF     0x0
#define WHITE   0x7

int loops = 0;
std::vector<Motor> motors;


void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("\nStarting...");

    Serial.printf("Board Version: %s\n", Configuration::VH_HWVERSION);
    Serial.printf("Firmware Version: %s\n\n", Configuration::VH_VERSION);
    
    uint64_t chipid = ESP.getEfuseMac();//The chip ID is essentially its MAC address(length: 6 bytes).
    Serial.printf("ESP32 Chip ID: %04X",(uint16_t)(chipid>>32));//print High 2 bytes
    Serial.printf("%08X\n",(uint32_t)chipid);//print Low 4bytes.

    uint8_t baseMac[6];
	// Get MAC address for WiFi station
	esp_read_mac(baseMac, ESP_MAC_WIFI_STA);
	char baseMacChr[18] = {0};
	Serial.printf("MAC: %02X:%02X:%02X:%02X:%02X:%02X\n", baseMac[0], baseMac[1], baseMac[2], baseMac[3], baseMac[4], baseMac[5]);

    uint8_t chiprevision = ESP.getChipRevision();
    Serial.printf("Chip Revision: %i\n", (chiprevision));
    
    uint32_t ramsize = ESP.getHeapSize();
    Serial.printf("RAM Size: %iKB\n", (ramsize/1024));

    uint32_t flashsize = ESP.getFlashChipSize();
    Serial.printf("Flash Size: %iMB\n", (flashsize/1024/1024));
    
    if(SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)){
        uint32_t flashsize = static_cast<uint32_t>(SPIFFS.totalBytes());
        Serial.printf("SPIFFS Size: %iKB\n\n", (flashsize/1024));
    }


    // Set LED state
    statusLED.initialize();
    statusLED.setLed(OFF);

    motors.push_back(Motor(Configuration::PIN_MOTOR_A_IN1, Configuration::PIN_MOTOR_A_IN2));
	motors.push_back(Motor(Configuration::PIN_MOTOR_B_IN1, Configuration::PIN_MOTOR_B_IN2));
	motors.push_back(Motor(Configuration::PIN_MOTOR_C_IN1, Configuration::PIN_MOTOR_C_IN2));
	motors.push_back(Motor(Configuration::PIN_MOTOR_D_IN1, Configuration::PIN_MOTOR_D_IN2));

    pinMode(Configuration::PIN_NSLEEP, OUTPUT);
    output_disable();

    pinMode(Configuration::PIN_POWEROFF, OUTPUT);

    // statusLED.test();
}

void output_enable(){
    digitalWrite(Configuration::PIN_NSLEEP, HIGH);
}
void output_disable(){  
    digitalWrite(Configuration::PIN_NSLEEP, LOW);
}

void loop() {

    if (loops <= 7){

        Serial.print("Loop : ");
        Serial.println(loops);
        
        switch(loops % 4) {
            case 1 : statusLED.setLed(RED); break;
            case 2 : statusLED.setLed(GREEN); break;
            case 3 : statusLED.setLed(BLUE); break;
            default : statusLED.setLed(OFF);
        }
        
        
        int i;
        for (i=0; i<4; i++){
            motors[i].setPWM(255);
            delay(LEDTESTDELAY);
            yield();
            motors[i].setPWM(0);
            delay(LEDTESTDELAY);
            yield();
            delay(LEDTESTDELAY);
            yield();
        }
        
        if (loops == 2){
            Serial.println("Enabling output");
            output_enable();
        }
        
        if (loops == 6){
            Serial.println("Powering off");
            digitalWrite(Configuration::PIN_POWEROFF, HIGH);
        }

        loops++;
    }
}

