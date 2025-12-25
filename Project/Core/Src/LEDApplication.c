#include <LEDApplication.h>

#include "stm32f0xx_hal.h"
#include "stdio.h"

uint32_t delays[] =
{ 250, 500, 1000, 2000 };
uint32_t active_delays[DELAY_COUNT];

int8_t index = 0;
int8_t direction = 1;

uint32_t last_led_tick = 0;

extern uint8_t btn;
extern uint8_t last_btn_state;
extern uint32_t last_btn_tick;
extern uint8_t click_count;
extern uint32_t last_hold_tick;
extern uint8_t hold_active;

void LEDInit()
{
	for (int i = 0; i < DELAY_COUNT; i++) {
		active_delays[i] = delays[i];
	}
}

void HandleLEDApplication()
{
	 /* LED BLINK LOGIC */
	    if (!hold_active && (HAL_GetTick() - last_led_tick >= active_delays[index]))
	    {
	        last_led_tick = HAL_GetTick();
	        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_8);


	        index += direction;

	        if (index >= DELAY_COUNT)
	            index = 0;
	        else if (index < 0)
	            index = DELAY_COUNT - 1;
	    }

	    /* Hold Mode - Halve delays ONCE when entering */
	    if (hold_active && (active_delays[0] == delays[0]))
	    {
	        // Only halve if not already halved
	        for (int i = 0; i < DELAY_COUNT; i++)
	            active_delays[i] = delays[i] / 2;
	    }

	    /* Restore delays when hold released */
	    if (!hold_active && (active_delays[0] != delays[0]))
	    {
	        // Only restore if currently halved
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
}
