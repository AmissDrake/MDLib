#include <Arduino.h>
#include <MDLib.h>

// Example for a 4-wheel drive robot
// This example demonstrates how to control a 4-wheel drive robot using the MDLib library
// It sets up four motors and moves the robot in different directions

Motor m1(5, 6);
Motor m2(9, 10);
Motor m3(3, 4);
Motor m4(7, 8);

Drive bot(&m1, &m2, &m3, &m4);

void setup() {
  bot.setWheelSpeed(1, 1, 1, 1);
}

void loop() {
  bot.move(100, 0, 0);  // Forward
  delay(3000);
  bot.move(0, 100, 0);  // Left
  delay(3000);
  bot.move(0, 0, 100);  // Rotate Anticlockwise
  delay(3000);
  bot.stop();
  delay(3000);
}
