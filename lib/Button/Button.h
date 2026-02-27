#ifndef Button_h
#define Button_h
#include "Arduino.h"

class Button {
    public:
        // Ports
        // L = yellow, R = blue
        Button(int nButtonPort, int nLedPortL, int nLedPortR, int nSuccessPort);

        // Running
        void msetup();
        bool mloop();

        // Info
        bool disarmed();
        void print_info();

    private:
        // Ports
        int buttonPort;
        int successPort;

        // Local variables
        int ledPortL;
        int ledPortR;
        byte left[2];
        bool right[2];
        int correctDigit;
        unsigned long downTime;
        bool held = false;

        // Internal functions
        int shuffle(int n);

        // Disarmed state
        bool isDisarmed = false;
        void disarm();
};

#endif