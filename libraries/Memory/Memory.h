#ifndef Memory_h
#define Memory_h
#include "Arduino.h"

class Memory {
    public:
        // Ports
        Memory(byte nSevSegGndPort, int nSevSegPort1, int nSevSegPort2, int nPort1, int nPort2, int nPort3, int nPort4, int nsuccessClock, int nFailPort);

        // Running
        void msetup();
        bool mloop();

        // Info
        bool disarmed();
        void print_info();

    private:
        // Ports
        int successClock;
        int failPort;
        int sevSegGndPort;
        byte sevSegPort[];
        int buttonPorts[4];

        // Local variables
        int dispNums[4] = {-1, -1, -1, -1};
        int flashNums[4] = {-1, -1, -1, -1}; 
        int pushNums[4] = {-1, -1, -1, -1};
        bool held[4] = {false, false, false, false};
        int stage;
        int ticks = 0;
        int t = 0;

        // Helper functions
        void sevseg_disp(int digit, int flashes);
        int find_correct_button();
        void led_step();
        void led_reset();

        // Disarmed state
        bool isDisarmed = false;
        void disarm();
};

#endif