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
        bool enable_bluetooth;
        
	private:
        void gen_random( char *s, const int len );
        
};

extern UserSettings userSettings;

#endif //UserSettings_h