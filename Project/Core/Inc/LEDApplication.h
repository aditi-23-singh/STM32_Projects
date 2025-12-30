#ifndef LEDAPPLICATION_H
#define LEDAPPLICATION_H

#include "main.h"
#include <stdint.h>

#define DELAY_COUNT 4

/* Function prototypes */
void UpdateLED(void);
void SetRemoteMode(EVENT_CODES_ENUM event);
void SetLocalMode(EVENT_CODES_ENUM event);
void LED_Init();

#endif /* LEDAPPLICATION_H */
