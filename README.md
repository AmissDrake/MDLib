# MDLib

![v1.1.1](https://img.shields.io/badge/Version-1.1.1-blue)

An intuitive, object-oriented Arduino library for simplifying the control of DC motors and complex holonomic drive systems.

MDLib provides a clean, unified interface for a variety of common motor drivers and includes powerful built-in kinematic models for 3 and 4-wheel omni-directional robots, allowing you to get your project moving in minutes.

## Features

- **Unified Motor Control:** A single, consistent API for different motor drivers. Just instantiate the class for your hardware and go.

- **Built-in Kinematic Models:** Includes pre-built inverse kinematics for 3-wheel and 4-wheel holonomic drive systems. Simply call drive.move(vx, vy, omega) and the library handles the complex wheel speed calculations.

- **Arduino Compatible:** Works with any board supported by the Arduino framework, including Uno, Nano, Mega, ESP32, and more. Also works with any board which supports the Arduino SDK.

## Licence

This project is licensed under the [MIT Licence](./LICENSE).
