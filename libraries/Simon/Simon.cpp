#include "Arduino.h"
#include "Simon.h"

Simon::Simon(/*other ports*/ int nSuccessPort) {
    successPort = nSuccessPort;
}

bool Simon::disarmed() {
    return isDisarmed;
}

void Simon::disarm() {
    isDisarmed = true;
    digitalWrite(successPort, HIGH);
}

void Simon::msetup() {
    pinMode(successPort, OUTPUT);
}

bool Simon::mloop() {
    return false;
}

