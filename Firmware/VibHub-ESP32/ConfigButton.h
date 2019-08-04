/*
	VibHub Button
*/
#ifndef ConfigButton_h
#define ConfigButton_h
#include "Configuration.h"


class ConfigButton {
    public:
        ConfigButton(void);
        
        void setup();
        // void interrupt();
        bool loop( bool isReconfiguring=false );
        
        bool isPressed();
        
    private:
        
};

extern ConfigButton configButton;

#endif //ConfigButton_h
