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

    int 4wMatrix[4][3] = {
        {1, 1, 1}, // Forward
        {1, 1, 0}, // Right
        {1, 0, 1}, // Left
        {0, 1, 1}  // Backward
    };

public:
    drive(motor* m1, motor* m2, motor* m3, motor* m4);
    setWheelSpeed(int wheel1speed, int wheel2speed, int wheel3speed, int wheel4speed);
    drive(motor* m1, motor* m2, motor* m3);

    void setWheelCount(WheelCount count);
    WheelCount getWheelCount() const { return wheelCount; }
}

#endif // MDLIB_H