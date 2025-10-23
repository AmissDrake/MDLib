#ifndef MDLib_H
#define MDLib_H

#include <Arduino.h>

// Forwards is counter-clockwise
// Backwards is clockwise
// Left is Y axis
// Front is X axis
// Positive Rotation is from X to Y axis


// Types of motor drivers supported:
// MD10C - 2 pin, PWM + DIR
// SmartElex - 2 pin, PWM + DIR
// MD20C - 2 pin PWM + DIR
// L298N - 3 pin, PWM + DIR1 + DIR2
// BTS7960 - 4 pin, L_PWM + R_PWM + L_EN + R_EN

enum Direction {
    CCW = 1,
    CW = 0
};

// Base Motor Class
class Motor{
protected:
    bool reverse; // Direction flag
    bool feedback; // Feedback flag

    // Motor Parameters
    int maxSpeed = 255; // Maximum speed
    int minSpeed = 0;   // Minimum speed

    int currentSpeed = 0; // Current speed of the motor

    int ENCpinA;
    int ENCpinB;

public:
    Motor(bool reverse = false) : reverse(reverse), feedback(false) {};


    void attachEncoder(int encoderPinA, int encoderPinB);
    void threshold(int max, int min);

    virtual void spin(int speed, Direction direction) = 0;
    virtual void setSpeed(int speed) = 0;
    virtual void stop() = 0;
    //void ramptoSpeed(int speed, int rampTime = 1000);

    // Getters for motor parameters
    bool isReversed() const { return reverse; }
    bool hasFeedback() const { return feedback; }
    int getMaxSpeed() const { return maxSpeed; }
    int getMinSpeed() const { return minSpeed; }
};

// Generic 2-pin Motor Driver Class
class MotorTwopin : public Motor {
private:
    int PWMpin;
    int DIRpin;
public:
    MotorTwopin(int PWMpin, int DIRpin, bool reverse = false) : Motor(reverse), PWMpin(PWMpin), DIRpin(DIRpin) {
        pinMode(PWMpin, OUTPUT);
        pinMode(DIRpin, OUTPUT);
    }

    void spin(int speed, Direction direction) override;
    void setSpeed(int speed) override;
    void stop() override;
};

// Specific Motor Driver Classes
using MD10C = MotorTwopin;
using SmartElex = MotorTwopin;
using MD20C = MotorTwopin;

// L298N Class
class L298N : public Motor {
private:
    int PWMpin;
    int DIRpin1;
    int DIRpin2;
public:
    L298N(int PWMpin, int DIRpin1, int DIRpin2, bool reverse = false) : Motor(reverse), PWMpin(PWMpin), DIRpin1(DIRpin1), DIRpin2(DIRpin2) {
        pinMode(PWMpin, OUTPUT);
        pinMode(DIRpin1, OUTPUT);
        pinMode(DIRpin2, OUTPUT);
    }

    void spin(int speed, Direction direction) override;
    void setSpeed(int speed) override;
    void stop() override;
};

// BTS7960 Class
class BTS7960 : public Motor {
private:
    int LPWMpin;
    int RPWMpin;
    int LENpin;
    int RENpin;
public:
    BTS7960(int LPWMpin, int RPWMpin, int LENpin, int RENpin, bool reverse = false) : Motor(reverse), LPWMpin(LPWMpin), RPWMpin(RPWMpin), LENpin(LENpin), RENpin(RENpin) {
        pinMode(LPWMpin, OUTPUT);
        pinMode(RPWMpin, OUTPUT);
        pinMode(LENpin, OUTPUT);
        pinMode(RENpin, OUTPUT);
    }

    BTS7960(int LPWMpin, int RPWMpin, bool reverse = false) : Motor(reverse), LPWMpin(LPWMpin), RPWMpin(RPWMpin) {
        LENpin = -1; // Not used
        RENpin = -1; // Not used
        pinMode(LPWMpin, OUTPUT);
        pinMode(RPWMpin, OUTPUT);
    }

    void spin(int speed, Direction direction) override;
    void setSpeed(int speed) override;
    void stop() override;
};

// Drive Class for controlling multiple motors
class Drive{
private:
    enum WheelCount {
       THREE_WHEEL = 3,
       FOUR_WHEEL = 4
    };

    WheelCount wheelCount; // Number of wheels
    Motor* m1;
    Motor* m2;
    Motor* m3;
    Motor* m4;
    
    float wheel1speed = 1.0;
    float wheel2speed = 1.0;
    float wheel3speed = 1.0;
    float wheel4speed = 1.0;

    float Matrix3w[3][3] = {}; // 3-wheel kinematic matrix


    float Matrix4w[4][3] = {}; // 4-wheel kinematic matrix

public:
    // 4 wheel drive
    Drive(Motor* m1, Motor* m2, Motor* m3, Motor* m4): m1(m1), m2(m2), m3(m3), m4(m4), wheelCount(FOUR_WHEEL) {};
    
    // 3 wheel drive
    Drive(Motor* m1, Motor* m2, Motor* m3) : m1(m1), m2(m2), m3(m3), wheelCount(THREE_WHEEL) {};
    
    void setWheelSpeedFactor(float wheel1speed, float wheel2speed, float wheel3speed, float wheel4speed);
    void setWheelSpeedFactor(float wheel1speed, float wheel2speed, float wheel3speed);

    void move(int vx, int vy, int omega);
    void stop();

    WheelCount getWheelCount() const { return wheelCount; }
};

#endif // MDLIB_H