/*
	VibHub Firmware Update
*/

#ifndef FWUpdate_h
#define FWUpdate_h

#include <Arduino.h>


class FWUpdate {
    public:
        FWUpdate(void);
        ~FWUpdate();
        
        bool running;
        void start(const char* file, const char* md5);
        
        
    private:
        
};

extern FWUpdate fwUpdate;

#endif //FWUpdate_h
