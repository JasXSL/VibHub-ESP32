#include <Arduino.h>

#include "Configuration.h"
#include "StatusLED.h"
#include "Pwm.h"
#include "Motor.h"
#include <WiFi.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>


const char* ssid     = "IoT Crap";
const char* password = "complete trash";


#define LEDTESTDELAY 50
#define FORMAT_SPIFFS_IF_FAILED true

// LED colors
#define OFF     0x0
#define RED     0x1
#define GREEN   0x2
#define BLUE    0x4
#define YELLOW  0x3
#define PURPLE  0x5
#define CYAN    0x6
#define WHITE   0x7

int loops = 0;
std::vector<Motor> motors;

uint32_t linepos = 0;
uint32_t y(){ return linepos; }
uint32_t next_y(){
    linepos += 40;
    return linepos;
}

void setup() {
    
    // Set LED state
    statusLED.initialize();
    statusLED.setLed( 1, RED );
    statusLED.setLed( 2, GREEN );

    motors.push_back(Motor(Configuration::PIN_MOTOR_A_IN1, Configuration::PIN_MOTOR_A_IN2));
    motors.push_back(Motor(Configuration::PIN_MOTOR_B_IN1, Configuration::PIN_MOTOR_B_IN2));
    motors.push_back(Motor(Configuration::PIN_MOTOR_C_IN1, Configuration::PIN_MOTOR_C_IN2));
    motors.push_back(Motor(Configuration::PIN_MOTOR_D_IN1, Configuration::PIN_MOTOR_D_IN2));

    pinMode(Configuration::PIN_NSLEEP, OUTPUT);
    output_disable();
    pinMode(Configuration::PIN_POWEROFF, OUTPUT);
    pinMode(Configuration::PIN_CONFIG_BUTTON, INPUT);
    
    
    Serial.begin(115200);
    delay(1000);
    Serial.println("\nStarting...");
    
    
    //The chip ID is essentially its MAC address(length: 6 bytes).
    uint64_t _chipid = ESP.getEfuseMac();
    char chipid[13] = {0};
    sprintf(chipid, "%04X%08X", (uint16_t)(_chipid>>32), (uint32_t)_chipid);

    // Get MAC address for WiFi station
    uint8_t baseMac[6];
    esp_read_mac(baseMac, ESP_MAC_WIFI_STA);
    char mac[18] = {0};
    char mac_unpadded[13] = {0};
	sprintf(mac, "%02X:%02X:%02X:%02X:%02X:%02X", baseMac[0], baseMac[1], baseMac[2], baseMac[3], baseMac[4], baseMac[5]);
	sprintf(mac_unpadded, "%02X%02X%02X%02X%02X%02X", baseMac[0], baseMac[1], baseMac[2], baseMac[3], baseMac[4], baseMac[5]);

    uint8_t chiprevision = ESP.getChipRevision();
    uint32_t ramsize = ESP.getHeapSize();
    uint32_t flashsize = ESP.getFlashChipSize();
    
    uint32_t spiffsize = 0;
    if(SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)){
        spiffsize = static_cast<uint32_t>(SPIFFS.totalBytes());
    }


    DynamicJsonDocument jsonBuffer(4096);
    JsonObject json = jsonBuffer.to<JsonObject>();
    json["hwver"] = Configuration::VH_HWVERSION;
    json["chipid"] = chipid;
    json["mac"] = mac_unpadded;
    json["chiprev"] = chiprevision;
    json["flashsize"] = flashsize;
    json["spiffsize"] = spiffsize;
    
    
    
    Serial.printf("Board Ver:      %s\n", Configuration::VH_HWVERSION);
    Serial.printf("Firmware:       %s\n", Configuration::VH_VERSION);
    Serial.printf("ESP32 Chip ID:  %s\n",chipid);
    Serial.printf("MAC:            %s\n", mac);
    Serial.printf("Chip Revision:  %d\n", (chiprevision));
    Serial.printf("RAM Size:       %d KB\n", (ramsize/1024));
    Serial.printf("Flash Size:     %d MB\n", (flashsize/1024/1024));
    Serial.printf("SPIFFS Size:    %d KB\n", (spiffsize/1024));
    
    serializeJson(json, Serial);
    
    
    Serial.println("\n\nHold config button to print report...");
    
    int i;
    for (i=0; i<10; i++){
        Serial.print(".");
        delay(200);
        yield();
    }
    
    if (digitalRead(Configuration::PIN_CONFIG_BUTTON) == Configuration::BUTTON_DOWN)
    {
        Serial.println("\nConnecting WIFI...");
        
        WiFi.begin(ssid, password);
        while (WiFi.status() != WL_CONNECTED) {
            delay(500);
            Serial.print(".");
        }
        
        if (WiFi.status() == WL_CONNECTED)
        {
            Serial.print("\nWiFi connected\nIP address: ");
            Serial.println(WiFi.localIP());
            Serial.println();
            
            Serial.println("Printing report...");
            
            WiFiClient client;
            const char * host = "192.168.1.60";
            const uint16_t port = 9100;
            if (!client.connect(host, port))
            {
                Serial.println("Connection to printer failed.");
            }
            else
            {
                Serial.println("Printer found");
            
                // Start print
                client.print("^XA\r\n");
                // Line
                client.print("^FO50,20^GB500,3,4^FS\r\n");
                // Font
                client.print("^CF0,30\r\n");
                
                client.printf("^FO50,%i^FDBoard Ver:      %s^FS\r\n", next_y(), Configuration::VH_HWVERSION);
                client.printf("^FO50,%i^FDFirmware:       %s^FS\r\n", next_y(), Configuration::VH_VERSION);
                
                client.printf("^FO50,%i^FDESP32 Chip ID:  %s^FS\r\n", next_y(), chipid);//print High 2 bytes
                
                client.printf("^FO50,%i^FDMAC:            %s^FS\r\n", next_y(), mac);
                
                client.printf("^FO50,%i^FDChip Revision:  %d^FS\r\n", next_y(), (chiprevision));
                
                client.printf("^FO50,%i^FDRAM Size:       %d KB^FS\r\n", next_y(), (ramsize/1024));
                
                client.printf("^FO50,%i^FDFlash Size:     %d MB^FS\r\n", next_y(), (flashsize/1024/1024));
                
                client.printf("^FO50,%i^FDSPIFFS Size:    %d KB^FS\r\n", next_y(), (spiffsize/1024));
                
                // Line
                client.printf("^FO50,%i^GB500,3,4^FS\r\n", next_y());
                
                // QR Code
                client.printf("^FO50,%i^BQN,2,6,H,7^FDHA,", next_y());
                serializeJson(json, client);
                client.print("^FS\r\n");
            
                // MAC Barcode
                client.printf("^FO740,80^BCR,50,Y,N,N,A^FD%s^FS\r\n", mac_unpadded);
                
                // End Print
                client.print("^XZ\r\n");
            }
        }
        
        // Start print
        Serial.print("^XA\r\n");
        // Line
        Serial.print("^FO50,20^GB500,3,4^FS\r\n");
        // Font
        Serial.print("^CF0,30\r\n");
        
        Serial.printf("^FO50,%i^FDBoard Ver:^FS^FO300,%i^FD%s^FS\r\n", next_y(), y(), Configuration::VH_HWVERSION);
        Serial.printf("^FO50,%i^FDFirmware:^FS^FO300,%i^FD%s^FS\r\n", next_y(), y(), Configuration::VH_VERSION);
        
        Serial.printf("^FO50,%i^FDESP32 Chip ID:^FS^FO300,%i^FD%s^FS\r\n", next_y(), y(), chipid);//print High 2 bytes
        
        Serial.printf("^FO50,%i^FDMAC:^FS^FO300,%i^FD%s^FS\r\n", next_y(), y(), mac);
        
        Serial.printf("^FO50,%i^FDChip Revision:^FS^FO300,%i^FD%d^FS\r\n", next_y(), y(), (chiprevision));
        
        Serial.printf("^FO50,%i^FDRAM Size:^FS^FO300,%i^FD%d KB^FS\r\n", next_y(), y(), (ramsize/1024));
        
        Serial.printf("^FO50,%i^FDFlash Size:^FS^FO300,%i^FD%d MB^FS\r\n", next_y(), y(), (flashsize/1024/1024));
        
        Serial.printf("^FO50,%i^FDSPIFFS Size:^FS^FO300,%i^FD%d KB^FS\r\n", next_y(), y(), (spiffsize/1024));
        
        // Line
        Serial.printf("^FO50,%i^GB500,3,4^FS\r\n", next_y());
        
        // QR Code
        Serial.printf("^FO50,%i^BQN,2,6,H,7^FDHA,", next_y());
        serializeJson(json, Serial);
        Serial.print("^FS\r\n");
        
        // MAC Barcode
        Serial.printf("^FO740,80^BCR,50,Y,N,N,A^FD%s^FS\r\n", mac_unpadded);
        
        // End Print
        Serial.print("^XZ\r\n");
        
        Serial.println();
    }
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
            case 1 : 
                statusLED.setLed( 1, RED );
                statusLED.setLed( 2, BLUE );
                break;
            case 2 : 
                statusLED.setLed( 1, GREEN );
                statusLED.setLed( 2, RED );
                break;
            case 3 : 
                statusLED.setLed( 1, BLUE );
                statusLED.setLed( 2, GREEN );
                break;
            default : 
                statusLED.setLed(OFF);
        }
    }

    // motors[0].setPWM( 255 );
    // motors[1].setPWM( 255 );
    // motors[2].setPWM( 255 );
    // motors[3].setPWM( 255 );

    motors[0].setPWM( (loops%2) * 255 );
    delay(LEDTESTDELAY);
    yield();
    motors[1].setPWM( !(loops%2) * 255 );
    delay(LEDTESTDELAY);
    yield();
    motors[2].setPWM( (loops%2) * 255 );
    delay(LEDTESTDELAY);
    yield();
    motors[3].setPWM( !(loops%2) * 255 );
    delay(LEDTESTDELAY);
    yield();

    if (loops == 2){
        Serial.println("Enabling output");
        output_enable();
    }

    if (loops == 6){
        Serial.println("Powering off");
        digitalWrite(Configuration::PIN_POWEROFF, HIGH);
    }

    if (loops == 7){
        Serial.println("Rave mode activated");
        statusLED.test();
    }

    loops++;
}

