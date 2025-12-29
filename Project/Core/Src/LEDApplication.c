#include "LEDApplication.h"

#define FAST_BLINK_PERIOD_MS  50   // Fast blink: toggle every 25ms (50ms full cycle)
#define SLOW_BLINK_PERIOD_MS  200  // Slow blink: toggle every 100ms (200ms full cycle)
#define BLINK_DURATION_MS     1000 // Blink for 1 second then return to IDLE

/* LED state variables */
uint32_t LedTimer = 0;
uint8_t LedState = 0;  // 0=OFF, 1=ON


uint8_t LocalMode = 0;   // Mode for LD3 (local LED)
uint8_t RemoteMode = 0;  // Mode for LD4 (remote LED controlled via UART)

/* Timing variables */
uint32_t LocalBlinkTimer = 0;    // Timer for LD3 blinking
uint32_t LocalDurationTimer= 0; // Duration timer for LD3 (1 second countdown)
uint32_t RemoteBlinkTimer = 0;   // Timer for LD4 blinking
uint32_t RemoteDurationTimer = 0;// Duration timer for LD4 (1 second countdown)
uint8_t PrevLocalMode = 0;

extern uint8_t CurrentMode;

void UpdateLED(void)
{
	// Check if the mode has changed since the last millisecond
	    if (CurrentMode != PrevLocalMode)
	    {
	        // RESET TIMERS: This makes the 1s duration start over for the new mode
	        LocalDurationTimer= 0;

	        LocalBlinkTimer = 0;

	        // Ensure the LED starts in a known state for the new mode
	        if (CurrentMode == 1 || CurrentMode == 2) {
	             HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET);
	        }
	    }
	    PrevLocalMode = CurrentMode;

	    LocalMode = CurrentMode;

    // --- LOCAL LED (LD3) CONTROL ---
    switch (LocalMode)
    {
        case 0:  // IDLE - LED OFF
            HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);
            LocalBlinkTimer = 0;
            LocalDurationTimer = 0;
            break;

        case 1:  // SINGLE CLICK - Fast blink for 1 second
            LocalDurationTimer++;
            LocalBlinkTimer++;

            // Toggle every 25ms (50ms period = ON for 25ms, OFF for 25ms)
            if (LocalBlinkTimer >= 25)
            {
                LocalBlinkTimer = 0;
                HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
            }

            // After 1 second, return to IDLE
            if (LocalDurationTimer >= BLINK_DURATION_MS)
            {
                LocalMode = 0;
                CurrentMode = 0;
                LocalDurationTimer = 0;
                LocalBlinkTimer = 0;
                HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);
            }
            break;

        case 2:  // DOUBLE CLICK - Slow blink for 1 second
            LocalDurationTimer++;
            LocalBlinkTimer++;

            // Toggle every 100ms (200ms period = ON for 100ms, OFF for 100ms)
            if (LocalBlinkTimer >= 100)
            {
                LocalBlinkTimer = 0;
                HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
            }

            // After 1 second, return to IDLE
            if (LocalDurationTimer >= BLINK_DURATION_MS)
            {
                LocalMode = 0;
                CurrentMode = 0;
                LocalDurationTimer = 0;
                LocalBlinkTimer = 0;
                HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);
            }
            break;

        case 3:  // HOLD - LED ON continuously
            HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET);
            LocalBlinkTimer = 0;
            LocalDurationTimer = 0;
            break;

        default:
            HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);
            LocalBlinkTimer = 0;
            LocalDurationTimer = 0;
            break;
    }

    // --- REMOTE LED (LD4) CONTROL (receives commands via UART) ---
    switch (RemoteMode)
    {
        case 0:  // IDLE - LED OFF
            HAL_GPIO_WritePin(LD4_GPIO_Port, LD4_Pin, GPIO_PIN_RESET);
            RemoteBlinkTimer = 0;
            RemoteDurationTimer = 0;
            break;

        case 1:  // SINGLE CLICK - Fast blink for 1 second
            RemoteDurationTimer++;
            RemoteBlinkTimer++;

            // Toggle every 25ms (50ms period)
            if (RemoteBlinkTimer >= 25)
            {
                RemoteBlinkTimer = 0;
                HAL_GPIO_TogglePin(LD4_GPIO_Port, LD4_Pin);
            }

            // After 1 second, return to IDLE
            if (RemoteDurationTimer >= BLINK_DURATION_MS)
            {
                RemoteMode = 0;
                RemoteDurationTimer = 0;
                RemoteBlinkTimer = 0;
                HAL_GPIO_WritePin(LD4_GPIO_Port, LD4_Pin, GPIO_PIN_RESET);
            }
            break;

        case 2:  // DOUBLE CLICK - Slow blink for 1 second
            RemoteDurationTimer++;
            RemoteBlinkTimer++;

            // Toggle every 100ms (200ms period)
            if (RemoteBlinkTimer >= 100)
            {
                RemoteBlinkTimer = 0;
                HAL_GPIO_TogglePin(LD4_GPIO_Port, LD4_Pin);
            }

            // After 1 second, return to IDLE
            if (RemoteDurationTimer >= BLINK_DURATION_MS)
            {
                RemoteMode = 0;
                RemoteDurationTimer = 0;
                RemoteBlinkTimer = 0;
                HAL_GPIO_WritePin(LD4_GPIO_Port, LD4_Pin, GPIO_PIN_RESET);
            }
            break;

        case 3:  // HOLD - LED ON continuously
            HAL_GPIO_WritePin(LD4_GPIO_Port, LD4_Pin, GPIO_PIN_SET);
            RemoteBlinkTimer = 0;
            RemoteDurationTimer = 0;
            break;

        default:
            HAL_GPIO_WritePin(LD4_GPIO_Port, LD4_Pin, GPIO_PIN_RESET);
            RemoteBlinkTimer = 0;
            RemoteDurationTimer = 0;
            break;
    }
}

void SetRemoteMode(uint8_t mode)
{
	RemoteMode = mode;
	    RemoteDurationTimer = 0; // Ensures Board B starts the 1s timer from zero
	    RemoteBlinkTimer = 0;    // Ensures Board B starts the first blink immediately
}
