#include "ButtonCore.h"

void ButtonCore_Init(ButtonCore_t *btn)
{
	btn->stable=0;
	btn->prev=0;
	btn->hold=0;
	btn->clicks=0;

	btn->debounceTimer=0;
	btn->pressTimer=0;
	btn->clickTimer=0;
}
ButtonEvent_t ButtonCore_Update(ButtonCore_t *btn, uint8_t rawState)
{
	ButtonEvent_t event =BTN_EVENT_NONE;
	if(rawState!=btn->stable)
	{
		btn->debounceTimer++;
		if(btn->debounceTimer >= DEBOUNCE_TIME_MS)
		{
			btn->stable=rawState;
			btn->debounceTimer=0;
		}
	}
	else
	{
		btn->debounceTimer=0;
	}
	//rising edge

	if(btn->stable && !btn->prev)
	{
		btn->pressTimer=0;
		btn->clickTimer=0;
		if(btn->clicks<2)
			btn->clicks++;
	}
	//hold
	if(btn->stable)
	{
		btn->pressTimer++;
		if(!btn->hold && btn->pressTimer >=HOLD_TIME_MS)
		{
			btn->hold=1;
			btn->clicks=0;
			event= BTN_EVENT_HOLD_START;
		}
	}
	//falling edge
	if(!btn->stable && btn->prev)
	{
		//
		if(btn->hold)
		{
			btn->hold=0;
			event =BTN_EVENT_HOLD_END;
		}
		btn->pressTimer=0;
	}
	//CLICK DECISION
	if(!btn->hold && !btn->stable && btn->clicks >0)
	{
		btn-> clickTimer++;
		if(btn->clickTimer>=CLICK_WINDOW_MS)
		{
			event=(btn->clicks==1)?BTN_EVENT_SINGLE_CLICK:BTN_EVENT_DOUBLE_CLICK;
			btn->clicks=0;
			btn->clickTimer=0;
		}
	}
	btn->prev=btn->stable;
	return event;
}

