#include "Button.h"
#include "stm32f0xx_hal.h"
#include "stdio.h"

#define HOLD_THRESHOLD_MS   100    // Press longer than this = hold

uint8_t btn = 0;
uint8_t last_btn_state = 0;
uint32_t last_btn_tick = 0;
uint8_t click_count = 0;
uint32_t btn_press_tick = 0;
uint32_t last_hold_tick = 0;
uint8_t hold_active = 0;

void DetectButton()
{
	 	 btn = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);

	    /* Detect rising edge */

	    if (btn && !last_btn_state)
	    {
	        btn_press_tick = HAL_GetTick();
	        last_btn_tick = btn_press_tick;
	        if (click_count <= 2)
	            click_count++;
	    }

	    /* Detect falling edge (Button Release)  */
	    if (!btn && last_btn_state)
	    {
	        if (!hold_active)
	        {
	            /* Start click resolution timer on release */
	            last_btn_tick = HAL_GetTick();
	        }
	        hold_active = 0;
	    }

	    /* Hold Detection */
	    if (btn && !hold_active && (HAL_GetTick() - btn_press_tick >= HOLD_THRESHOLD_MS))
	    {
	        hold_active = 1;
	        click_count = 0;
	        last_hold_tick = HAL_GetTick();
	    }


	    last_btn_state = btn;
}

