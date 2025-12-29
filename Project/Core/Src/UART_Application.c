#include "UART_Application.h"
#include "LEDApplication.h"
#include "LedController.h"

extern UART_HandleTypeDef huart1;
extern uint8_t CurrentMode;

static uint8_t uart_rx_byte = 0;


volatile uint32_t uart_tx_count = 0;
volatile uint32_t uart_rx_count = 0;
volatile uint8_t last_rx_byte = 0;
void UART_AppInit(void)
{
    HAL_UART_Receive_IT(&huart1, &uart_rx_byte, 1);
}

void UART_SendEvent(uint8_t evt)
{

    HAL_StatusTypeDef status = HAL_UART_Transmit(&huart1, &evt, 1, 10);

    if (status == HAL_OK)
    {
        uart_tx_count++;
    }
}

void UART_OnByteReceived(void)
{
    // Debug counters
    uart_rx_count++;
    last_rx_byte = uart_rx_byte;


    switch(uart_rx_byte)
    {
        case EVT_SINGLE_CLICK:
            SetRemoteMode(1);
            break;

        case EVT_DOUBLE_CLICK:
            SetRemoteMode(2);
            break;

        case EVT_HOLD_START:
            SetRemoteMode(3);
            break;

        case EVT_HOLD_END:
            SetRemoteMode(0);
            break;

        default:
            break;
    }


        __HAL_UART_CLEAR_FLAG(&huart1, UART_CLEAR_FEF | UART_CLEAR_NEF | UART_CLEAR_OREF);
        HAL_UART_Receive_IT(&huart1, &uart_rx_byte, 1);
}
