#include "SevSeg.h"
#define STRIKE_HZ 2000
#define TIMER_HZ 1750

SevSeg sevseg; //Instantiate a seven segment controller object
bool active = true;

void setup() {
  Serial.begin(9600);

  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  
  byte numDigits = 4;
  byte digitPins[] = {5, 4, 3, 2};
  byte segmentPins[] = {6, 7, 8, 9, 10, 11, 12, 13};
  bool resistorsOnSegments = true; // 'false' means resistors are on digit pins
  byte hardwareConfig = COMMON_CATHODE; // See README.md for options
  bool updateWithDelays = false; // Default 'false' is Recommended
  bool leadingZeros = false; // Use 'true' if you'd like to keep the leading zeros
  bool disableDecPoint = false; // Use 'true' if your decimal point doesn't exist or isn't connected
  
  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments,
  updateWithDelays, leadingZeros, disableDecPoint);
  sevseg.setBrightness(90);

  for (int i=22; i <= 53; i++) {  
    pinMode(i, INPUT_PULLUP);
    Serial.println(i);
  }

  delay(1000);
}

byte strikeTimer = 0;

void explode() {
  tone(A3, 2000, 1000);
  active = false;
}

void strike() {
  static byte strikes = 0;
  if (strikes == 0) {
    digitalWrite(A0, HIGH);
    tone(A3, STRIKE_HZ, 100);
    strikes++;
  }
  else if (strikes == 1) {
    digitalWrite(A1, HIGH);
    tone(A3, STRIKE_HZ, 100);
    strikes++;
  }
  else {
    digitalWrite(A2, HIGH);
    tone(A3, STRIKE_HZ, 100);
    explode();
  }
}

void defuse() {
  active = false;
  tone(A3, 1250, 200);
  delay(200);
  tone(A3, 1562, 200);
  delay(200);
  tone(A3, 1875, 200);
  delay(200);
  tone(A3, 2500, 600);
}

void module_loop() {
  bool defused = true;
  for (int i=22; i <= 52; i+=2) {
    if (digitalRead(i) == LOW) {
      defused = false;
    }
    if (digitalRead(i+1) == LOW) {
      if (strikeTimer == 0) {
        strike();
        strikeTimer = 10;
      }
    }
  }
  if (defused) {
    defuse();
    Serial.println("Defused!");
  }
  if (strikeTimer > 0) {
    strikeTimer--;
    Serial.println(strikeTimer);
  }
}

byte dt = 10;

void loop() {
  static unsigned long timer = millis();
  const unsigned long secs = 5 * 60;
  static unsigned long centisecs =  secs * 100 + 100;
  
  if (active) {
    if (centisecs > 0) {
      if (millis() - timer >= dt) {
        module_loop();
        timer += dt;
        centisecs--;

        if (centisecs >= 6000) {
          sevseg.setNumber((centisecs / 60 / 100 * 100) + (centisecs / 100) % 60, 2);
        }
        else {
          sevseg.setNumber(centisecs, 2);
        }
        

        if (strikeTimer == 0){

          if (centisecs % 5 == 0) {
            module_loop();
            int cmod = centisecs % 100;
            if (centisecs >= 6000) {
              if (cmod == 0) {
                tone(A3, TIMER_HZ, dt * 5);
              }
            }
            else if (centisecs >= 1000) {
              if ((cmod == 0) || (cmod == 90)) {
                tone(A3, TIMER_HZ, dt * 3);
              }
            }
            else{
              if ((cmod == 0) || (cmod == 90) || (cmod == 75) || (cmod == 65) || (cmod == 50) || (cmod == 40)) {
                tone(A3, TIMER_HZ, dt * 3);
              }
            }
          }
        }
      }
    }
    else {
      explode();
    }
  }

  sevseg.refreshDisplay(); // Must run repeatedly
}
