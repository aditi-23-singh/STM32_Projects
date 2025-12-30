#ifndef BUTTON_CORE_H
#define BUTTON_CORE_H

#include "main.h"

typedef enum {
    BUTTON_EVENT_NONE = 0,
    BUTTON_EVENT_SINGLE_CLICK,
    BUTTON_EVENT_DOUBLE_CLICK,
    BUTTON_EVENT_HOLD_START,
    BUTTON_EVENT_HOLD_END
} BUTTON_EVENT_TYPEDEF_ENUM;

typedef struct {
    uint8_t btn_stable;
    uint8_t btn_prev;
    uint32_t btn_debounce;
    uint32_t btn_press_start;
    uint8_t btn_clicks;
    uint8_t btn_hold;
    uint32_t btn_click_timer;
    uint32_t btn_release_start;
    BUTTON_EVENT_TYPEDEF_ENUM pending_event;

    // Hardware info
    GPIO_TypeDef* GPIO_Port;
    uint16_t GPIO_Pin;

} Button_t;

#define HOLD_TIME_MS 175
#define CLICK_WINDOW_MS 150
#define DEBOUNCE_TIME_MS 10

void ButtonCore_Init(Button_t *btn, GPIO_TypeDef* Port, uint16_t Pin);
void ButtonCore_Update(Button_t *btn);
BUTTON_EVENT_TYPEDEF_ENUM GetEvent(Button_t *btn);

#endif
