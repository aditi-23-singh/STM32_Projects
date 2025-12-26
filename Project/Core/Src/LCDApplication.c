/* Modified LCDApplication.c - SHOW BOTH BOARDS ON 20x4 LCD */
#include "LCDApplication.h"
#include "LCD.h"
#include "LEDApplication.h"
#include "gpio.h"
#include "stm32f0xx_hal.h"
#include "stdio.h"
#include "UARTCommunication.h"  // NEW: Add this

#define MODE_IDLE   0
#define MODE_SINGLE 1
#define MODE_DOUBLE 2
#define MODE_HOLD   3

uint8_t prev_mode = 255;
uint32_t prev_delay = 0;

// NEW: Previous remote state
uint8_t prev_remote_mode = 255;
uint32_t prev_remote_delay = 0;

uint8_t current_mode;

int8_t index;
extern int8_t direction;
extern uint32_t active_delays[DELAY_COUNT];

extern uint8_t btn;
extern uint8_t last_btn_state;
extern uint32_t last_btn_tick;
extern uint8_t click_count;
extern uint32_t last_hold_tick;
extern uint8_t hold_active;

// NEW: Remote board variables
extern uint8_t remote_mode;
extern int8_t remote_index;
extern int8_t remote_direction;
extern uint32_t remote_delays[DELAY_COUNT];

// NEW: Tracking for sending state
static uint8_t last_sent_mode = 255;
static int8_t last_sent_index = -1;
static int8_t last_sent_direction = 0;

void HandleLCDApplication()
{
    /* Determine current LOCAL mode */
    if (hold_active)
        current_mode = MODE_HOLD;
    else if (click_count == 2)
        current_mode = MODE_DOUBLE;
    else if (click_count == 1)
        current_mode = MODE_SINGLE;
    else
        current_mode = MODE_IDLE;

    char buffer[21];  // 20 chars + null terminator

    // ========== UPDATE LOCAL (ROW 0 & 1) ==========

    /* Update LOCAL Mode Display */
    if (current_mode != prev_mode)
    {
        LCD_SetCursor(0, 0);

        switch (current_mode)
        {
            case MODE_HOLD:
                LCD_Print("LOCAL - HOLD        ");
                break;
            case MODE_DOUBLE:
                LCD_Print("LOCAL - DOUBLE      ");
                break;
            case MODE_SINGLE:
                LCD_Print("LOCAL - SINGLE      ");
                break;
            default:
                LCD_Print("LOCAL - IDLE        ");
                break;
        }

        prev_mode = current_mode;

        // NEW: Send state when mode changes
        UART_SendState(current_mode, index, direction);
        last_sent_mode = current_mode;
        last_sent_index = index;
        last_sent_direction = direction;
    }

    /* Update LOCAL Delay Display */
    if (active_delays[index] != prev_delay)
    {
        LCD_SetCursor(1, 0);
        sprintf(buffer, "Delay: %lu ms       ", active_delays[index]);
        LCD_Print(buffer);

        prev_delay = active_delays[index];

        // NEW: Send state when delay index changes
        if (last_sent_index != index || last_sent_direction != direction) {
            UART_SendState(current_mode, index, direction);
            last_sent_index = index;
            last_sent_direction = direction;
        }
    }

    // ========== NEW: UPDATE REMOTE (ROW 2 & 3) ==========

    /* Update REMOTE Mode Display */
    if (remote_mode != prev_remote_mode)
    {
        LCD_SetCursor(2, 0);

        switch (remote_mode)
        {
            case MODE_HOLD:
                LCD_Print("REMOTE - HOLD       ");
                break;
            case MODE_DOUBLE:
                LCD_Print("REMOTE - DOUBLE     ");
                break;
            case MODE_SINGLE:
                LCD_Print("REMOTE - SINGLE     ");
                break;
            default:
                LCD_Print("REMOTE - IDLE       ");
                break;
        }

        prev_remote_mode = remote_mode;
    }

    /* Update REMOTE Delay Display */
    uint32_t current_remote_delay = remote_delays[remote_index];
    if (current_remote_delay != prev_remote_delay)
    {
        LCD_SetCursor(3, 0);
        sprintf(buffer, "Delay: %lu ms       ", current_remote_delay);
        LCD_Print(buffer);

        prev_remote_delay = current_remote_delay;
    }
}
