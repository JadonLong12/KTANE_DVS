#ifndef Simon_h
#define Simon_h
#include "Arduino.h"

class Simon {
    public:
        Simon(/*other ports*/ int nSuccessPort);
        void msetup();
        bool mloop();
        bool disarmed();

    private:
        int successPort;
        bool isDisarmed = false;
        void disarm();
};

#endif