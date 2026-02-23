#ifndef Module_h
#define Module_h
#include "Arduino.h"

class Module {
    public:
        // Ports
        Module(/*other ports*/ int nSuccessPort);

        // Running
        void msetup();
        bool mloop();

        // Info
        bool disarmed();
        void print_info();

    private:
        // Ports
        int successPort;

        // Disarmed state
        bool isDisarmed = false;
        void disarm();
};

#endif