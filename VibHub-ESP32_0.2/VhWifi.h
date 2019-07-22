/*
	VibHub wifi
*/
#ifndef VhWifi_h
#define VhWifi_h

#include "Configuration.h"

using namespace std::placeholders;
// Offload the constant CSS and JS to be injected into each page into program memory
// CSS Style shared on each page




class VhWifi{
	public:
		VhWifi(void) : 
			connected(false)
		{}
		void connect();
        void loop();
        
		bool connected;
        
	private:
        // WiFiManager* _wifiManager;
		// void handleFatalError();		// If something happened that can't be recovered from
		// Any non-constant data needed to be loaded should go in here
		String getCustomJSPre();		// Non constant data that should go above the constant data
		String getCustomJSPost();		// == || == below the constant data
		String getParam(String name);	// gets a custom param
        

};



extern VhWifi vhWifi;

#endif //VhWifi_h