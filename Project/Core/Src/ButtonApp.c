#include "ButtonApp.h"
#include "ButtonCore.h"
#include "stm32f0xx_hal.h"
#include "UART_Application.h"

extern uint8_t CurrentMode;
static ButtonCore_t Button;
void ButtonApp_Init(void)
{
	ButtonCore_Init(&Button);
}
void ButtonApp_Update(void)
{
	uint8_t raw=(HAL_GPIO_ReadPin(Button_GPIO_Port, Button_Pin)==GPIO_PIN_SET);
	ButtonEvent_t event=ButtonCore_Update(&Button, raw);

	switch(event)
	{
	case BTN_EVENT_SINGLE_CLICK:
		//CurrentMode=0;
		CurrentMode=1;
		UART_SendEvent(EVT_SINGLE_CLICK);
		break;

	case BTN_EVENT_DOUBLE_CLICK:
		//CurrentMode=0;
		CurrentMode = 2;
		UART_SendEvent(EVT_DOUBLE_CLICK);
		break;

	 case BTN_EVENT_HOLD_START:
		// CurrentMode=0;
		CurrentMode = 3;
		UART_SendEvent(EVT_HOLD_START);
		break;

	case BTN_EVENT_HOLD_END:
		CurrentMode = 0;
		UART_SendEvent(EVT_HOLD_END);
		break;

	default:
		//HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);
	 //ResetLedTimers();
		break;
	    }
}
