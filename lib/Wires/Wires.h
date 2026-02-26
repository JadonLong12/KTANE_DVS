#ifndef Wires_h
#define Wires_h
#include "Arduino.h"

class Wires {
  public:
    Wires(int port0, int port1, int port2, int port3, int nSuccessPort);
    void msetup();
    bool mloop();
    bool disarmed();
    void print_info();
    
  private:
    const int black = 0;
    const int yellow = 215; //259
    const int green = 259; //321
    const int purple = 329; //411
    const int red = 427; //510

    int wiresPorts[4];
    int correctWire;
    int successPort;
    bool isDisarmed = false;

    bool ceq(int c1, int c2);
    int ccount(int color, int c1, int c2, int c3, int c4);
    int find_correct_wire(int c1, int c2, int c3, int c4);
    void disarm();
};

#endif
