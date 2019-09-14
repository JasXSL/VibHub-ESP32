/*
	VibHub config
*/
#ifndef UserSettings_h
#define UserSettings_h

class UserSettings{

	public:
		UserSettings(void);
        
        void load( bool reset=false );
        void save();
        void reset();
		
        
        char server[64];
        unsigned int port;
        char deviceid[64];
        int sleep_after_min;
        bool initialized;
        
        void loop();
        void resetSleepTimer();

	private:
        void gen_random( char *s, const int len );
        long last_action;               // millis() of last time there was an update
        
};

extern UserSettings userSettings;

#endif //UserSettings_h