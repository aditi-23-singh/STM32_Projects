#include "LCDApplication.h"
#include "LEDApplication.h"
#include "gpio.h"
#include "stm32f0xx_hal.h"
#include "stdio.h"

#define MODE_IDLE   0
#define MODE_SINGLE 1
#define MODE_DOUBLE 2
#define MODE_HOLD   3

uint8_t prev_mode = 255;
uint32_t prev_delay = 0;

uint8_t current_mode;

extern int8_t index;
extern int8_t direction;

extern uint32_t active_delays[DELAY_COUNT];

extern uint8_t btn;
extern uint8_t last_btn_state;
extern uint32_t last_btn_tick;
extern uint8_t click_count;
extern uint32_t last_hold_tick;
extern uint8_t hold_active;

void HandleLCDApplication()
{
	 /* Determine current mode */
	    if (hold_active)
	        current_mode = MODE_HOLD;
	    else if (click_count == 2)
	        current_mode = MODE_DOUBLE;
	    else if (click_count == 1)
	        current_mode = MODE_SINGLE;
	    else
	        current_mode = MODE_IDLE;

	    char buffer[20];

	    /* Update Mode Display */
	    if (current_mode != prev_mode)
	    {
	        LCD_SetCursor(0, 0);

	        switch (current_mode)
	        {
	            case MODE_HOLD:
	                LCD_Print("Mode: HOLD      ");
	                break;
	            case MODE_DOUBLE:
	                LCD_Print("Mode: DOUBLE    ");
	                break;
	            case MODE_SINGLE:
	                LCD_Print("Mode: SINGLE    ");
	                break;
	            default:
	                LCD_Print("Mode: IDLE      ");
	                break;
	        }

	        prev_mode = current_mode;
	    }

	    /* Update Delay Display  */
	    if (active_delays[index] != prev_delay)
	    {
	        LCD_SetCursor(1, 0);
	        sprintf(buffer, "Delay: %lu ms   ", active_delays[index]);
	        LCD_Print(buffer);

	        prev_delay = active_delays[index];
	    }
}


