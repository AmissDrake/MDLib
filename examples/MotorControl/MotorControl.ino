#include <Arduino.h>
#include <MDLib.h>

// MotorTwopin is a generic class for any 2-pin (PWM, DIR) driver.
MD20C m1(5, 6);      // Motor 1: PWM=5, DIR=6
MD20C m2(9, 10);     // Motor 2: PWM=9, DIR=10

void setup() {
  // Initialize Serial Monitor for feedback.
  Serial.begin(9600);
  Serial.println("Two Motor Control Example");
}

void loop() {
  // Move motors forward (in opposite directions for a typical chassis)
  Serial.println("Moving forward...");
  m1.setSpeed(200);  // Set speed to 200 (CCW)
  m2.setSpeed(-200); // Set speed to 200 (CW)
  delay(1000);       // Wait for 1 second

  // Stop both motors
  Serial.println("Stopping...");
  m1.stop();
  m2.stop();
  delay(1000);       // Wait for 1 second before repeating
}