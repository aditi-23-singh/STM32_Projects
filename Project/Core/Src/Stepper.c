//#include "Stepper.h"
//void Stepper_SetEnable(uint8_t enable)
//{
//	HAL_GPIO_WritePin(EN_GPIO_Port, EN_Pin, enable? GPIO_PIN_RESET: GPIO_PIN_SET);
//
//}
//void Stepper_Step(uint32_t pulse_delay)
//{
//	HAL_GPIO_WritePin(STEP_GPIO_Port, STEP_Pin, GPIO_PIN_SET);
//	for(volatile uint32_t i=0;i<pulse_delay;i++);
//	HAL_GPIO_WritePin(STEP_GPIO_Port, STEP_Pin, GPIO_PIN_RESET);
//	for(volatile uint32_t i=0;i<pulse_delay;i++);
//}
//
