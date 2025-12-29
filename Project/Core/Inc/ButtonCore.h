#ifndef BUTTON_CORE_H
#define BUTTON_CORE_H

#include <stdint.h>

#define HOLD_TIME_MS 175U
#define CLICK_WINDOW_MS 150U
#define DEBOUNCE_TIME_MS 10U

typedef enum{
	BTN_EVENT_NONE=0,
	BTN_EVENT_SINGLE_CLICK,
	BTN_EVENT_DOUBLE_CLICK,
	BTN_EVENT_HOLD_START,
	BTN_EVENT_HOLD_END,
	BTN_EVENT_HOLD
} ButtonEvent_t;
typedef struct{
	uint8_t stable;
	uint8_t prev;
	uint8_t hold;
	uint8_t clicks;

	uint32_t debounceTimer;
	uint32_t pressTimer;
	uint32_t clickTimer;
} ButtonCore_t;

void ButtonCore_Init(ButtonCore_t *btn);
ButtonEvent_t ButtonCore_Update(ButtonCore_t *btn,uint8_t rawState);
#endif
