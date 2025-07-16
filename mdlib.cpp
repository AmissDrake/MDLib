#include <mdlib.h>


motor::motor(int PWMpin, int DIRpin, bool reverse) {
    this->PWMpin = PWMpin;
    this->DIRpin = DIRpin;
    this->reverse = reverse;

    pinMode(PWMpin, OUTPUT);
    pinMode(DIRpin, OUTPUT);
}

void motor::attachEncoder(int encoderPinA, int encoderPinB) {
    pinMode(encoderPinA, INPUT_PULLUP);
    pinMode(encoderPinB, INPUT_PULLUP);
    // Additional encoder setup can be added here

    feedback = true; // Set feedback flag to true
}

void motor::threshold(int max, int min) {
    // Set the speed limits
    if (max >= 0 && max <= 255) {
        maxSpeed = max;
    }
    if (min >= 0 && min <= 255) {
        minSpeed = min;
    }
}

void motor::spin(int speed, int direction) {
    // Ensure speed is within bounds
    speed = constrain(speed, this->minSpeed, maxSpeed);
    analogWrite(PWMpin, speed);
    
    // Set direction
    if (reverse == false) {
        digitalWrite(DIRpin, direction);
    } else {
        digitalWrite(DIRpin, !direction);
    }
}

void motor::setSpeed(int speed) {
    int dir = (speed >= 0) ? FORWARD : BACKWARD;
    speed = abs(speed);
    speed = constrain(speed, minSpeed, maxSpeed);
    analogWrite(PWMpin, speed);
    digitalWrite(DIRpin, (reverse) ? !dir : dir);
}

void motor::stop() {
    analogWrite(PWMpin, 0); // Stop the motor
    digitalWrite(DIRpin, LOW); // Set direction to LOW
}

void motor::ramptoSpeed(int speed, int rampTime) {
