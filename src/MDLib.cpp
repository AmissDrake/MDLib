#include "MDLib.h"

/* ------------------------------------------------------------*/
/* ------------- Generic Motor Class Methods ------------------*/
/* ------------------------------------------------------------*/

void Motor::attachEncoder(int encoderPinA, int encoderPinB) {
    pinMode(encoderPinA, INPUT_PULLUP);
    pinMode(encoderPinB, INPUT_PULLUP);
    // Additional encoder setup can be added here

    feedback = true; // Set feedback flag to true
    this->ENCpinA = encoderPinA;
    this->ENCpinB = encoderPinB;
}

void Motor::threshold(int max, int min) {
    // Set the speed limits
    if (max >= 0 && max <= 255) {
        maxSpeed = max;
    }
    if (min >= 0 && min <= 255) {
        minSpeed = min;
    }
}

/* ------------------------------------------------------------*/
/* -------------- MotorTwoPin Class Methods -------------------*/
/* ------------------------------------------------------------*/

void MotorTwopin::spin(int speed, Direction direction) {
    // Ensure speed is within bounds
    speed = constrain(speed, minSpeed, maxSpeed);
    analogWrite(PWMpin, speed);
    
    // Set direction
    if (isReversed()) { direction = (Direction)(direction ^ 1); }
    digitalWrite(DIRpin, direction);
}

void MotorTwopin::setSpeed(int speed) {
    Direction direction = (speed < 0) ? CW : CCW; // If speed is negative rotate CW

    // Set speed
    speed = abs(speed);

    MotorTwopin::spin(speed, direction);
}

void MotorTwopin::stop() {
    analogWrite(PWMpin, 0); // Stop the Motor
}

/* ------------------------------------------------------------*/
/* ------------------ L298N Class Methods ---------------------*/
/* ------------------------------------------------------------*/

void L298N::spin(int speed, Direction direction) {
    // Ensure speed is within bounds
    speed = constrain(speed, minSpeed, maxSpeed);
    analogWrite(PWMpin, speed);
    
    // Set direction
    if (isReversed()) { direction = (Direction)(direction ^ 1); }
    digitalWrite(DIRpin1, direction);
    digitalWrite(DIRpin2, !direction);
}

void L298N::setSpeed(int speed) {
    Direction direction = (speed < 0) ? CW : CCW; // If speed is negative rotate CW
    speed = abs(speed);

    L298N::spin(speed, direction);
}

void L298N::stop() {
    analogWrite(PWMpin, 0); // Stop the Motor
}

/* ------------------------------------------------------------*/
/* ---------------- BTS7960 Class Methods ---------------------*/
/* ------------------------------------------------------------*/

void BTS7960::spin(int speed, Direction direction) {
    // Set direction
    if (isReversed()) { direction = (Direction)(direction ^ 1); }
    // Ensure speed is within bounds
    speed = constrain(speed, minSpeed, maxSpeed);

    if (direction == CCW) {
        analogWrite(LPWMpin, speed);
        if (LENpin != -1) {
        digitalWrite(LENpin, HIGH);
        }
        analogWrite(RPWMpin, 0);
        if (RENpin != -1) {
        digitalWrite(RENpin, LOW);
        }
    } else {
        analogWrite(LPWMpin, 0);
        if (LENpin != -1) {
        digitalWrite(LENpin, LOW);
        }
        analogWrite(RPWMpin, speed);
        if (RENpin != -1) {
        digitalWrite(RENpin, HIGH);
        }
    }
}

void BTS7960::setSpeed(int speed) {
    Direction direction = (speed < 0) ? CW : CCW; // If speed is negative rotate CW
    speed = abs(speed);

    BTS7960::spin(speed, direction);
}

void BTS7960::stop() {
    analogWrite(LPWMpin, 0);
    digitalWrite(LENpin, LOW);
    analogWrite(RPWMpin, 0);
    digitalWrite(RENpin, LOW);
}

/* ------------------------------------------------------------*/
/* ------------------ Drive Class Methods ---------------------*/
/* ------------------------------------------------------------*/

void Drive::setWheelSpeedFactor(float wheel1speed, float wheel2speed, float wheel3speed, float wheel4speed) {
    this->wheel1speed = wheel1speed;
    this->wheel2speed = wheel2speed;
    this->wheel3speed = wheel3speed;
    this->wheel4speed = wheel4speed;

    // Update the 4-wheel matrix
    Matrix4w[0][0] = -wheel1speed; // M1 Front Left Forward
    Matrix4w[0][1] = wheel1speed;  // M1 Front Left Left
    Matrix4w[0][2] = wheel1speed;  // M1 Front Left Rotate
    Matrix4w[1][0] = wheel2speed;  // M2 Front Right Forward
    Matrix4w[1][1] = wheel2speed;  // M2 Front Right Left
    Matrix4w[1][2] = wheel2speed;  // M2 Front Right Rotate
    Matrix4w[2][0] = wheel3speed;  // M3 Back Right Forward
    Matrix4w[2][1] = -wheel3speed; // M3 Back Right Left
    Matrix4w[2][2] = wheel3speed;  // M3 Back Right Rotate
    Matrix4w[3][0] = -wheel4speed; // M4 Back Left Forward
    Matrix4w[3][1] = -wheel4speed; // M4 Back Left Left
    Matrix4w[3][2] = wheel4speed;  // M4 Back Left Rotate
}

void Drive::setWheelSpeedFactor(float wheel1speed, float wheel2speed, float wheel3speed) {
    this->wheel1speed = wheel1speed;
    this->wheel2speed = wheel2speed;
    this->wheel3speed = wheel3speed;
    
    // Update the 3-wheel matrix
    Matrix3w[0][0] = -(2.0/sqrt(3.0)) * wheel1speed;    // M1 Front Left Forward 
    Matrix3w[0][1] = (1.0/2.0) * wheel1speed;           // M1 Front Left Left
    Matrix3w[0][2] = wheel1speed;                       // M1 Front Left Rotate
    Matrix3w[1][0] = (2.0/sqrt(3.0)) * wheel2speed;     // M2 Front Right Forward
    Matrix3w[1][1] = (1.0/2.0) * wheel2speed;           // M2 Front Right Left
    Matrix3w[1][2] = wheel2speed;                       // M2 Front Right Rotate
    Matrix3w[2][0] = 0;                                 // M3 Back Forward
    Matrix3w[2][1] = wheel3speed;                       // M3 Back Left
    Matrix3w[2][2] = wheel3speed;                       // M3 Back Rotate
}

void Drive::move(int vx, int vy, int omega) {
    if (wheelCount == FOUR_WHEEL) {
        // Calculate speeds for 4-wheel drive
        int speed1 = Matrix4w[0][0] * vx + Matrix4w[0][1] * vy + Matrix4w[0][2] * omega;
        int speed2 = Matrix4w[1][0] * vx + Matrix4w[1][1] * vy + Matrix4w[1][2] * omega;
        int speed3 = Matrix4w[2][0] * vx + Matrix4w[2][1] * vy + Matrix4w[2][2] * omega;
        int speed4 = Matrix4w[3][0] * vx + Matrix4w[3][1] * vy + Matrix4w[3][2] * omega;

        m1->setSpeed(speed1); 
        m2->setSpeed(speed2);
        m3->setSpeed(speed3);
        m4->setSpeed(speed4);
    } else if (wheelCount == THREE_WHEEL) {
        // Calculate speeds for 3-wheel drive
        int speed1 = Matrix3w[0][0] * vx + Matrix3w[0][1] * vy + Matrix3w[0][2] * omega;
        int speed2 = Matrix3w[1][0] * vx + Matrix3w[1][1] * vy + Matrix3w[1][2] * omega;
        int speed3 = Matrix3w[2][0] * vx + Matrix3w[2][1] * vy + Matrix3w[2][2] * omega;

        m1->setSpeed(speed1);
        m2->setSpeed(speed2);
        m3->setSpeed(speed3);
    }
}

void Drive::stop() {
    if (wheelCount == FOUR_WHEEL) {
        m1->stop();
        m2->stop();
        m3->stop();
        m4->stop();
    } else if (wheelCount == THREE_WHEEL) {
        m1->stop();
        m2->stop();
        m3->stop();
    }
}

