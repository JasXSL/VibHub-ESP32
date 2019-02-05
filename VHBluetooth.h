
/*
  VibHub motor data
*/
#ifndef Bluetooth_h
#define Bluetooth_h

#include "Arduino.h"
#include <vector>

#include "BLEDevice.h"
#include "BLEServer.h"
#include "BLEUtils.h"
#include "BLE2902.h"
#include "BLEScan.h"
#include "BLEAdvertisedDevice.h"
#include "sdkconfig.h"
#include <esp_log.h>
#include <string>
#include <Task.h>

#include "sdkconfig.h"


class MySecurity : public BLESecurityCallbacks {

	private:
		uint32_t onPassKeyRequest();
		void onPassKeyNotify(uint32_t pass_key);
		bool onConfirmPIN(uint32_t pass_key);
		bool onSecurityRequest();
		void onAuthenticationComplete(esp_ble_auth_cmpl_t cmpl);

};

class bleCharacteristicCallback: public BLECharacteristicCallbacks{
	public:
		static constexpr char SERVICE_MAIN[37] = "bedabb1e-d06e-babe-feed-faceb16b00b5";
		static constexpr char CHARACTERISTIC_PWM[37] = "bedabb1e-d06e-d00d-c0de-000000000000";
		static constexpr char CHARACTERISTIC_PROGRAM[37] = "bedabb1e-d06e-d00d-c0de-000000000001";
		static constexpr char CHARACTERISTIC_WIFI[37] = "ba5edca7-face-d00d-c0de-000000000000";
		static constexpr char CHARACTERISTIC_WIFI_PASS[37] = "ba5edca7-face-d00d-c0de-000000000001";
		

	private:
		void onWrite(BLECharacteristic* pCharacteristic);
		void onRead(BLECharacteristic* pCharacteristic);
};

class VHBluetooth: public Task{

	public:
		bool reset;

	private:
		void run(void *data);
};

extern BLESecurity *pSecurity;

#endif


