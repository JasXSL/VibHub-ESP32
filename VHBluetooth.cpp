#include "VHBluetooth.h"
#include "ApiClient.h"

uint32_t MySecurity::onPassKeyRequest(){
	Serial.println("PassKeyRequest");
	return 123456;
}
void MySecurity::onPassKeyNotify(uint32_t pass_key){
	Serial.printf("The passkey Notify number:%d\n", pass_key);
}
bool MySecurity::onConfirmPIN(uint32_t pass_key){
	Serial.printf("The passkey YES/NO number:%d\n", pass_key);
	vTaskDelay(5000);
	return true;
}
bool MySecurity::onSecurityRequest(){
	Serial.println("SecurityRequest");
	return true;
}

void MySecurity::onAuthenticationComplete(esp_ble_auth_cmpl_t cmpl){
	Serial.println("onAuthenticationComplete");
	if(cmpl.success){
		uint16_t length;
		esp_ble_gap_get_whitelist_size(&length);
		Serial.printf("size: %d\n", length);
	}
}



constexpr char bleCharacteristicCallback::SERVICE_MAIN[37];
constexpr char bleCharacteristicCallback::CHARACTERISTIC_PWM[37];
constexpr char bleCharacteristicCallback::CHARACTERISTIC_PROGRAM[37];
constexpr char bleCharacteristicCallback::CHARACTERISTIC_WIFI[37];
constexpr char bleCharacteristicCallback::CHARACTERISTIC_WIFI_PASS[37];

void bleCharacteristicCallback::onWrite(BLECharacteristic* pCharacteristic) {

	String uuid = pCharacteristic->getUUID().toString().c_str();

	// Front light colors, expecting a 3x byte array
	
	if( uuid == bleCharacteristicCallback::CHARACTERISTIC_PWM ){ 

		
		std::string value = pCharacteristic->getValue();
		//uint8_t val = value[0];
		/* uint16_t example
		for(uint8_t i = 0; i<value.size() && i<6; ++i){
			if(!(i%2))
				val[i/2] = value[i];
			else
				val[i/2] = val[i/2]|(value[i]<<8);
		}
		*/
		uint8_t i;
		for( i = 0; i<4 && i<value.size(); ++i ){

			apiClient.setFlatPWM(i, value[i]);
			Serial.printf("Motor %i = %i\n", i, value[i]);

		}

	}

	//Bicycle::onFrontLightColorsWrite(uint8_t value[3])
	//Serial.printf("BLE received on characteristic: %s\n", uuid);
}

void bleCharacteristicCallback::onRead(BLECharacteristic* pCharacteristic) {
	std::string msg = pCharacteristic->getValue();
	Serial.printf("BLE received: %s, %i\n", msg.c_str(), msg.length());
}



void VHBluetooth::run(void *data) {
			
	Serial.println("Starting BLE work!");
	BLEDevice::init("VibHub");
	BLEServer* pServer = BLEDevice::createServer();
	BLEDevice::setEncryptionLevel(ESP_BLE_SEC_ENCRYPT);
	BLEDevice::setSecurityCallbacks(new MySecurity());

	Serial.printf("Creating service %s\n", bleCharacteristicCallback::SERVICE_MAIN);
	BLEService* pService = pServer->createService(bleCharacteristicCallback::SERVICE_MAIN);

	
	Serial.println("Creating callback");
	bleCharacteristicCallback *callback = new bleCharacteristicCallback();

	uint8_t out[] = {0,0,0,0};

	// Front light characteristic
	Serial.printf("Creating first characteristic %s\n", bleCharacteristicCallback::CHARACTERISTIC_PWM);
	BLECharacteristic* pCharacteristic = pService->createCharacteristic(
		BLEUUID(bleCharacteristicCallback::CHARACTERISTIC_PWM),
		BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE
	);
	pCharacteristic->setAccessPermissions(ESP_GATT_PERM_READ_ENCRYPTED | ESP_GATT_PERM_WRITE_ENCRYPTED);
	pCharacteristic->setCallbacks(callback);
	pCharacteristic->setValue(out, 4);
	
	// Rear light characteristic
	/*
	Serial.printf("Creating second characteristic %s\n", bleCharacteristicCallback::CHARACTERISTIC_FLAGS);
	pCharacteristic = pService->createCharacteristic(
		BLEUUID(bleCharacteristicCallback::CHARACTERISTIC_FLAGS),
		BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE
	);
	pCharacteristic->setAccessPermissions(ESP_GATT_PERM_READ_ENCRYPTED | ESP_GATT_PERM_WRITE_ENCRYPTED);
	pCharacteristic->setCallbacks(callback);
	out[0] = bicycle->setting_flags;
	pCharacteristic->setValue(out, 1);

	// Sensor sensitivity threshold
	// CHARACTERISTIC_THRESHOLD
	Serial.printf("Creating third characteristic %s\n", bleCharacteristicCallback::CHARACTERISTIC_THRESHOLD);
	pCharacteristic = pService->createCharacteristic(
		BLEUUID(bleCharacteristicCallback::CHARACTERISTIC_THRESHOLD),
		BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE
	);
	pCharacteristic->setAccessPermissions(ESP_GATT_PERM_READ_ENCRYPTED | ESP_GATT_PERM_WRITE_ENCRYPTED);
	pCharacteristic->setCallbacks(callback);
	out[1] = bicycle->photocell_threshold&0xFF;
	out[0] = (bicycle->photocell_threshold>>8)&0xFF;
	pCharacteristic->setValue(out, 2);
	*/
	Serial.println("Starting");

	pService->start();

	BLEAdvertising* pAdvertising = pServer->getAdvertising();
	pAdvertising->addServiceUUID(BLEUUID(pService->getUUID()));

	BLESecurity *pSecurity = new BLESecurity();
	pSecurity->setAuthenticationMode(ESP_LE_AUTH_REQ_SC_BOND);
	pSecurity->setCapability(ESP_IO_CAP_OUT);
	pSecurity->setInitEncryptionKey(ESP_BLE_ENC_KEY_MASK | ESP_BLE_ID_KEY_MASK);

	pAdvertising->start();

	Serial.println("Advertising started!");
	delay(portMAX_DELAY);
}
