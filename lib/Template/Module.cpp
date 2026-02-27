#include "Arduino.h"
#include "Module.h"

Module::Module(/*other ports*/ int nSuccessPort) {
    // Other ports
    successPort = nSuccessPort;
}

bool Module::disarmed() {
    return isDisarmed;
}

void Module::disarm() {
    isDisarmed = true;
    digitalWrite(successPort, HIGH);
}

void Module::msetup() {
    pinMode(successPort, OUTPUT);
}

bool Module::mloop() {
    return false;
}

void Module::print_info() {
    
}