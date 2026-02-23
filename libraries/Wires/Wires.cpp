#include "Arduino.h"
#include "Wires.h"

Wires::Wires(int port0, int port1, int port2, int port3, int nSuccessPort) {
  wiresPorts[0] = port0;
  wiresPorts[1] = port1;
  wiresPorts[2] = port2;
  wiresPorts[3] = port3;
  successPort = nSuccessPort;
}

void Wires::msetup() {
  correctWire = find_correct_wire(analogRead(wiresPorts[0]), analogRead(wiresPorts[1]), analogRead(wiresPorts[2]), analogRead(wiresPorts[3]));
  pinMode(successPort, OUTPUT);
}

bool Wires::mloop() {
  for (int i=0; i<4; i++){
    if (wiresPorts[i] != -1){
      if ((analogRead(wiresPorts[i]) >= 800) || (analogRead(wiresPorts[i]) == 255)) {
        wiresPorts[i] = -1;
        if (i == correctWire) {disarm();}
        else {return true;}
      }
    }
  }
  return false;
}

bool Wires::disarmed() {
  return isDisarmed;
}

bool Wires::ceq(int c1, int c2) {
  return abs(c1 - c2) < 30;
}

int Wires::ccount(int color, int c1, int c2, int c3, int c4) {
  return ceq(color, c1) + ceq(color, c2) + ceq(color, c3) + ceq(color, c4);
}

int Wires::find_correct_wire(int c1, int c2, int c3, int c4) {
  if (ceq(c2, black)) {
    if (ceq(c1, c3)) {return 2;}
    else if (ccount(red, c1,c2,c3,c4) == 0) {return 1;}
    else if (ceq(c4, purple) || ceq(c4, green)) {return 0;}
    else if (ccount(black, c1,c2,c3,c4) % 2 != 0) {return 3;}
    else {return 1;}
  }
  else if (ceq(c2, yellow) || ceq(c2, purple)) {
    if (ccount(green, c1,c2,c3,c4) > ccount(purple, c1,c2,c3,c4)) {return 0;}
    else if (ccount(black, c1,c2,c3,c4) == 1) {
      if (ceq(c1, black)) {return 0;}
      else if (ceq(c2, black)) {return 1;}
      else if (ceq(c3, black)) {return 2;}
      else {return 3;}
    }
    else if (ceq(c2, yellow) && !(ceq(c3, black))) {return 3;}
    else if (!ceq(c1, c4)) {return 1;}
    else {return 2;}
  }
  else {
    if ((ccount(red, c1,c2,c3,c4) > 0) && (ccount(black, c1,c2,c3,c4) > 0)) {return 3;}
    else if (ccount(green, c1,c2,c3,c4) >= 2) {
      if (ceq(c4, green)) {return 3;}
      else if (ceq(c3, green)) {return 2;}
      else {return 1;}
    }
    else if (ccount(red, c1,c2,c3,c4) > 1) {return 2;}
    else if (ccount(yellow, c1,c2,c3,c4) % 2 == 0) {return 3;}
    else {return 0;}
  }
}

void Wires::disarm() {
  isDisarmed = true;
  digitalWrite(successPort, HIGH);
}

void Wires::print_info() {
  for (int i=0; i<4; i++) {
    int port = wiresPorts[i];
    Serial.print(analogRead(port));
    Serial.print(": ");
    if (ceq(black, analogRead(port))) {Serial.print("Black");}
    else if (ceq(yellow, analogRead(port))) {Serial.print("Yellow");}
    else if (ceq(green, analogRead(port))) {Serial.print("Green");}
    else if (ceq(purple, analogRead(port))) {Serial.print("Purple");}
    else if (ceq(red, analogRead(port))) {Serial.print("Red");}
    else {Serial.print("ERROR");}
    Serial.println();
  }
  Serial.print("Correct wire:" );
  Serial.println(correctWire + 1);
  Serial.print("Disarmed: ");
  Serial.println(isDisarmed);
  Serial.println();
}