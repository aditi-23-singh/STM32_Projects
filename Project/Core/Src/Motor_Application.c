#include "Motor_Application.h"
#include "Motor_Controller.h"
#include "gpio.h"
#include <stdio.h>
#include "string.h"

extern MotorController_t MotorControl;

#define STEPS_PER_MOVE 200
#define PERIOD_MEDIUM_US 2000
#define PERIOD_SLOW_US 5000
#define PERIOD_FAST_US 1000

void MotorApplication_Init(void) {

	Motor_Init();
}

void MotorApplication_SetMotorMode(EVENT_CODES_ENUM event) {
    switch (event) {
        case EVT_SINGLE_CLICK:
        	MotorControl.direction = -1;
        	MotorControl.period_us = PERIOD_MEDIUM_US;
        	MotorControl.steps_remaining = STEPS_PER_MOVE;
        	MotorControl.running = 1;
            break;

        case EVT_DOUBLE_CLICK:
        	MotorControl.direction = 1;
        	MotorControl.period_us = PERIOD_SLOW_US;
        	MotorControl.steps_remaining = STEPS_PER_MOVE;
        	MotorControl.running = 1;
            break;

        case EVT_HOLD_START:
        	MotorControl.direction = -1;
        	MotorControl.period_us = PERIOD_MEDIUM_US;
        	MotorControl.steps_remaining = 0;
        	MotorControl.running = 1;
            break;

        case EVT_FACET_CW:
                   MotorControl.direction = 1;
                   MotorControl.period_us = PERIOD_FAST_US;
                   MotorControl.steps_remaining = 0;
                   MotorControl.running = 1;
                   break;

        case EVT_FACET_CCW:
                   MotorControl.direction = -1;
                   MotorControl.period_us = PERIOD_FAST_US;
                   MotorControl.steps_remaining = 0;
                   MotorControl.running = 1;
                   break;
        case EVT_GRAIN_CW:
                  MotorControl.direction = 1;
                  MotorControl.period_us = PERIOD_FAST_US;
                  MotorControl.steps_remaining = 0;
                  MotorControl.running = 1;
                  break;

         case EVT_GRAIN_CCW:
                  MotorControl.direction = -1;
                  MotorControl.period_us = PERIOD_FAST_US;
                  MotorControl.steps_remaining = 0;
                  MotorControl.running = 1;
                  break;

        case EVT_HOLD_END:
        	MotorControl.running = 0;
        	MotorControl.accumulator_us = 0;
            break;

    }

}

void MotorApplication_Update(void) {

	Motor_Update();
}
