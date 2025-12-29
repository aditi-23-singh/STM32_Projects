#include "LedController.h"

#define FAST_TOGGLE_MS 25U
#define SLOW_TOGGLE_MS 100U

void LedController_Init(LedController_t *ctrl)
{
	ctrl-> mode=LED_MODE_IDLE;
	ctrl->blinkTimer=0;
	ctrl->durationTimer=0;
	ctrl->ledState=0;

}

void LedController_SetMode(LedController_t *ctrl, LedMode_t mode)
{
	ctrl->mode=mode;
	ctrl->blinkTimer=0;
	ctrl->durationTimer=0;
	ctrl->ledState=0;

}

void LedController_Update(
		LedController_t *ctrl,
		LedOnOffFn setFn,
		LedToggleFn toggleFn){
	switch (ctrl->mode){
	case LED_MODE_IDLE:
		setFn(0);
	break;

	case LED_MODE_SINGLE_CLICK:
	ctrl->blinkTimer++;
	ctrl->durationTimer++;

	if (ctrl->blinkTimer >= FAST_TOGGLE_MS) {
		ctrl->blinkTimer = 0;
		toggleFn();
	}

	if (ctrl->durationTimer >= BLINK_DURATION_MS) {
		LedController_SetMode(ctrl, LED_MODE_IDLE);
		setFn(0);
	}
	break;

	case LED_MODE_DOUBLE_CLICK:
		ctrl->blinkTimer++;
		ctrl->durationTimer++;

	if (ctrl->blinkTimer >= SLOW_TOGGLE_MS) {
		ctrl->blinkTimer = 0;
		toggleFn();
	}

	if (ctrl->durationTimer >= BLINK_DURATION_MS) {
		LedController_SetMode(ctrl, LED_MODE_IDLE);
		setFn(0);
	}
	break;

	case LED_MODE_HOLD:
		setFn(1);
		break;

	default:
		setFn(0);
		break;
	}
}
