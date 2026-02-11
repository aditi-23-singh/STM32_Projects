🚀 Distributed Dual-Node Stepper Motor Control System
Bi-Directional UART Communication | Dual STM32 | TMC2208 | Event-Driven Firmware
📌 Project Summary

Designed and implemented a distributed embedded control architecture using two STM32F051 microcontrollers communicating over bi-directional UART, where both nodes are capable of controlling the same stepper motor through a TMC2208 driver.

The system supports:

Motor control from either board

Real-time command arbitration

Single click → Start/Stop

Double click → Reverse direction

Long press → Mode control

Deterministic timer-based pulse generation

LED-assisted hardware debugging

This project demonstrates distributed embedded design, peripheral-level control, communication protocol handling, and real-time event synchronization.

🧠 System Architecture
Both STM32 boards:

Scan user inputs

Generate control commands

Transmit and receive via UART

Update motor state based on synchronized command logic

Motor control authority is dynamically managed through command synchronization.

⚙️ Core Engineering Components
1️⃣ Bi-Directional UART Communication

Full-duplex UART configuration

Interrupt-driven RX handling

Lightweight command protocol

Conflict handling logic

Command acknowledgment strategy

Key focus areas:

Clock accuracy & baud rate matching

Framing reliability

Data integrity verification

Real-time synchronization

This demonstrates understanding of multi-node embedded systems and communication integrity.

2️⃣ Event-Based Input Engine

Implemented a structured FSM to detect:

Single click

Double click (timing window based)

Long press

Double click triggers direction reversal.

Key techniques:

Edge detection

Non-blocking timing logic

Debounce filtering

Deterministic state transitions

This reflects correct event-driven firmware design rather than delay-based logic.

3️⃣ Deterministic Motor Control (TMC2208 – STEP/DIR Mode)

Hardware timer-based pulse generation

Adjustable step frequency

Direction toggling without step loss

Safe enable/disable sequencing

Synchronization with UART events

Ensured:

No missed steps during direction changes

No blocking delays

Pulse width constraints respected

Stable motor operation under distributed command input

4️⃣ Distributed Control Handling

Since both boards can issue motor commands:

Implemented command synchronization logic

Prevented inconsistent motor states

Ensured deterministic behavior under concurrent inputs

This models real-world distributed embedded systems where multiple controllers share authority.


5️⃣ Structured Hardware Debugging

Used LED signaling as a deterministic debug interface:

UART transmit/receive confirmation

State machine transitions

Direction change verification

Error detection states

Also performed:

Register-level inspection

Timer debugging

Peripheral clock validation

Step signal probing

Demonstrates disciplined debugging methodology.

🧪 Engineering Challenges Solved

Reliable double-click detection under timing constraints

UART synchronization between two MCUs

Preventing step loss during direction reversal

Avoiding race conditions in distributed control

Maintaining deterministic pulse generation

🎯 What This Project Demonstrates

This project reflects:

Multi-controller embedded system design

Real-time event-driven firmware

Deterministic motor control

Communication protocol implementation

Structured debugging methodology

Hardware-level reasoning under timing constraints

It models a scaled-down version of distributed hardware control systems used in industrial and silicon enablement environments.

Maintaining deterministic pulse generation

🔬 Potential Extensions

DMA-based UART communication

Acceleration ramp (trapezoidal velocity profile)

Arbitration protocol with priority logic

RTOS-based task scheduling

Closed-loop feedback with encoder

CRC-based command validation
