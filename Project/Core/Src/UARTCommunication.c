#include "UARTCommunication.h"
#include "usart.h"
#include "stm32f0xx_hal.h"
#include <string.h>

// Remote board state variables
uint8_t remote_mode = 0;
int8_t remote_index = 0;
int8_t remote_direction = 1;
uint32_t remote_delays[4] = {250, 500, 1000, 2000};
uint8_t remote_led_physical_state = 0;
// Reception buffer
static uint8_t rx_buffer[sizeof(UART_Packet_t)];
static volatile uint8_t rx_complete = 0;

// External UART handle (from usart.c)
extern UART_HandleTypeDef huart1;

/**
 * @brief Initialize UART communication
 */
void UART_Comm_Init(void)
{
    // Start receiving data via interrupt
    HAL_UART_Receive_IT(&huart1, rx_buffer, sizeof(UART_Packet_t));
}

/**
 * @brief Calculate simple checksum
 */
static uint8_t Calculate_Checksum(uint8_t mode, int8_t index, int8_t direction)
{
    return (uint8_t)(mode + index + direction);
}

/**
 * @brief Send state to remote board
 */
void UART_SendState(uint8_t mode, int8_t index, int8_t direction)
{
    UART_Packet_t packet;

    packet.start_byte = 0xAA;
    packet.mode = mode;
    packet.index = index;
    packet.direction = direction;
    packet.checksum = Calculate_Checksum(mode, index, direction);
    packet.end_byte = 0x55;

    HAL_UART_Transmit(&huart1, (uint8_t*)&packet, sizeof(UART_Packet_t), 100);
}

/**
 * @brief Process received data from remote board
 */
void UART_ProcessReceivedData(void)
{
    if (rx_complete)
    {
        UART_Packet_t *packet = (UART_Packet_t*)rx_buffer;

        // Validate packet
        if (packet->start_byte == 0xAA &&
            packet->end_byte == 0x55 &&
            packet->checksum == Calculate_Checksum(packet->mode, packet->index, packet->direction))
        {
            // Update remote board state
            remote_mode = packet->mode;
            remote_index = packet->index;
            remote_direction = packet->direction;

            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, packet->led_state ? GPIO_PIN_SET : GPIO_PIN_RESET);
            // Update remote delays based on mode
            if (packet->mode == 3)  // HOLD mode
            {
                remote_delays[0] = 125;
                remote_delays[1] = 250;
                remote_delays[2] = 500;
                remote_delays[3] = 1000;
            }
            else
            {
                remote_delays[0] = 250;
                remote_delays[1] = 500;
                remote_delays[2] = 1000;
                remote_delays[3] = 2000;
            }
        }

        rx_complete = 0;

        // Restart reception
        HAL_UART_Receive_IT(&huart1, rx_buffer, sizeof(UART_Packet_t));
    }
}

/**
 * @brief UART Receive Complete Callback
 * This function is called by HAL when UART reception is complete
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
        rx_complete = 1;
    }
}
