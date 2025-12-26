/* CORRECTED LEDApplication.c - Don't modify remote variables! */
#include <LEDApplication.h>
#include "stm32f0xx_hal.h"
#include "stdio.h"
#include "UARTCommunication.h"

uint32_t delays[] = { 250, 500, 1000, 2000 };
uint32_t active_delays[DELAY_COUNT];

int8_t index = 0;
int8_t direction = 1;

uint32_t last_led_tick = 0;

extern uint8_t current_mode;
extern uint8_t btn;
extern uint8_t last_btn_state;
extern uint32_t last_btn_tick;
extern uint8_t click_count;
extern uint32_t last_hold_tick;
extern uint8_t hold_active;

// IMPORT remote board variables from UARTCommunication.c
extern uint8_t remote_mode;
extern int8_t remote_index;
extern int8_t remote_direction;
extern uint32_t remote_delays[DELAY_COUNT];

// LOCAL COPY for remote LED cycling - don't modify the extern variables!
static int8_t remote_led_index = 0;
static int8_t remote_led_direction = 1;
static uint32_t last_remote_led_tick = 0;

void LEDInit()
{
    for (int i = 0; i < DELAY_COUNT; i++) {
        active_delays[i] = delays[i];
    }
}

void HandleLEDApplication()
{
    // ========== LOCAL LED LOGIC (PC8) ==========

    /* LED BLINK LOGIC */
    if (!hold_active && (HAL_GetTick() - last_led_tick >= active_delays[index]))
    {
        last_led_tick = HAL_GetTick();
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_8);

        uint8_t current_pin_state = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_8);
        UART_SendState(current_mode, index, direction, current_pin_state);

        index += direction;

        if (index >= DELAY_COUNT)
            index = 0;
        else if (index < 0)
            index = DELAY_COUNT - 1;
    }

    /* Hold Mode - Halve delays ONCE when entering */
    if (hold_active && (active_delays[0] == delays[0]))
    {
        for (int i = 0; i < DELAY_COUNT; i++)
            active_delays[i] = delays[i] / 2;
    }

    /* Restore delays when hold released */
    if (!hold_active && (active_delays[0] != delays[0]))
    {
        for (int i = 0; i < DELAY_COUNT; i++)
            active_delays[i] = delays[i];
    }

    /* Hold Action (Fast Cycling) */
    if (hold_active && (HAL_GetTick() - last_hold_tick >= active_delays[index]))
    {
        last_hold_tick = HAL_GetTick();
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_8);

        index += direction;
        if (index >= DELAY_COUNT)
            index = 0;
        else if (index < 0)
            index = DELAY_COUNT - 1;
    }

    /* Click Resolution*/
    if (!hold_active && click_count > 0 && (HAL_GetTick() - last_btn_tick > 50))
    {
        if (click_count == 1)
        {
            index += direction;
            if (index >= DELAY_COUNT)
                index = 0;
            else if (index < 0)
                index = DELAY_COUNT - 1;
        }
        else
        {
            direction = -direction;
        }

        click_count = 0;
    }

    // ========== REMOTE LED LOGIC (PC9) ==========

    /* Synchronize local copy with received remote state when it changes */
    static int8_t last_received_index = 0;
    static int8_t last_received_direction = 1;

    if (remote_index != last_received_index || remote_direction != last_received_direction)
    {
        // New data received - sync local copy
        remote_led_index = remote_index;
        remote_led_direction = remote_direction;
        last_received_index = remote_index;
        last_received_direction = remote_direction;
    }

    /* Blink remote LED based on remote board state */
    if (remote_mode != 0)  // Not IDLE
    {
        uint32_t current_remote_delay = remote_delays[remote_led_index];

        if (HAL_GetTick() - last_remote_led_tick >= current_remote_delay)
        {
            last_remote_led_tick = HAL_GetTick();
            HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_9);  // Toggle remote LED

            // Cycle through delays using LOCAL copy
            remote_led_index += remote_led_direction;
            if (remote_led_index >= DELAY_COUNT)
                remote_led_index = 0;
            else if (remote_led_index < 0)
                remote_led_index = DELAY_COUNT - 1;
        }
    }
    else
    {
        // Remote is IDLE - turn off LED
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);
        last_remote_led_tick = HAL_GetTick();
        remote_led_index = 0;  // Reset local copy
    }
}
