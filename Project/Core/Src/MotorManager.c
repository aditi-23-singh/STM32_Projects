#include "MotorManager.h"
#include "Stepper.h"
#include <stdint.h>
#include "stm32f0xx.h"

const uint16_t DELAY_1_16 =100;
const uint16_t DELAY_1_256=10;
const uint16_t MOVE_DURATION_MS=2000;


static MotorState_t currentState=MOTOR_IDLE;
static uint32_t movementStartTime=0;
static uint32_t currentPulseDelay=100;

void MotorManager_Init(void)
{
	currentState= MOTOR_IDLE;
	Stepper_SetEnable(0);
}
void MotorManager_HandleEvent(EVENT_CODES_ENUM event)
{
	uint32_t now=HAL_GetTick();
	switch(event)
	{
	case EVT_SINGLE_CLICK:
	            currentPulseDelay = DELAY_1_16;
	            movementStartTime = now;
	            currentState = MOTOR_MOVING_TIMED;
	            Stepper_SetEnable(1);
	            break;
	case EVT_DOUBLE_CLICK:
	            currentPulseDelay = DELAY_1_256;
	            movementStartTime = now;
	            currentState = MOTOR_MOVING_TIMED;
	            Stepper_SetEnable(1);
	            break;
	case EVT_HOLD_START:
	            currentState = MOTOR_MOVING_HOLD;
	            currentPulseDelay = DELAY_1_16;
	            Stepper_SetEnable(1);
	            break;
	case EVT_HOLD_END:
	            currentState = MOTOR_IDLE;
	            Stepper_SetEnable(0);
	            break;

	}
}
void MotorManager_Update(void){
		if(currentState==MOTOR_MOVING_TIMED)
		{
			if(HAL_GetTick()-movementStartTime<MOVE_DURATION_MS)
			{
				//Stepper_Step(currentPulseDelay);
			}
			else{
				currentState=MOTOR_IDLE;
				Stepper_SetEnable(0);
			}
		}
		else if(currentState==MOTOR_MOVING_HOLD)
		{
			//Stepper_Step(currentPulseDelay);
		}
}
