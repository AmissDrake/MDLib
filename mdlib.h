#ifndef MDLIB_H
#define MDLIB_H

#include <arduino.h>

#define FORWARD 1
// Forwards is clockwise
#define BACKWARD 0
// Backwards is counter-clockwise

class motor{
private:
    int PWMpin; // PWM pin
    int DIRpin; // Direction pin
    bool reverse; // Direction flag
    bool feedback; // Feedback flag

    // Motor Parameters
    int maxSpeed = 255; // Maximum speed
    int minSpeed = 0;   // Minimum speed

    int currentSpeed = 0; // Current speed of the motor

public:
    motor(int PWMpin, int DIRpin, bool reverse = false);
    void attachEncoder(int encoderPinA, int encoderPinB);
    void threshold(int max, int min);
    void spin(int speed, int direction);
    void setSpeed(int speed);
    void stop();
    void ramptoSpeed(int speed, int rampTime = 1000);

    // Getters for motor parameters
    bool isReversed() const { return reverse; }
    bool PWMpin() const { return PWMpin; }
    bool DIRpin() const { return DIRpin; }
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

    int 4wMatrix[4][3] = {
        {wheel1speed, wheel1speed, wheel1speed}, // Forward
        {wheel2speed, wheel2speed, wheel2speed}, // Right
        {wheel3speed, wheel3speed, wheel3speed}, // Left
        {wheel4speed, wheel4speed, wheel4speed}  // Backward
    };

    int 3wMatrix[5][3] =  {
        {wheel1speed, wheel1speed, wheel1speed}, // Forward
        {wheel2speed, wheel2speed, wheel2speed}, // Right
        {wheel3speed, wheel3speed, wheel3speed}, // Left
    };

public:
    // 4 wheel drive
    drive(motor* m1, motor* m2, motor* m3, motor* m4);
    setWheelSpeed(int wheel1speed, int wheel2speed, int wheel3speed, int wheel4speed);
    drive(motor* m1, motor* m2, motor* m3);

    void setWheelCount(WheelCount count);
    WheelCount getWheelCount() const { return wheelCount; }
}

#endif // MDLIB_H