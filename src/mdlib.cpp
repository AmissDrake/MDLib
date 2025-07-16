#include <mdlib.h>


motor::motor(int PWMpin, int DIRpin, bool reverse) {
    this->PWMpin = PWMpin;
    this->DIRpin = DIRpin;
    this->reverse = reverse;

    this->mdType = TWO_PIN_MD; // Default to two-pin motor type

    pinMode(PWMpin, OUTPUT);
    pinMode(DIRpin, OUTPUT);
}

motor::motor(int PWMpin, int DIRpin1, int DIRpin2, bool reverse) {
    this->PWMpin = PWMpin;
    this->DIRpin1 = DIRpin1;
    this->DIRpin2 = DIRpin2;
    this->reverse = reverse;

    this->mdType = THREE_PIN_MD; // Set to three-pin motor type
    pinMode(PWMpin, OUTPUT);
    pinMode(DIRpin1, OUTPUT);
    pinMode(DIRpin2, OUTPUT);
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

void motor::spin(int speed, Direction direction) {
    // Ensure speed is within bounds
    speed = constrain(speed, this->minSpeed, maxSpeed);
    analogWrite(PWMpin, speed);
    
    // Set direction
    if (reverse == false) {
        if (mdType == TWO_PIN_MD) {
            digitalWrite(DIRpin, direction);
        } else {
            digitalWrite(DIRpin1, direction);   
            digitalWrite(DIRpin2, !direction);
        }
    } else {
        if (mdType == TWO_PIN_MD) {
            digitalWrite(DIRpin, !direction);
        } else {
            digitalWrite(DIRpin1, !direction);
            digitalWrite(DIRpin2, direction);
        }
    }
}

void motor::setSpeed(int speed) {
    bool dir = (speed < 0) ? LOW : HIGH;
    speed = abs(speed);
    speed = constrain(speed, minSpeed, maxSpeed);
    analogWrite(PWMpin, speed);
    if (mdType == TWO_PIN_MD) {
        digitalWrite(DIRpin, (reverse) ? !dir : dir);
    } else {
        digitalWrite(DIRpin1, (reverse) ? !dir : dir);
        digitalWrite(DIRpin2, (reverse) ? dir : !dir);
    }
}

void motor::stop() {
    analogWrite(PWMpin, 0); // Stop the motor
}

drive::drive(motor* m1, motor* m2, motor* m3, motor* m4) {
    this->m1 = m1;
    this->m2 = m2;
    this->m3 = m3;
    this->m4 = m4;
    this->wheelCount = FOUR_WHEEL; // Default to four wheels
}

void drive::setWheelSpeed(int wheel1speed, int wheel2speed, int wheel3speed, int wheel4speed) {
    this->wheel1speed = wheel1speed;
    this->wheel2speed = wheel2speed;
    this->wheel3speed = wheel3speed;
    this->wheel4speed = wheel4speed;

    // Update the 4-wheel matrix
    4wMatrix[0][0] = -wheel1speed; // M1 Front Left Forward
    4wMatrix[0][1] = wheel1speed;  // M1 Front Left Left
    4wMatrix[0][2] = wheel1speed;  // M1 Front Left Rotate
    4wMatrix[1][0] = wheel2speed;  // M2 Front Right Forward
    4wMatrix[1][1] = wheel2speed;  // M2 Front Right Left
    4wMatrix[1][2] = wheel2speed;  // M2 Front Right Rotate
    4wMatrix[2][0] = wheel3speed;  // M3 Back Right Forward
    4wMatrix[2][1] = -wheel3speed; // M3 Back Right Left
    4wMatrix[2][2] = wheel3speed;  // M3 Back Right Rotate
    4wMatrix[3][0] = -wheel4speed; // M4 Back Left Forward
    4wMatrix[3][1] = -wheel4speed; // M4 Back Left Left
    4wMatrix[3][2] = wheel4speed;  // M4 Back Left Rotate
}

drive::drive(motor* m1, motor* m2, motor* m3) {
    this->m1 = m1;
    this->m2 = m2;
    this->m3 = m3;
    this->wheelCount = THREE_WHEEL; // Default to three wheels

    this-> 3wMatrix[3][3] =  {
        {-(2/sqrt(3))*wheel1speed, (1/2)*wheel1speed, wheel1speed}, // M1 Front Left
        {(2/sqrt(3))*wheel2speed, (1/2)*wheel2speed, wheel2speed}, // M2 Front Right
        {0, -wheel3speed, wheel3speed}, // M3 Back
    };
}

void drive::setWheelSpeed(int wheel1speed, int wheel2speed, int wheel3speed) {
    this->wheel1speed = wheel1speed;
    this->wheel2speed = wheel2speed;
    this->wheel3speed = wheel3speed;
    
    // Update the 3-wheel matrix
    3wMatrix[0][0] = -(2/sqrt(3)) * wheel1speed; // M1 Front Left Forward 
    3wMatrix[0][1] = (1/2) * wheel1speed;       // M1 Front Left Left
    3wMatrix[0][2] = wheel1speed;               // M1 Front Left Rotate
    3wMatrix[1][0] = (2/sqrt(3)) * wheel2speed; // M2 Front Right Forward
    3wMatrix[1][1] = (1/2) * wheel2speed;       // M2 Front Right Left
    3wMatrix[1][2] = wheel2speed;               // M2 Front Right Rotate
    3wMatrix[2][0] = 0;                          // M3 Back Forward
    3wMatrix[2][1] = wheel3speed;               // M3 Back Left
    3wMatrix[2][2] = wheel3speed;               // M3 Back Rotate
}

void drive::move(int vx, int vy, int omega) {
    if (wheelCount == FOUR_WHEEL) {
        // Calculate speeds for 4-wheel drive
        int speed1 = wheel1speed * (-vx + vy + omega);
        int speed2 = wheel2speed * ( vx + vy + omega);
        int speed3 = wheel3speed * (vx - vy + omega);
        int speed4 = wheel4speed * (-vx - vy + omega);

        m1->setSpeed(speed1);
        m2->setSpeed(speed2);
        m3->setSpeed(speed3);
        m4->setSpeed(speed4);
    } else if (wheelCount == THREE_WHEEL) {
        // Calculate speeds for 3-wheel drive
        int speed1 = wheel1speed * (-(2/sqrt(3)) * vx + (1/2) * vy + omega);
        int speed2 = wheel2speed * ( (2/sqrt(3)) * vx + (1/2) * vy + omega);
        int speed3 = wheel3speed * (0 - vy + omega);

        m1->setSpeed(speed1);
        m2->setSpeed(speed2);
        m3->setSpeed(speed3);
    }
}

void drive::stop() {
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

