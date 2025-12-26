/* Fixed UARTCommunication.h */
#ifndef UARTCOMMUNICATION_H
#define UARTCOMMUNICATION_H

#include <stdint.h>

// UART packet structure
typedef struct {
    uint8_t start_byte;   // 0xAA
    uint8_t mode;         // 0=IDLE, 1=SINGLE, 2=DOUBLE, 3=HOLD
    int8_t index;         // Current delay index
    int8_t direction;     // 1=forward, -1=backward
    uint8_t led_state;
    uint8_t checksum;     // Simple checksum
    uint8_t end_byte;     // 0x55
} UART_Packet_t;

/* Initialize UART communication */
void UART_Comm_Init(void);

/* Send current board state to remote board */
void UART_SendState(uint8_t mode, int8_t index, int8_t direction);

/* Process received UART data - ADD THIS DECLARATION */
void UART_ProcessReceivedData(void);

#endif /* UARTCOMMUNICATION_H */
