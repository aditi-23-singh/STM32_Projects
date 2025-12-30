#include "ButtonCore.h"

void ButtonCore_Init(Button_t *btn, GPIO_TypeDef* Port, uint16_t Pin) {



    btn->btn_stable = 0;
    btn->btn_prev = 0;
    btn->btn_debounce= 0;
    btn->btn_press_start = 0;
    btn->btn_clicks = 0;
    btn->btn_hold = 0;
    btn->btn_click_timer = 0;
    btn->pending_event = BUTTON_EVENT_NONE;
}


static void Button_Debounce(Button_t *btn,uint8_t raw,uint32_t now)
{
	if(raw != btn->btn_stable)
	{
		btn->btn_debounce++;
		if(btn->btn_debounce >= DEBOUNCE_TIME_MS )
		{
			btn->btn_stable = raw;
			btn->btn_debounce = 0;
		}
	}
	else
	{
		btn->btn_debounce = 0;
	}
}

void ButtonCore_Update(Button_t *btn)
{
    uint8_t btn_raw = (HAL_GPIO_ReadPin(Button_GPIO_Port, Button_Pin) == GPIO_PIN_SET) ? 1 : 0;
    uint32_t now = HAL_GetTick();

    Button_Debounce(btn,btn_raw, now);

    if (btn->btn_stable && !btn->btn_prev) {
        btn->btn_press_start = now;
        if (!btn->btn_hold)
        {
            btn->btn_clicks++;
            if (btn->btn_clicks > 2)
            	btn->btn_clicks = 2;
        }
        btn->btn_release_start = 0;
    }

    if (btn->btn_stable && !btn->btn_hold && (now - btn->btn_press_start >= HOLD_TIME_MS)) {
        btn->btn_hold = 1;
        btn->btn_clicks = 0;
        btn->pending_event = BUTTON_EVENT_HOLD_START;
    }

    if (!btn->btn_stable && btn->btn_prev) {
        if (btn->btn_hold)
        {
            btn->pending_event = BUTTON_EVENT_HOLD_END;
            btn->btn_hold = 0;
        }
        else
        {
            btn->btn_release_start = now;
        }
        btn->btn_press_start = 0;
    }

    if (!btn->btn_hold && !btn->btn_stable && (btn->btn_clicks > 0))
    {
        btn->btn_click_timer++;
        if (btn->btn_click_timer >= CLICK_WINDOW_MS)
        {
            if (btn->btn_clicks == 1)
            {
                btn->pending_event = BUTTON_EVENT_SINGLE_CLICK;
            } else if(btn->btn_clicks == 2)
            {
                btn->pending_event = BUTTON_EVENT_DOUBLE_CLICK;
            }

            btn->btn_clicks = 0;
            btn->btn_click_timer = 0;
        }
    }

    btn->btn_prev = btn->btn_stable;
}

BUTTON_EVENT_TYPEDEF_ENUM GetEvent(Button_t *btn)
{
	BUTTON_EVENT_TYPEDEF_ENUM ev = btn->pending_event;
	btn->pending_event = BUTTON_EVENT_NONE;
	return ev;
}
