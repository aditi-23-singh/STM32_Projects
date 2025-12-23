# **# STM32 Projects**



This repository contains STM32 embedded projects demonstrating **single click, double click, and long-press (hold)** button handling to control LED behavior using **STM32 HAL.**



The projects are implemented using **polling-based logic with `HAL\_GetTick()`**, without blocking delays, making them suitable for real-time embedded applications.





### **## Repository Structure**



Projects/

├── click1/

│ └── main.c

├── hold1/

│ └── main.c

└── README.md

### 

### **## Hardware Requirements**



**-** STM32 development board (e.g., STM32F0/F1 series)

\- User Button connected to **PA0**

\- LED connected to **PC8 / PC13** (depending on board)

\- STM32CubeIDE



**---**



## **## Project 1: `click1` — Button Click Control**



#### **### Features**

**- Single Click**

&nbsp; - Cycles through predefined LED blink delays

\- **Double Click**

&nbsp; - Reverses the blink direction (forward / backward)

\- Non-blocking implementation using `HAL\_GetTick()`



#### **### LED Behavior**

| Action 	| Result |

|------		|-------|

| Single Click 	| Move to next blink delay |

| Double Click 	| Reverse delay sequence direction |

#### 

#### **### Delay Values**



{ 250 ms, 500 ms, 1000 ms, 2000 ms }





## **## Project 2: hold1 — Button Click + Long Press Control**



#### **#### Features**



* Single Click->Change LED blink speed
* Double Click->Reverse blinking direction
* Long Press (Hold)->LED blinks at half the current delay
* Normal blinking is suspended during hold
* Clean separation of normal mode and hold mode
* Hold Logic->Hold detected after 100 ms
* Delay values are halved dynamically
* Original delays restored on button release



#### **### Software Design Highlights**



* Uses HAL\_GPIO\_TogglePin() for efficient blinking
* Uses unsigned time comparison to handle SysTick overflow safely
* Prevents multiple timing sources from controlling the LED simultaneously
* Suitable for extension to:
* Interrupt-based button handling
* Finite State Machine (FSM)
* RTOS-based systems



#### **### Future Improvements**



* Debouncing using timer-based filtering
* EXTI interrupt-based button handling
* LED control using HAL\_GPIO\_WritePin() with state machine
* Code reuse using common button driver module



#### 

#### **###Author**



**Aditi Singh**

**B.Tech, Electronics and Communication Engineering**

**NIT Sikkim**

**Interests: Embedded Systems, VLSI, Firmware Development**

