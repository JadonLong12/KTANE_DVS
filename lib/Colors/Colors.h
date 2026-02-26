#ifndef Colors_h
#define Colors_h
#include "Arduino.h"

class Colors {
    public:
        // Ports
        Colors(int nButtonPort, int nRed, int nGreen, int nBlue, int nSuccessPort);

        // Running
        void msetup();
        bool mloop();

        // Info
        bool disarmed();
        void print_info();

        // Helper
        void flashgreen();

    private:
        // Ports
        byte buttonPort;
        byte successPort;
        byte red, green, blue;

        // Internal variables
        byte colors[6] = {0, 0, 0, 0, 0, 0};
        bool held = false;
        byte stage = 0;
        byte correct = -1;
        unsigned long offset;

        // Helper functions
        void randomize();
        void disp_colors();

        // Disarmed state
        bool isDisarmed = false;
        void disarm();
};

#endif