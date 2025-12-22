# STM32F0 LED Delay Control with Single/Double Click

## Overview

This project implements a **non-blocking LED blinking system** on the **STM32F0-Discovery (STM32F051R8)** board. The blinking behavior is controlled via a **single push-button**, allowing the user to:

- **Single click:** Advance to the next LED blink delay  
- **Double click:** Reverse the traversal direction of the delay sequence  

The system uses a **delay array**: `{500 ms, 1000 ms, 2000 ms, 3000 ms}`. The LED continuously toggles according to the current delay value, and button presses modify the delay or direction dynamically.

---

## Features

- Non-blocking LED blink using `HAL_GetTick()`  
- Configurable delay array for flexible blink patterns  
- Single click → next delay (according to current direction)  
- Double click → reverse delay traversal direction  
- Deterministic and safe handling of multiple clicks within 200 ms  
- Easy to extend for long-press or multi-click functionality  

---

## Hardware Required

- STM32F0-Discovery Board (STM32F051R8)  
- USER button (built-in on PA0)  
- LD3/LD4 LEDs (built-in on PC8/PC9)  

---

## Project Structure
click1/
   -> Core/ # Source and header files
   -> Drivers/ # STM32 HAL drivers
   -> click1.ioc # CubeMX configuration file
   -> STM32F051R8TX_FLASH.ld # Linker script
   -> .gitignore # Git ignore file
   -> README.md # This file

- **Core/**: Contains `main.c` and peripheral initialization code  
- **Drivers/**: HAL libraries required for the STM32F0  
- **click1.ioc**: CubeMX project configuration  
- **.gitignore**: Prevents build artifacts from being pushed to GitHub  

---

## Usage

1. Open the project in **STM32CubeIDE**  
2. Build and flash to the STM32F0-Discovery board  
3. Observe the LED blinking pattern on **PC8**  
4. Press the USER button (PA0):
   - Single click → next delay
   - Double click → reverse direction
5. LED will continue blinking according to updated delay  

---

## Implementation Notes

- Uses **non-blocking timing** with `HAL_GetTick()` instead of `HAL_Delay()`  
- Button handling includes **software debouncing** and a **200 ms double-click detection window**  
- Triple or more clicks within 200 ms are treated as double-clicks to ensure deterministic behavior  
- Code follows an **event-driven architecture** with clear separation of LED and button logic  

---

## Future Improvements

- Add **long-press detection** for additional functions  
- Support **multi-LED patterns** or **PWM brightness control**  
- Port the logic to **other STM32 series boards** with minimal changes  

---

## Author

**Aditi Singh**  
B.Tech, Electronics & Communication Engineering, NIT Sikkim  
Passionate about **VLSI design, embedded systems, and firmware development**
