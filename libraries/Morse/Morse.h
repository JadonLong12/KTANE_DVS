#ifndef Morse_h
#define Morse_h
#include "Arduino.h"

class Morse {
    public:
        Morse(byte nPotentiometerPort, byte nLedPort, byte nButtonPort, byte a, byte b, byte c, byte d, byte e, byte f, byte g, byte nSuccessPort);
        void msetup();
        bool mloop();
        void print_info();
        bool disarmed();

    private:
        // Ports
        byte ledPort;
        byte potentiometerPort;
        byte buttonPort;
        byte successPort;
        byte segmentPins[8];
        // Disarm Data
        bool isDisarmed = false;
        void disarm();
        void runSevseg();
        // Module Data
        int wordIndex;
        unsigned long t = 0;
        int i = 0;
        int j = 0;
        bool held = false;
        byte inVal = 0;

        static const unsigned long flashes[10][4];
        static const byte digits[10];
        static const byte lengths[10];
};

#endif