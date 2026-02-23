#ifndef WhosOnFirst_h
#define WhosOnFirst_h
#include "Arduino.h"
#include "LiquidCrystal.h"

class WhosOnFirst {
    public:
        // Ports
        WhosOnFirst(byte rs, byte en, byte d4, byte d5, byte d6, byte d7, byte b1, byte b2, byte b3, byte b4);

        // Running
        void msetup();
        bool mloop();

        // Info
        bool disarmed();
        void print_info();

    private:
        // Ports
        byte buttonPort[4];

        // Disarmed state
        bool isDisarmed = false;
        void disarm();

        // Internal variables
        LiquidCrystal lcd;
        byte blinkPos[2];
        byte dispWords[4];
        byte stage = 0;
        byte intermissionClock = 0;
        bool held[4] = {false, false, false, false};

        static const byte wordLists[35][10];
        static byte leftIdx[35];
        static byte topIdx[35];
        static const String words[70];

        // Helper functions
        byte find_correct_button();
        void randomize();
        bool in(byte listIdx[35], byte val);
        bool identical_disp_words();
        void display_words();
    };

#endif