# Bluetooth and CAN Bus Control System

An embedded control system enabling communication between an Arduino Uno and external devices over both Bluetooth and CAN bus — built as a separate module assignment from the [gyroscope-based gesture/throttle control project](https://github.com/Bidex18/Gyroscope-Based-Gesture-Recognition-System-for-Throttle-Valve-Control), focused specifically on the dual-protocol communication architecture.

## What it does

- Enables an Arduino Uno to communicate with external devices via **Bluetooth** and **CAN bus** simultaneously.
- Controls LED state and reads push-button/analog sensor input via Bluetooth.
- Exchanges data with external devices over the CAN bus network.
- Designed with a modular architecture for easy scalability and integration of additional peripherals.

## Tools & Stack

- Arduino Uno
- CAN bus module
- Bluetooth module
- Arduino IDE (C++)

## How it works

1. The Arduino reads local inputs (push buttons, analog sensors) and controls LED outputs.
2. Bluetooth provides a wireless local/remote control channel for these inputs/outputs.
3. In parallel, the system communicates over CAN bus, exchanging data with other devices on the network without interference.
4. The modular design allows additional sensors, peripherals or communication channels to be added without restructuring the core system.

## Files

- `ACW2_microcontrollers.ino` — main Arduino firmware
- `ACW2.pdf` — full technical write-up and evaluation

## Status

Individual project. Built and tested on physical hardware. Suitable as a foundation for automotive, industrial, or home automation applications.
