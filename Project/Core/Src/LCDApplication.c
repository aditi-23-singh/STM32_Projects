#include "LCDApplication.h"

uint8_t prev_mode = 255;
uint32_t prev_delay = 0;
uint8_t current_mode = 0;

extern uint32_t LED_delays[DELAY_COUNT];
extern uint32_t active_LED_delays[DELAY_COUNT];
extern uint32_t led_timer;
extern uint8_t toggle_count;
extern int8_t Delay_index;
extern int8_t Direction;

void UpdateDisplay(void)
{
    static uint32_t last_mode_change = 0;
    static uint8_t display_mode = 0;  // Separate variable for display

    // Update display when mode changes
    if (current_mode != prev_mode)
    {
        LCD_SetCursor(0,0);
        switch (current_mode)
        {
            case 3:
                LCD_Print("Mode: HOLD      ");
                display_mode = 3;
                break;
            case 2:
                LCD_Print("Mode: DOUBLE    ");
                display_mode = 2;
                last_mode_change = HAL_GetTick();
                break;
            case 1:
                LCD_Print("Mode: SINGLE    ");
                display_mode = 1;
                last_mode_change = HAL_GetTick();
                break;
            case 0:
                // Only update to IDLE if not in a temporary display state
                if (display_mode == 0)
                {
                    LCD_Print("Mode: IDLE      ");
                }
                break;
            default:
                LCD_Print("Mode: IDLE      ");
                display_mode = 0;
                break;
        }
        prev_mode = current_mode;
    }

    // Auto-clear temporary mode display after 1 second
    if ((display_mode == 1 || display_mode == 2) &&
        (HAL_GetTick() - last_mode_change >= 1000))
    {
        LCD_SetCursor(0,0);
        LCD_Print("Mode: IDLE      ");
        display_mode = 0;
    }

    // Update delay display
    if (active_LED_delays[Delay_index] != prev_delay)
    {
        char buffer[20];
        LCD_SetCursor(1, 0);
        snprintf(buffer, sizeof(buffer), "Delay: %lu ms   ", active_LED_delays[Delay_index]);
        LCD_Print(buffer);
        prev_delay = active_LED_delays[Delay_index];
    }
}
