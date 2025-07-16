#include <MDLib.h>
#include <Arduino.h>

motor m1(5, 6, false);
motor m2(9, 10, false);

void setup() {
  m1.setSpeed(200);
  m2.setSpeed(-200);
}

void loop() {
  // Move forward for 1 second
  delay(1000);
  m1.stop();
  m2.stop();
  delay(1000);
}
