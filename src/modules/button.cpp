#include <Button.h> 

Button button1(4, 2, 3, 12);

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
  button1.msetup();
}


unsigned long t = 0;
unsigned long striketimer = 0;
byte dig = 0;
bool strike = false;
void loop() {
  if (button1.mloop()) {
    digitalWrite(11, LOW);
    delay(50);
  }
  digitalWrite(11, HIGH);

  if (millis() >= t + 1000) {
    t += 1000;
    if (dig == 0) {dig = 9;}
    else {dig -= 1;}

    Serial.println();
    button1.print_info();
    Serial.println(millis());
    Serial.println();
  }
  delay(100);
}
