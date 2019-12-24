#include <Arduino.h>

#include "Configuration.h"
#include "StatusLED.h"
#include "Pwm.h"
#include "Motor.h"

#define LEDTESTDELAY 150

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
    Serial.printf("Firmware Version: %s\n", Configuration::VH_VERSION);

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

