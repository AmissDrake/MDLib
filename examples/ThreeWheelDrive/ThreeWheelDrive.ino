#include <Arduino.h>
#include <MDLib.h>

// Initialize three BTS7960 motor drivers with their respective 4 pins.
BTS7960 m1(2, 3, 4, 5);    // Front Left motor
BTS7960 m2(6, 7, 8, 9);    // Front Right motor
BTS7960 m3(10, 11, 12, 13); // Back motor

// It accepts any object derived from the base 'Motor' class.
Drive omni(&m1, &m2, &m3);

void setup() {
  Serial.begin(9600);
  Serial.println("3-Wheel Omni-Drive Example with BTS7960");

  // Set up the kinematics.
  omni.setWheelSpeedFactor(1.0, 1.0, 1.0);
}

void loop() {
  // Move forward (positive X direction) at a speed of 100
  Serial.println("Moving forward...");
  omni.move(100, 0, 0);
  delay(3000);

  // Strafe left (positive Y direction) at a speed of 100
  Serial.println("Strafing left...");
  omni.move(0, 100, 0);
  delay(3000);

  // Rotate counter-clockwise at a speed of 100
  Serial.println("Rotating CCW...");
  omni.move(0, 0, 100);
  delay(3000);

  // Stop all motors
  Serial.println("Stopping...");
  omni.stop();
  delay(3000);
}