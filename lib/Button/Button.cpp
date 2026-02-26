#include "Arduino.h"
#include "Button.h"

Button::Button(int nButtonPort, int nLedPortL, int nLedPortR, int nSuccessPort) {
    buttonPort = nButtonPort;
    ledPortL = nLedPortL;
    ledPortR = nLedPortR;
    successPort = nSuccessPort;
    downTime = 0;
}

bool Button::disarmed() {
    return isDisarmed;
}

void Button::disarm() {
    isDisarmed = true;
    digitalWrite(successPort, HIGH);
}

int Button::shuffle(int n) {
    if (n == 0) {return 8;}
    else if (n == 1) {return 4;}
    else if (n == 2) {return 7;}
    else if (n == 3) {return 0;}
    else if (n == 4) {return 5;}
    else if (n == 5) {return 9;}
    else if (n == 6) {return 1;}
    else if (n == 7) {return 3;}
    else if (n == 8) {return 6;}
    else {return 2;}
}

void Button::msetup() {
    pinMode(ledPortL, OUTPUT);
    pinMode(ledPortR, OUTPUT);
    pinMode(successPort, OUTPUT);

    left[0] = random(0, 3);
    left[1] = random(0, 3);
    right[0] = random(0, 2);
    right[1] = random(0, 2);

    int orig = left[0] + 3*left[1] + 1;
    if (right[0]) {
        if (right[1]) {
            correctDigit = shuffle(shuffle(shuffle(shuffle(orig))));
        }
        else {
            correctDigit = shuffle(shuffle(shuffle(orig)));
        }
    }
    else {
        if (right[1]) {
            correctDigit = shuffle(shuffle(orig));
        }
        else {
            correctDigit = shuffle(orig);
        }
    }
}

bool Button::mloop() {
    if (digitalRead(buttonPort) == LOW) {
        digitalWrite(ledPortR, right[0] ? HIGH : LOW);
        digitalWrite(ledPortL, ((left[0] == 0) || ((left[0] == 1) && (millis()/500 % 2 == 0))) ? HIGH : LOW);
        if (held) {
            held = false;
            if ((abs((millis() - downTime) - (10 - correctDigit) * 1000) % 10000) < 1000) {
                disarm();
            }
            else {
                return true;
            }
        }
    }
    else {
        digitalWrite(ledPortR, right[1] ? HIGH : LOW);
        digitalWrite(ledPortL, ((left[1] == 0) || ((left[1] == 1) && (millis()/500 % 2 == 0))) ? HIGH : LOW);
        if (!held) {
            downTime = millis();
            held = true;
        }
    }

    return false;
}

void Button::print_info() {
    Serial.print("Blue LED: ");
    Serial.print((left[0] == 0) ? "On" : ((left[0] == 1) ? "Blink" : "Off"));
    Serial.print(" -> ");
    Serial.println((left[1] == 0) ? "On" : ((left[1] == 1) ? "Blink" : "Off"));

    Serial.print("Yellow LED: ");
    Serial.print((right[0]) ? "On" : "Off");
    Serial.print(" -> ");
    Serial.println((right[1]) ? "On" : "Off");

    Serial.print("Correct digit: ");
    Serial.println(correctDigit);

    if (held) {
        Serial.print("Button held for: ");
        Serial.println(millis() - downTime);
    }
}
