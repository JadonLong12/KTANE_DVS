#include "Arduino.h"
#include "Memory.h"

Memory::Memory(int nSevSegGndPort, int nSevSegPort1, int nSevSegPort2, int nPort1, int nPort2, int nPort3, int nPort4, int nsuccessClock, int nFailPort) {
    sevSegGndPort = nSevSegGndPort;
    sevSegPort[0] = nSevSegPort1;
    sevSegPort[1] = nSevSegPort2;
    successClock = nsuccessClock;
    failPort = nFailPort;
    buttonPorts[0] = nPort1;
    buttonPorts[1] = nPort2;
    buttonPorts[2] = nPort3;
    buttonPorts[3] = nPort4;
}

bool Memory::disarmed() {
    return isDisarmed;
}

void Memory::disarm() {
    digitalWrite(sevSegGndPort, HIGH);
    isDisarmed = true;
}

void Memory::sevseg_disp(int digit, int flashes) {
    t = (t+1) % 4;
    if (t == 0) {
        if ((ticks % 2 == 0) && ticks - 2 < flashes * 2) {
            if (digit == 0) {
                digitalWrite(sevSegPort[0], LOW);
                digitalWrite(sevSegPort[1], LOW);
            }
            else if (digit == 1) {
                digitalWrite(sevSegPort[0], HIGH);
                digitalWrite(sevSegPort[1], HIGH);
            }
            else if (digit == 2) {
                digitalWrite(sevSegPort[0], HIGH);
                digitalWrite(sevSegPort[1], LOW);
            }
            else {
                digitalWrite(sevSegPort[0], LOW);
                digitalWrite(sevSegPort[1], HIGH);
            }
            digitalWrite(sevSegGndPort, LOW);
        }
        else {
            digitalWrite(sevSegGndPort, HIGH);
        }

        ticks = (ticks+1) % (flashes*2 + 6);

        if (ticks == 0) {print_info();}
    }
}

int Memory::find_correct_button() {
    int disp = dispNums[stage];
    if (stage == 0) {
        if (disp == 0) {return 1;}
        else if (disp == 1) {return 3;}
        else if (disp == 2) {return 0;}
        else {return 2;}
    }
    else if (stage == 1) {
        if (disp == 0) {return flashNums[1];}
        else if (disp == 1) {return dispNums[0];}
        else if (disp == 2) {return pushNums[0];}
        else {return flashNums[0];}
    }
    else if (stage == 2) {
        if (disp == 0) {return flashNums[1];}
        else if (disp == 1) {return pushNums[1];}
        else if (disp == 2) {return dispNums[1];}
        else {return dispNums[0];}
    }
    else if (stage == 3) {
        if (disp == 0) {return flashNums[pushNums[0]];}
        else if (disp == 1) {return dispNums[flashNums[1]];}
        else if (disp == 2) {return dispNums[dispNums[1]];}
        else {return flashNums[pushNums[2]];}
    }
}

void Memory::msetup() {
    pinMode(sevSegPort[0], OUTPUT);
    pinMode(sevSegPort[1], OUTPUT);
    pinMode(successClock, OUTPUT);
    pinMode(failPort, OUTPUT);
    pinMode(sevSegGndPort, OUTPUT);

    digitalWrite(sevSegGndPort, LOW);
    led_reset();

    stage = 0;
    dispNums[0] = random(0, 4);
    flashNums[0] = random(0, 4);
}

void Memory::led_step() {
    digitalWrite(successClock, LOW);
    digitalWrite(successClock, HIGH);
}

void Memory::led_reset() {
    digitalWrite(failPort, LOW);
    digitalWrite(failPort, HIGH);
    led_step();
}

bool Memory::mloop() {
    if (!isDisarmed) {
        sevseg_disp(dispNums[stage], flashNums[stage]);

        for (int i=0; i<4; i++) {
            if (digitalRead(buttonPorts[i]) == HIGH) {
                held[i] = true;
            }
            else {
                if (held[i]) {
                    held[i] = false;
                    if (i == find_correct_button()) {
                        Serial.println("CORRECT");
                        led_step();
                        if (stage == 3) {
                            disarm();
                        }
                        else {
                            pushNums[stage] = i;
                            stage++;
                            dispNums[stage] = random(0, 4);
                            flashNums[stage] = random(0, 4);
                        }
                    ticks = flashNums[stage] * 2 + 2;
                    } 
                    else {
                        Serial.println("INCORRECT");
                        stage = 0;
                        led_reset();
                        for (int i=0; i<4; i++) {
                            dispNums[i] = -1;
                            flashNums[i] = -1;
                            pushNums[i] = -1;
                        }
                        dispNums[0] = random(0, 4);
                        flashNums[0] = random(0, 4);
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

void Memory::print_info() {
    Serial.print("Display numbers: ");
    for (int i=0; i<4; i++) {
        Serial.print(dispNums[i] + 1);
        Serial.print(", ");
    }
    Serial.println();

    Serial.print("Flash numbers: ");
    for (int i=0; i<4; i++) {
        Serial.print(flashNums[i] + 1);
        Serial.print(", ");
    }
    Serial.println();

    Serial.print("Previous buttons: ");
    for (int i=0; i<4; i++) {
        Serial.print(pushNums[i] + 1);
        Serial.print(", ");
    }
    Serial.println();

    Serial.print("Correct button: ");
    Serial.println(find_correct_button() + 1);
    Serial.println("");
}