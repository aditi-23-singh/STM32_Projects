#ifndef LED_CONTROLLER_H
#define LED_CONTROLLER_H

#include <stdint.h>
#define BLINK_DURATION_MS 1000U

typedef enum
{
	LED_MODE_IDLE=0,
	LED_MODE_SINGLE_CLICK,
	LED_MODE_DOUBLE_CLICK,
	LED_MODE_HOLD
} LedMode_t;

typedef struct
{
	LedMode_t mode;
	uint32_t blinkTimer;
	uint32_t durationTimer;
	uint8_t ledState;
} LedController_t;

typedef void (*LedOnOffFn)(uint8_t state);
typedef void (*LedToggleFn)(void);

void LedController_Init(LedController_t *ctrl);
void LedController_SetMode(LedController_t *ctrl, LedMode_t mode);
void LedController_Update(
		LedController_t *ctrl,
		LedOnOffFn setFn,
		LedToggleFn toggleFn
);
#endif
