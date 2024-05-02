#include <Arduino.h>

// put function declarations here:
int myFunction(int, int);

void setup() {
  // put your setup code here, to run once:
}

void loop() {
  delay(5000);
  pinMode(7, HIGH);
  delay(5000);
  pinMode(7, LOW);

  // pinMode(7,HIGH);

  // put your main code here, to run repeatedly:
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}