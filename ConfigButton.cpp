/*
	VibHub Button
    
    Handles single button interface
*/

#include <functional>
#include "ConfigButton.h"
#include "StatusLED.h"
#include "Configuration.h"
#include "VhWifi.h"
#include "UserSettings.h"
#include "ApiClient.h"
#include "VHBluetooth.h"

unsigned long _heldTime;
int _lastButtonState;
bool _doReconfigure;
bool _isReconfiguring;
bool _buttonHeld;

unsigned long _pressedTime;
int _pressCount;

ConfigButton::ConfigButton( void ){

    _heldTime = 0;
    _lastButtonState = Configuration::BUTTON_UP;
    _doReconfigure = false;
    _isReconfiguring = false;
    _buttonHeld = false;

}


void ConfigButton::setup(){

    //set wifireset pin as input
    pinMode(Configuration::PIN_CONFIG_BUTTON, INPUT);

}

bool ConfigButton::isPressed(){

    return digitalRead(Configuration::PIN_CONFIG_BUTTON) == Configuration::BUTTON_DOWN;

}

bool ConfigButton::loop( bool isReconfiguring ){
    
    if( _doReconfigure ){

        _doReconfigure = false;
        Serial.println("ConfigButton: doReconfigure");
        
        // Disconnect from server
        apiClient.disconnect();
        
        // Start wifi portal
        vhWifi.connect(true, false);
        
        Serial.println("ConfigButton: Finished manual reconfig");
        // _isReconfiguring = false;
        
        return false;

    }
    
    int buttonState = digitalRead(Configuration::PIN_CONFIG_BUTTON);
    long delta = millis() - _heldTime;

    // Pressed
    if( buttonState == Configuration::BUTTON_DOWN && _lastButtonState == Configuration::BUTTON_UP ){
        _heldTime = millis();
        userSettings.resetSleepTimer();
    }

    // Start after holding for 4 seconds
    else if( buttonState == Configuration::BUTTON_DOWN && delta > Configuration::BUTTON_HOLD_TIME ){

        Serial.println("ConfigButton:  Button Held >4s");
        if( !isReconfiguring ){

            Serial.println("ConfigButton: Going in to manual reconfig");
            // Run reconfig next tick
            // Doing it here will cause crash
            _doReconfigure = true;
            // _isReconfiguring = true;

        }
        else{

            Serial.println("ConfigButton: Aborting manual reconfig to do reset");
            statusLED.setState(StatusLED::STATE_WIFI_ERR);
            
            Serial.println("ConfigButton: userSettings.reset");
            userSettings.reset();

            Serial.println("ConfigButton: vhWifi.clearSettings");
            vhWifi.clearSettings();
            
            //vhWifi will trigger the reboot
            return true;

        }

    }
    else if( userSettings.enable_bluetooth && buttonState == Configuration::BUTTON_DOWN && delta > Configuration::BUTTON_BT_MIN_TIME && delta < Configuration::BUTTON_BT_MIN_TIME+50 ){
        statusLED.quickFlashBluetooth();
    }

    // Released
    else if( buttonState == Configuration::BUTTON_UP && _lastButtonState == Configuration::BUTTON_DOWN ){

        _buttonHeld = false;
        if( userSettings.enable_bluetooth && delta > Configuration::BUTTON_BT_MIN_TIME && delta < Configuration::BUTTON_BT_MAX_TIME ){

            // Enable bluetooth pairing
            pSecurity->setCapability(ESP_IO_CAP_NONE);
            statusLED.setBluetoothPairable( true );
            Serial.println("Enabling bluetooth bonding");

        }
        else if( delta > Configuration::BUTTON_DEBOUNCE ){

            Serial.println("ConfigButton: Button Pressed");
            if( isReconfiguring ){

                // _isReconfiguring = false;
                Serial.println("ConfigButton: Stopping reconfig");                
                // Give feedback
                statusLED.setState(StatusLED::STATE_INIT);
                
                //vhWifi trigger the reboot
                return true;

            }
            else{

                if( _pressCount == 0 ){
                    
                    _pressedTime = millis();
                    ++_pressCount;

                }
                else if( delta < Configuration::BUTTON_HOLD_TIME ){

                    ++_pressCount;
                    Serial.print("ConfigButton: press count: ");
                    Serial.println(_pressCount);
                    if( _pressCount >= 5 ){

                        Serial.println("ConfigButton: Running LED Test");
                        _pressCount = 0;
                        statusLED.test();

                    }

                }
                else
                    _pressCount = 0;
                
            }
        }

    }
    // Give visual indication that button has been held long enough
    else if( !_buttonHeld && buttonState == Configuration::BUTTON_DOWN && _lastButtonState == Configuration::BUTTON_DOWN ){

        if( delta > Configuration::BUTTON_HOLD_TIME ){

            Serial.println("ConfigButton: Button is being held");
            statusLED.setState(StatusLED::STATE_INIT);
            _buttonHeld = true; // Fire only once

        }

    }
    
    _lastButtonState = buttonState;
    
    return false;

}


ConfigButton configButton = ConfigButton();
