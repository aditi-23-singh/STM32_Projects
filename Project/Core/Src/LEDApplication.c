#include "LEDApplication.h"

#define FAST_BLINK_PERIOD_MS  50   // Fast blink: toggle every 25ms (50ms full cycle)
#define SLOW_BLINK_PERIOD_MS  200  // Slow blink: toggle every 100ms (200ms full cycle)
#define BLINK_DURATION_MS     1000 // Blink for 1 second then return to IDLE

/* LED state variables */
uint32_t led_timer = 0;
uint8_t led_state = 0;  // 0=OFF, 1=ON

/* Mode control */
uint8_t local_mode = 0;   // Mode for LD3 (local LED)
uint8_t remote_mode = 0;  // Mode for LD4 (remote LED controlled via UART)

/* Timing variables */
uint32_t local_blink_timer = 0;    // Timer for LD3 blinking
uint32_t local_duration_timer = 0; // Duration timer for LD3 (1 second countdown)
uint32_t remote_blink_timer = 0;   // Timer for LD4 blinking
uint32_t remote_duration_timer = 0;// Duration timer for LD4 (1 second countdown)
uint8_t prev_local_mode = 0;

extern uint8_t current_mode;

void UpdateLED(void)
{
	// Check if the mode has changed since the last millisecond
	    if (current_mode != prev_local_mode)
	    {
	        // RESET TIMERS: This makes the 1s duration start over for the new mode
	        local_duration_timer = 0;
	        local_blink_timer = 0;

	        // Ensure the LED starts in a known state for the new mode
	        if (current_mode == 1 || current_mode == 2) {
	             HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET);
	        }
	    }
	    prev_local_mode = current_mode;

	    local_mode = current_mode;

    // --- LOCAL LED (LD3) CONTROL ---
    switch (local_mode)
    {
        case 0:  // IDLE - LED OFF
            HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);
            local_blink_timer = 0;
            local_duration_timer = 0;
            break;

        case 1:  // SINGLE CLICK - Fast blink for 1 second
            local_duration_timer++;
            local_blink_timer++;

            // Toggle every 25ms (50ms period = ON for 25ms, OFF for 25ms)
            if (local_blink_timer >= 25)
            {
                local_blink_timer = 0;
                HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
            }

            // After 1 second, return to IDLE
            if (local_duration_timer >= BLINK_DURATION_MS)
            {
                local_mode = 0;
                current_mode = 0;
                local_duration_timer = 0;
                local_blink_timer = 0;
                HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);
            }
            break;

        case 2:  // DOUBLE CLICK - Slow blink for 1 second
            local_duration_timer++;
            local_blink_timer++;

            // Toggle every 100ms (200ms period = ON for 100ms, OFF for 100ms)
            if (local_blink_timer >= 100)
            {
                local_blink_timer = 0;
                HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
            }

            // After 1 second, return to IDLE
            if (local_duration_timer >= BLINK_DURATION_MS)
            {
                local_mode = 0;
                current_mode = 0;
                local_duration_timer = 0;
                local_blink_timer = 0;
                HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);
            }
            break;

        case 3:  // HOLD - LED ON continuously
            HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET);
            local_blink_timer = 0;
            local_duration_timer = 0;
            break;

        default:
            HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);
            local_blink_timer = 0;
            local_duration_timer = 0;
            break;
    }

    // --- REMOTE LED (LD4) CONTROL (receives commands via UART) ---
    switch (remote_mode)
    {
        case 0:  // IDLE - LED OFF
            HAL_GPIO_WritePin(LD4_GPIO_Port, LD4_Pin, GPIO_PIN_RESET);
            remote_blink_timer = 0;
            remote_duration_timer = 0;
            break;

        case 1:  // SINGLE CLICK - Fast blink for 1 second
            remote_duration_timer++;
            remote_blink_timer++;

            // Toggle every 25ms (50ms period)
            if (remote_blink_timer >= 25)
            {
                remote_blink_timer = 0;
                HAL_GPIO_TogglePin(LD4_GPIO_Port, LD4_Pin);
            }

            // After 1 second, return to IDLE
            if (remote_duration_timer >= BLINK_DURATION_MS)
            {
                remote_mode = 0;
                remote_duration_timer = 0;
                remote_blink_timer = 0;
                HAL_GPIO_WritePin(LD4_GPIO_Port, LD4_Pin, GPIO_PIN_RESET);
            }
            break;

        case 2:  // DOUBLE CLICK - Slow blink for 1 second
            remote_duration_timer++;
            remote_blink_timer++;

            // Toggle every 100ms (200ms period)
            if (remote_blink_timer >= 100)
            {
                remote_blink_timer = 0;
                HAL_GPIO_TogglePin(LD4_GPIO_Port, LD4_Pin);
            }

            // After 1 second, return to IDLE
            if (remote_duration_timer >= BLINK_DURATION_MS)
            {
                remote_mode = 0;
                remote_duration_timer = 0;
                remote_blink_timer = 0;
                HAL_GPIO_WritePin(LD4_GPIO_Port, LD4_Pin, GPIO_PIN_RESET);
            }
            break;

        case 3:  // HOLD - LED ON continuously
            HAL_GPIO_WritePin(LD4_GPIO_Port, LD4_Pin, GPIO_PIN_SET);
            remote_blink_timer = 0;
            remote_duration_timer = 0;
            break;

        default:
            HAL_GPIO_WritePin(LD4_GPIO_Port, LD4_Pin, GPIO_PIN_RESET);
            remote_blink_timer = 0;
            remote_duration_timer = 0;
            break;
    }
}

void SetRemoteMode(uint8_t mode)
{
	remote_mode = mode;
	    remote_duration_timer = 0; // Ensures Board B starts the 1s timer from zero
	    remote_blink_timer = 0;    // Ensures Board B starts the first blink immediately
}
