#include <Arduino.h>
#include <MDLib.h>

// Initialize four L298N motor driver objects.
// Each constructor takes three arguments: PWM pin, DIR1 pin, and DIR2 pin.
L298N m1(2, 3, 4);    // Front Left motor
L298N m2(5, 6, 7);    // Front Right motor
L298N m3(8, 9, 10);   // Back Left motor
L298N m4(11, 12, 13); // Back Right motor

// The Drive class initialization is identical, as it works with any 'Motor' type.
Drive bot(&m1, &m2, &m3, &m4);

void setup() {
  Serial.begin(9600);
  Serial.println("4-Wheel Omni-Drive Example with L298N");
  
  // Initialize the robot's kinematics matrix.
  bot.setWheelSpeedFactor(1.0, 1.0, 1.0, 1.0);
}

void loop() {
  // The library handles the specific implementation for the L298N drivers automatically.

  // Move forward (positive X direction)
  Serial.println("Moving forward...");
  bot.move(100, 0, 0);
  delay(3000);

  // Strafe left (positive Y direction)
  Serial.println("Strafing left...");
  bot.move(0, 100, 0);
  delay(3000);

  // Rotate counter-clockwise (positive omega)
  Serial.println("Rotating CCW...");
  bot.move(0, 0, 100);
  delay(3000);

  // Stop all motors
  Serial.println("Stopping...");
  bot.stop();
  delay(3000);
}