#include "Arduino.h"
#include "Colors.h"

#define RED    1
#define GREEN  2
#define YELLOW 3
#define BLUE   4
#define PURPLE 5
#define OFF    6


Colors::Colors(int nButtonPort, int nRed, int nGreen, int nBlue, int nSuccessPort) {
    buttonPort = nButtonPort;
    red = nRed;
    green = nGreen;
    blue = nBlue;
    successPort = nSuccessPort;
}

bool Colors::disarmed() {
    return isDisarmed;
}

void Colors::disarm() {
    isDisarmed = true;
    digitalWrite(green, HIGH);
    digitalWrite(red, LOW);
    digitalWrite(blue, LOW);
    digitalWrite(successPort, HIGH);
}


void Colors::randomize() {
    bool inclColors[6] = {false, false, false, false, false, false}; // red green yellow blue purple off
    byte length;
    bool badRandom = true;
    while (badRandom) {
        for (byte i=0; i<6; i++) {
            length = 0;
            if (random(0, 2) == 0) {
                inclColors[i] = true;
                length++;
                if (i != 1) {badRandom = false;}
            }
        }
    }

    if (!inclColors[4] && (!inclColors[0] &&  !inclColors[1] && !inclColors[2] && inclColors[3])) {
        inclColors[4] = true;
        length++;
    }
    else if (!inclColors[5] && (( inclColors[0] && !inclColors[1] && !inclColors[2] && !inclColors[3])
                             || (!inclColors[0] &&  inclColors[1] && !inclColors[2] &&  inclColors[3]))) {
        inclColors[5] = true;
        length++;
    }

    if (length < 2) {
        inclColors[4] = true;
        inclColors[5] = true;
    }

    for (byte i=0; i<6; i++) {
        Serial.print(inclColors[i]);
        if (inclColors[i]) {
            colors[i] = i+1;
        }
        else {
            do {
                colors[i] = random(1, 7);
            } while (!inclColors[colors[i]-1]);
        }
    }
    Serial.println();

    for (byte i=0; i<6; i++) {
        byte j = random(0, 6);
        byte temp = colors[i];
        colors[i] = colors[j];
        colors[j] = temp;
    }


    if (inclColors[0]) {
        if (inclColors[1]) {
            if (inclColors[2]) {
                if (inclColors[3]) {
                    correct = BLUE;
                }
                else {
                    correct = GREEN;
                }
            }
            else {
                if (inclColors[3]) {
                    correct = GREEN;
                }
                else {
                    correct = RED;
                }
            }
        }
        else {
            if (inclColors[2]) {
                if (inclColors[3]) {
                    correct = YELLOW;
                }
                else {
                    correct = RED;
                }
            }
            else {
                if (inclColors[3]) {
                    correct = RED;
                }
                else {
                    correct = OFF;
                }
            }
        }
    }
    else {
        if (inclColors[1]) {
            if (inclColors[2]) {
                if (inclColors[3]) {
                    correct = YELLOW;
                }
                else {
                    correct = OFF;
                }
            }
            else {
                if (inclColors[3]) {
                    correct = BLUE;
                }
                else {
                    correct = GREEN;
                }
            }
        }
        else {
            if (inclColors[2]) {
                if (inclColors[3]) {
                    correct = BLUE;
                }
                else {
                    correct = YELLOW;
                }
            }
            else {
                correct = PURPLE;
            }
        }
    }
}

void Colors::disp_colors() {
    if (millis() % 1000 > 900) {
        digitalWrite(red, LOW);
        digitalWrite(green, LOW);
        digitalWrite(blue, LOW);
    }
    else {
        byte color = (colors[(millis() % 6000) / 1000]) % 6;
        digitalWrite(red, color % 2);
        digitalWrite(green, (color == 2) || (color == 3));
        digitalWrite(blue, color > 3);
    }
}

void Colors::msetup() {
    pinMode(red, OUTPUT);
    pinMode(green, OUTPUT);
    pinMode(blue, OUTPUT);
    pinMode(successPort, OUTPUT);
    randomize();
    offset = millis();
}

void Colors::flashgreen() {
    digitalWrite(red, LOW);
    digitalWrite(blue, LOW);
    digitalWrite(green, LOW);
    delay(3000);
    randomize();
    offset = millis();
}

bool Colors::mloop() {
    if (!isDisarmed) {
        disp_colors();
        if (digitalRead(buttonPort) == HIGH) {
            if (!held) {
                held = true;
                if (correct == colors[((millis() - offset) % 6000) / 1000]) {
                    /*stage++;
                    if (stage == 3) {
                        disarm();
                    }
                    else {
                        flashgreen();
                    }*/
                    disarm();
                }
                else {
                    return true;
                }
            }
        }
        else {
            held = false;
        }
    }
    return false;
}

void Colors::print_info() {
    const String words[6] = {"Red", "Green", "Yellow", "Blue", "Purple", "Off"};
    Serial.print("Colors: ");
    for (int i=0; i<6; i++) {
        Serial.print(words[colors[i] - 1]);
        Serial.print(", ");
    }
    Serial.println();
    Serial.print("Correct Color: ");
    Serial.println(words[correct-1]);
    Serial.print("Button State: ");
    Serial.println(digitalRead(buttonPort));
    Serial.println();
}