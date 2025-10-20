#include <Arduino.h>
#include <MDLib.h>

// Example for a single motor control
// This example demonstrates basic motor control using the MDLib library
// It sets up a motor and spins it in both directions with a delay

// Reverse the motor direction if needed

Motor m1(5, 6); // PWM, DIR

void setup() {
  m1.threshold(200, 50); // Set min and max speed
}

void loop() {
  m1.spin(150, CCW); // Counter-clockwise
  delay(1000);
  m1.spin(150, CW);  // Clockwise
  delay(1000);
}
