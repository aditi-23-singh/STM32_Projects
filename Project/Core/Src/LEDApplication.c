#include "LEDApplication.h"
#include "LedController.h"
#include "stm32f0xx_hal.h"

extern uint8_t CurrentMode;
static LedController_t LocalLed;
static LedController_t RemoteLed;

static void LD3_Set(uint8_t state)
{
	HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, state?GPIO_PIN_SET: GPIO_PIN_RESET);

}
static void LD3_Toggle(void)
{
	HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
}
static void LD4_Set(uint8_t state)
{
	HAL_GPIO_WritePin(LD4_GPIO_Port, LD4_Pin, state? GPIO_PIN_SET:GPIO_PIN_RESET);

}
static void LD4_Toggle(void)
{
	HAL_GPIO_TogglePin(LD4_GPIO_Port,LD4_Pin);

}
void LEDApplication_Init(void)
{
	LedController_Init(&LocalLed);
	LedController_Init(&RemoteLed);
}

void UpdateLED(void)
{
	static uint8_t prevMode=0;
	if(CurrentMode!=prevMode)
	{
		LedController_SetMode(&LocalLed, (LedMode_t)CurrentMode);
		prevMode=CurrentMode;
	}

    LedController_Update(&LocalLed, LD3_Set, LD3_Toggle);
    LedController_Update(&RemoteLed, LD4_Set, LD4_Toggle);
}
void SetRemoteMode(uint8_t mode)
{
	LedController_SetMode(&RemoteLed, (LedMode_t)mode);
}


