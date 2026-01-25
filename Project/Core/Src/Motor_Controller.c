#include "Motor_Controller.h"
#include "gpio.h"
#include "core_cm0.h"

MotorController_t MotorControl = {0};

void Delay_us(uint32_t us)
{
	uint32_t cycles = us*(SystemCoreClock / 1000000UL) / 4U ;
	volatile uint32_t i;
	for(i = 0; i< cycles; ++i);

}

void Motor_Init(void)
{
	HAL_GPIO_WritePin(EN_GPIO_Port, EN_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(DIR_GPIO_Port, DIR_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(STEP_GPIO_Port, STEP_Pin, GPIO_PIN_RESET);

	MotorControl.running = 0;
    MotorControl.steps_remaining = 0;
    MotorControl.accumulator_us = 0;
	MotorControl.period_us = 0;
	MotorControl.direction = 0;

}

void Motor_Update(void)
{
	if(!MotorControl.running)
		return;

	MotorControl.accumulator_us += 1000;

	while(MotorControl.accumulator_us >= MotorControl.period_us && MotorControl.running)
	{
		HAL_GPIO_WritePin(DIR_GPIO_Port, DIR_Pin, (MotorControl.direction ? GPIO_PIN_SET : GPIO_PIN_RESET));

		HAL_GPIO_WritePin(STEP_GPIO_Port, STEP_Pin, GPIO_PIN_SET);
		Delay_us(STEP_PULSE_WIDTH_US);
		HAL_GPIO_WritePin(STEP_GPIO_Port, STEP_Pin, GPIO_PIN_RESET);

		MotorControl.accumulator_us -= MotorControl.period_us;

		if(MotorControl.steps_remaining > 0)
		{
			MotorControl.steps_remaining--;
			if(MotorControl.steps_remaining == 0)
				MotorControl.running = 0;

		}
	}
}
