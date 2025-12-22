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

