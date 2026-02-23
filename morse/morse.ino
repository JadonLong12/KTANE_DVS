#include <Morse.h> 

Morse morse(A0, 9, 10, 2, 3, 4, 5, 6, 7, 8, 12);

void set_random_seed() {
  int randsum = 0;
  for (int i=0; i<8; i++){
    randsum += analogRead(i);
  }
  randomSeed(randsum);
}

void setup() {
  Serial.begin(9600);
  pinMode(11, OUTPUT);
  set_random_seed();
  morse.msetup();
}


unsigned long t = 0;
void loop() {
  if (morse.mloop()) {
    digitalWrite(11, LOW);
    delay(50);
  }
  digitalWrite(11, HIGH);

  if (millis() >= t + 500) {
    t += 500;

    morse.print_info();
    Serial.println();
  }
}
