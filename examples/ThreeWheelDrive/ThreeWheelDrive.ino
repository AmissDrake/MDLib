#include <Arduino.h>
#include <MDLib.h>

motor m1(3, 4); // FL
motor m2(5, 6); // FR
motor m3(9, 10); // Back

drive omni(&m1, &m2, &m3);

void setup() {
  omni.setWheelSpeed(1, 1, 1);
}

void loop() {
  omni.move(100, 0, 0); // Move forward
  delay(3000);
  omni.move(0, 100, 0); // Move left
  delay(3000);
  omni.move(0, 0, 100); // Rotate counter-clockwise
  delay(3000);
  omni.stop();
  delay(3000);
}
