#include "Configuration.h"

// drv8833 driver(PCA9634_ALL_CALL_ADDR, PIN_ENABLE);


uint8_t outs[8] = {
    Configuration::PIN_MOTOR_A_IN1,
    Configuration::PIN_MOTOR_A_IN2,
    Configuration::PIN_MOTOR_B_IN1,
    Configuration::PIN_MOTOR_B_IN2,
    Configuration::PIN_MOTOR_C_IN1,
    Configuration::PIN_MOTOR_C_IN2,
    Configuration::PIN_MOTOR_D_IN1,
    Configuration::PIN_MOTOR_D_IN2
};


void setPin(uint8_t ch, uint8_t duty) {
    ledcWrite(ch, duty);
}


// Blinks enable pin at 10hz for 2 seconds
void blink_enable(){
	bool blink = false;
	for (int j=0; j<20; j++){
		digitalWrite(Configuration::PIN_NSLEEP, blink);
		blink = !blink;
		delay(100);
	}
}

//
#define LEDTESTDELAY 150
void pwmn_wave() {
	unsigned long timer = 0;
	int i; int j; int k;
	int ledValue;
	// Turn all off
	for (i=0; i<8; i++){
		setPin(i, 0);
	}
	
	// Sequence through each led channel one at a time
	Serial.println("LED Seq");
	for (j=0; j<2; j++){
		for (i=0; i<8; i++){
			setPin(i, 255);
			delay(LEDTESTDELAY);
			setPin(i, 0);
		}
		for (i=6; i>0; i--){ // Missing the first and last is intentional
			setPin(i, 255);
			delay(LEDTESTDELAY);
			setPin(i, 0);
		}
		yield();
	}
	// End sequence by manaully doing first
	setPin(0, 255);
	delay(LEDTESTDELAY);
	setPin(0, 0);
}

//
// void _motor_wave(bool fast_decay, bool forward){
	// unsigned long timer = 0;
	// for (int j=0; j<300; j++){
		// timer = millis();
		
		// driver.setMotor(0, (128 + (127 * cos((2*PI/2000 * timer) + PI*2))), fast_decay, forward);
		// driver.setMotor(1, (128 + (127 * cos((2*PI/2000 * timer) + PI*2))), fast_decay, forward);
		// driver.setMotor(2, (128 + (127 * cos((2*PI/2000 * timer) + PI*2))), fast_decay, forward);
		// driver.setMotor(3, (128 + (127 * cos((2*PI/2000 * timer) + PI*2))), fast_decay, forward);
		
		// yield();
		// delay(20);
	// }
// }
// void motor_wave() {
	// Serial.println("Fast Decay, Forward");
	// _motor_wave(true, true);
	// Serial.println("Fast Decay, Reverse");
	// _motor_wave(true, false);
	// Serial.println("Slow Decay, Forward");
	// _motor_wave(false, true);
	// Serial.println("Slow Decay, Reverse");
	// _motor_wave(false, false);
// }

void motor_reset_channels(){
	// driver.setMotor(0, 0, true, true);
	// driver.setMotor(1, 0, true, true);
	// driver.setMotor(2, 0, true, true);
	// driver.setMotor(3, 0, true, true);
	for (int i=0; i<8; i++){
		setPin(i, 0);
	}
}

// #define CHANNELTESTDELAY 300
// void motor_test_channel(uint8_t ch){
	
	// Serial.print("Testing motor channel: ");
	// Serial.println(ch);
	// driver.enable();
	
	// Serial.println("Fast Decay, Off");
	// driver.setMotor(ch, 0, true, true);
	// delay(CHANNELTESTDELAY);
	
	// Serial.println("Fast Decay, Forward, On");
	// driver.setMotor(ch, 255, true, true);
	// delay(CHANNELTESTDELAY);
	
	// Serial.println("Fast Decay, Reverse, On");
	// driver.setMotor(ch, 255, true, false);
	// delay(CHANNELTESTDELAY);
	
	
	// Serial.println("Slow Decay, Off");
	// driver.setMotor(ch, 0, false, true);
	// delay(CHANNELTESTDELAY);
	
	// Serial.println("Slow Decay, Forward, On");
	// driver.setMotor(ch, 255, false, true);
	// delay(CHANNELTESTDELAY);
	
	// Serial.println("Slow Decay, Reverse, On");
	// driver.setMotor(ch, 255, false, false);
	// delay(CHANNELTESTDELAY);
	
	
	// // Turn off
	// driver.setMotor(ch, 0, true, true);
	// driver.disable();
	// delay(CHANNELTESTDELAY);
// }


void setup() {
    Serial.begin(115200);
    
    Serial.println("Started");
    
    for (int i=0; i<8; i++){
        pinMode(outs[i], OUTPUT);
        ledcSetup(i, Configuration::PWM_FREQ, Configuration::PWM_RESOLUTION);
        ledcAttachPin(outs[i], i);
	}
    
	motor_reset_channels();
	blink_enable();
	// driver.disable();
	
}

unsigned int loops = 0;

void loop() {
	
	Serial.print("\nLoop : ");
	Serial.println(loops);
	loops++;
	
	Serial.println("PWM Wave");
	// driver.disable();
	pwmn_wave();
	
	motor_reset_channels();
	
	// Serial.println("Motor Channel Test");
	// motor_test_channel(0);
	// motor_test_channel(1);
	// motor_test_channel(2);
	// motor_test_channel(3);
	
	// Serial.println("Motor Wave");
	// driver.enable();
	// motor_wave();
}

