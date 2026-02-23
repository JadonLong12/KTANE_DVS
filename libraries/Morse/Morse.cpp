#include "Arduino.h"
#include "Morse.h"

Morse::Morse(byte nPotentiometerPort, byte nLedPort, byte nButtonPort, byte a, byte b, byte c, byte d, byte e, byte f, byte g, byte nSuccessPort) {
    successPort = nSuccessPort;
    ledPort = nLedPort;
    potentiometerPort = nPotentiometerPort;
    buttonPort = nButtonPort;
    segmentPins[0] = a;
    segmentPins[1] = b;
    segmentPins[2] = c;
    segmentPins[3] = d;
    segmentPins[4] = e;
    segmentPins[5] = f;
    segmentPins[6] = g;
    segmentPins[7] = A7;
}

const unsigned long Morse::flashes[10][4] = {
    {0b11111100000001011111101000000010, 0b10000000111111010111111010000000, 0b11111101011111100000000000000000, 0b0},
    {0b10101010000000101111110000000101, 0b11111010100000001011111101010000, 0b00010101000000000000000000000000, 0b0},
    {0b10101000000010101010000000101111, 0b11000000010111111010100000001011, 0b11110101000000000000000000000000, 0b0},
    {0b11111101010100000001011111101000, 0b00001000000010111111000000011111, 0b10101111110000000000000000000000, 0b0},
    {0b11111100000001011111101000000010, 0b11111100000001111110101111110100, 0b00000111111010111111000000000000, 0b0},
    {0b10101000000010111111011111101000, 0b00001000000010111111000000011111, 0b10101111110000000000000000000000, 0b0},
    {0b10101000000010111111011111101000, 0b00001010000000101111110101000000, 0b01011111101010000000000000000000, 0b0},
    {0b11111101010100000001011111101000, 0b00001010000000111111010111111010, 0b00000011111101011111100000000000, 0b0},
    {0b11111100000001111110111111011111, 0b10000000111111011111100000001111, 0b11010101000000010101000000000000, 0b0},
    {0b10101010000000111111011111101111, 0b11000000011111101111110000000100, 0b00000101010000000000000000000000, 0b0},
};

const byte Morse::digits[10] = {
    0b11111100,
    0b01100000,
    0b11011010,
    0b11110010,
    0b01100110,
    0b10110110,
    0b10111110,
    0b11100000,
    0b11111110,
    0b11110110
};

const byte Morse::lengths[10] = {15, 8, 8, 10, 19, 10, 13, 20, 19, 10};

bool Morse::disarmed() {
    return isDisarmed;
}

void Morse::disarm() {
    isDisarmed = true;
    digitalWrite(successPort, HIGH);
}

void Morse::msetup() {
    pinMode(successPort, OUTPUT);
    pinMode(ledPort, OUTPUT);
    wordIndex = random(0, 10);
    for (int k=0; k<7; k++) {
        pinMode(segmentPins[k], OUTPUT);
    }
}

void Morse::runSevseg() {
    static byte s = 0;
    s++;
    s %= 7;
    digitalWrite(segmentPins[(s+6) % 7], LOW);
    digitalWrite(segmentPins[s], bitRead(digits[inVal], 7-s) ? HIGH : LOW);
}

const int dt = 150;
bool Morse::mloop() {
    if (millis() >= t + dt) {
        inVal = analogRead(potentiometerPort) / 103;
        t += dt;
        digitalWrite(ledPort, bitRead(flashes[wordIndex][j], 31-i) ? HIGH : LOW);
        i++;
        if ((i == lengths[wordIndex]) && (j == 3)) {
            digitalWrite(ledPort, LOW);
            i = 0;
            j = 0;
        }
        else if (i == 32) {
            i = 0;
            j++;
        }
    }

    if (digitalRead(buttonPort) == LOW) {
        if (held) {
            held = false;
            if (inVal == wordIndex) {
                disarm();
            }
            else {
                return true;
            }
        }
    }
    else {
        held = true;
    }

    runSevseg();
    return false;
}

void Morse::print_info() {
    Serial.print("Dial: ");
    Serial.println(analogRead(potentiometerPort) / 103);
    Serial.print("Correct: ");
    Serial.println(wordIndex);
    Serial.println();
}