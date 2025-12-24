#include <LEDApplication.h>

#include "stm32f0xx_hal.h"
#include "stdio.h"

uint32_t delays[] =
{ 250, 500, 1000, 2000 };
uint32_t active_delays[DELAY_COUNT];

int8_t idx = 0;
int8_t dir = 1;

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
		active_delays[i] = delays[i];  // start with normal delays
	}
}

void HandleLEDApplication()
{
	if (!hold_active && (HAL_GetTick() - last_led_tick >= active_delays[idx]))

	{
		last_led_tick = HAL_GetTick();
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_8);

		/* AUTO move to next delay */
		idx += dir;

		if (idx >= DELAY_COUNT)
			idx = 0;
		else if (idx < 0)
			idx = DELAY_COUNT - 1;
	}

	/*  Button Release Detection */
	if (!btn && last_btn_state) {
		if (hold_active) {
			// Restore original delays
			for (int i = 0; i < DELAY_COUNT; i++)
				active_delays[i] = delays[i];
		}
		if (!hold_active) {
			/* Wait for single/double click resolution */
			last_btn_tick = HAL_GetTick();
		}
		hold_active = 0;

	}

	if (hold_active) {
		for (int i = 0; i < DELAY_COUNT; i++)
			active_delays[i] = delays[i] / 2;
	}

	/* Hold Action (Fast Cycling)  */
	if (hold_active && (HAL_GetTick() - last_hold_tick >= active_delays[idx])) {

		// HALVE the delays while holding

		last_hold_tick = HAL_GetTick();
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_8);

		idx += dir;
		if (idx >= DELAY_COUNT)
			idx = 0;
		else if (idx < 0)
			idx = DELAY_COUNT - 1;
	}

	/* Click Resolution */
	if (!hold_active && click_count > 0 && (HAL_GetTick() - last_btn_tick > 50)) {
		if (click_count == 1) {
			idx += dir;
			if (idx >= DELAY_COUNT)
				idx = 0;
			else if (idx < 0)
				idx = DELAY_COUNT - 1;
		}
		else {
			dir = -dir;
		}

		click_count = 0;
	}

	last_btn_state = btn;
}
