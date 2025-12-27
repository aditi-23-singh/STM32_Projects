#ifndef UART_APPLICATION_H
#define UART_APPLICATION_H

#include "main.h"
#include "usart.h"
#include <stdint.h>

/* Event codes */
#define EVT_SINGLE_CLICK  0x01
#define EVT_DOUBLE_CLICK  0x02
#define EVT_HOLD_START    0x03
#define EVT_HOLD_END      0x04

/* Function prototypes */
void UART_AppInit(void);
void UART_SendEvent(uint8_t evt);
void UART_OnByteReceived(void);

#endif /* UART_APPLICATION_H */
