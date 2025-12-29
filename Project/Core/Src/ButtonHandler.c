//#include "ButtonHandler.h"
//
//#define HOLD_TIME_MS     175
//#define CLICK_WINDOW_MS  150
//#define DEBOUNCE_TIME_MS 10
//
//extern uint8_t CurrentMode;
//
//// button states
//uint8_t btn_stable = 0;
//uint8_t btn_prev = 0;
//int32_t btn_debounce = 0;
//uint32_t btn_press_time = 0;
//uint32_t btn_click_timer = 0;
//uint8_t btn_clicks = 0;
//uint8_t btn_hold = 0;
//
//void UpdateButton(void)
//{
//
//    uint8_t btn_raw = (HAL_GPIO_ReadPin(Button_GPIO_Port, Button_Pin) == GPIO_PIN_SET) ? 1 : 0;
//
//
//    if (btn_raw != btn_stable)
//    {
//        btn_debounce++;
//        if (btn_debounce >= DEBOUNCE_TIME_MS)
//        {
//            btn_stable = btn_raw;
//            btn_debounce = 0;
//        }
//    }
//    else
//    {
//        btn_debounce = 0;
//    }
//
//
//    if (btn_stable && !btn_prev)
//    {
//        btn_press_time = 0;
//
//        if (btn_clicks < 2)
//            btn_clicks++;
//
//        btn_click_timer = 0;
//    }
//
//
//    if (btn_stable)
//    {
//        btn_press_time++;
//
//        if (!btn_hold && (btn_press_time >= HOLD_TIME_MS))
//        {
//            btn_hold = 1;
//            btn_clicks = 0;
//            btn_click_timer = 0;
//            CurrentMode = 3;
//            UART_SendEvent(EVT_HOLD_START);
//        }
//    }
//
//    if (!btn_stable && btn_prev)
//    {
//        btn_press_time = 0;
//
//        if (btn_hold)
//        {
//            CurrentMode = 0;
//            UART_SendEvent(EVT_HOLD_END);
//        }
//
//        btn_hold = 0;
//        btn_click_timer = 0;
//    }
//
//
//    if (!btn_hold && !btn_stable && (btn_clicks > 0))
//    {
//        btn_click_timer++;
//
//        if (btn_click_timer >= CLICK_WINDOW_MS)
//        {
//            if (btn_clicks == 1)
//            {
//                CurrentMode = 1;
//                UART_SendEvent(EVT_SINGLE_CLICK);
//            }
//            else
//            {
//                CurrentMode = 2;
//                UART_SendEvent(EVT_DOUBLE_CLICK);
//            }
//
//            btn_clicks = 0;
//            btn_click_timer = 0;
//        }
//    }
//
//    btn_prev = btn_stable;
//}
