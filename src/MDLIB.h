#ifndef MDLIB_H
#define MDLIB_H

#include <Arduino.h>

// Forwards is counter-clockwise
// Backwards is clockwise
// Left is Y axis
// Right is X axis
// Positive Rotation is from X to Y axis

enum Direction {
    CCW = 1, 
    CW = 0
};

class motor{
private:
    int PWMpin; // PWM pin
    int DIRpin1; // Direction pin
    int DIRpin2; // Second direction pin for three-pin motors
    int DIRpin; // Direction pin for two-pin motors
    bool reverse; // Direction flag
    bool feedback; // Feedback flag

    enum MD_TYPE {
        TWO_PIN_MD,
        THREE_PIN_MD,
    };

    MD_TYPE mdType; // Motor type

    // Motor Parameters
    int maxSpeed = 255; // Maximum speed
    int minSpeed = 0;   // Minimum speed

    int currentSpeed = 0; // Current speed of the motor

public:
    motor(int PWMpin, int DIRpin, bool reverse = false);
    motor(int PWMpin, int DIRpin1, int DIRpin2, bool reverse = false);
    void attachEncoder(int encoderPinA, int encoderPinB);
    void threshold(int max, int min);
    void spin(int speed, Direction direction);
    void setSpeed(int speed);
    void stop();
    //void ramptoSpeed(int speed, int rampTime = 1000);

    // Getters for motor parameters
    bool isReversed() const { return reverse; }
    int getPWMpin() const { return PWMpin; }
    int getDIRpin() const { return DIRpin; }
}

class drive{
private:
    enum WheelCount {
       THREE_WHEEL = 3,
       FOUR_WHEEL = 4
    };
    WheelCount wheelCount; // Number of wheels
    motor* m1;
    motor* m2;
    motor* m3;
    motor* m4;

    int wheel1speed = 1;
    int wheel2speed = 1;
    int wheel3speed = 1;
    int wheel4speed = 1;


public:
    // 4 wheel drive
    drive(motor* m1, motor* m2, motor* m3, motor* m4);
    void setWheelSpeed(int wheel1speed, int wheel2speed, int wheel3speed, int wheel4speed);
    drive(motor* m1, motor* m2, motor* m3);
    void setWheelSpeed(int wheel1speed, int wheel2speed, int wheel3speed);

    void move(int vx, int vy, int omega);
    void stop();

    WheelCount getWheelCount() const { return wheelCount; }
}

#endif // MDLIB_H