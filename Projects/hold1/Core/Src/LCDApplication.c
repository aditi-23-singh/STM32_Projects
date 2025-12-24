#include "LCDApplication.h"
#include "LEDApplication.h"

#include "stm32f0xx_hal.h"
#include "stdio.h"

#define MODE_IDLE   0
#define MODE_SINGLE 1
#define MODE_DOUBLE 2
#define MODE_HOLD   3

uint8_t prev_mode = 255;
uint32_t prev_delay = 0;

uint8_t current_mode;

extern int8_t idx;
extern int8_t dir;

extern uint32_t active_delays[DELAY_COUNT];

extern uint8_t btn;
extern uint8_t last_btn_state;
extern uint32_t last_btn_tick;
extern uint8_t click_count;
extern uint32_t last_hold_tick;
extern uint8_t hold_active;

void HandleLCDApplication()
{
	  if (hold_active)
	      current_mode = MODE_HOLD;
	  else if (click_count == 2)
	      current_mode = MODE_DOUBLE;
	  else if (click_count == 1)
	      current_mode = MODE_SINGLE;
	  else
	      current_mode = MODE_IDLE;

	  char buffer[20];

	if (current_mode != prev_mode)
	{
	    LCD_SetCursor(0, 0);

	    switch (current_mode)
	    {
	        case MODE_HOLD:
	            LCD_Print("Mode: HOLD");
	            break;
	        case MODE_DOUBLE:
	            LCD_Print("Mode: DOUBLE ");
	            break;
	        case MODE_SINGLE:
	            LCD_Print("Mode: SINGLE ");
	            break;
	        default:
	            LCD_Print("Mode: IDLE");
	            break;
	    }

	    prev_mode = current_mode;
	}

	if (active_delays[idx] != prev_delay)
	{
	    LCD_SetCursor(1, 0);
	    sprintf(buffer, "Delay: %lu ms   ", active_delays[idx]);
	    LCD_Print(buffer);

	    prev_delay = active_delays[idx];
	}
}


