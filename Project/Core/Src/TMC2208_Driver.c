#include "TMC2208_Driver.h"
#include <string.h>

#define TMC_SYNC_BYTE  0x05
#define TMC_READ_REQ_LEN 4
#define TMC_WRITE_REQ_LEN 8
#define TMC_REPLY_LEN 12

uint8_t debug_tx_status = 0;
uint8_t debug_rx_status = 0;
uint8_t debug_reply_sync = 0;
uint8_t debug_reply_addr = 0;
uint8_t debug_reply_reg = 0;
uint8_t debug_calc_crc = 0;
uint8_t debug_recv_crc = 0;
uint32_t debug_raw_value = 0;
bool debug_crc_match = false;
uint8_t debug_uart_state = 0;

uint8_t CRC1 = 0;
uint8_t RxDummy[12] = {0};
uint8_t TxBuffer[8];

static uint8_t CalcCRC(uint8_t *datagram, uint8_t len) {
    uint8_t crc = 0;
    for (int i = 0; i < len; i++) {
        uint8_t currentByte = datagram[i];
        for (int j = 0; j < 8; j++) {
            if ((crc >> 7) ^ (currentByte & 0x01)) {
                crc = (crc << 1) ^ 0x07;
            } else {
                crc = (crc << 1);
            }
            currentByte = currentByte >> 1;
        }
    }
    return crc;
}

void UART_FlushRx(UART_HandleTypeDef *huart)
{
    volatile uint32_t tmp;
    while (__HAL_UART_GET_FLAG(huart, UART_FLAG_RXNE))
    {
        tmp = huart->Instance->RDR;
        (void)tmp;
    }
}

void TMC_Init(TMC2208_t *driver, UART_HandleTypeDef *uart_handle, uint8_t slave_addr) {
    driver->uartHandle = uart_handle;
    driver->slaveAddress = slave_addr;
    driver->isCRCError = false;
    driver->shadow_gconf = 0;
    driver->shadow_ihold_irun = 0;
    driver->shadow_ioin = 0;
}

void TMC_WriteRegister(TMC2208_t *driver, uint8_t reg_addr, uint32_t data) {
    TxBuffer[0] = TMC_SYNC_BYTE;
    TxBuffer[1] = driver->slaveAddress;
    TxBuffer[2] = reg_addr | 0x80;
    TxBuffer[3] = (data >> 24) & 0xFF;
    TxBuffer[4] = (data >> 16) & 0xFF;
    TxBuffer[5] = (data >> 8) & 0xFF;
    TxBuffer[6] = data & 0xFF;
    TxBuffer[7] = CalcCRC(TxBuffer, 7);

    UART_FlushRx(driver->uartHandle);

    debug_tx_status = HAL_UART_Transmit(driver->uartHandle, TxBuffer, 8, 100);

    uint8_t dummy[8];
    debug_rx_status = HAL_UART_Receive(driver->uartHandle, dummy, 8, 50);
}

bool TMC2208_SyncUART(TMC2208_t *driver)
{
    if (driver == NULL || driver->uartHandle == NULL)
        return false;

    uint32_t dummy;

    UART_FlushRx(driver->uartHandle);
    HAL_Delay(10);

    TMC_ReadRegister(driver, TMC2208_GCONF, &dummy);
    HAL_Delay(10);

    UART_FlushRx(driver->uartHandle);

    return true;
}

bool TMC_ReadRegister(TMC2208_t *driver, uint8_t reg_addr, uint32_t *value) {
    if (driver == NULL || value == NULL) {
        return false;
    }

    uint8_t txBuffer[4];
    uint8_t RxBuffer[12] = {0};


    txBuffer[0] = TMC_SYNC_BYTE;
    txBuffer[1] = driver->slaveAddress;
    txBuffer[2] = reg_addr;
    txBuffer[3] = CalcCRC(txBuffer, 3);

    UART_FlushRx(driver->uartHandle);
    __HAL_UART_FLUSH_DRREGISTER(driver->uartHandle);


    debug_uart_state = driver->uartHandle->gState;

    HAL_StatusTypeDef tx_stat = HAL_UART_Transmit(driver->uartHandle, txBuffer, 4, 100);
    debug_tx_status = tx_stat;

    if (tx_stat != HAL_OK) {
        return false;
    }

    HAL_Delay(2);

    HAL_StatusTypeDef rx_stat = HAL_UART_Receive(driver->uartHandle, RxBuffer, 12, 100);
    debug_rx_status = rx_stat;

    if (rx_stat != HAL_OK) {
        return false;
    }


    for (int i = 0; i < 12; i++) {
        RxDummy[i] = RxBuffer[i];
    }

    uint8_t *reply = &RxBuffer[4];


    debug_reply_sync = reply[0];
    debug_reply_addr = reply[1];
    debug_reply_reg = reply[2];


    if (reply[0] != TMC_SYNC_BYTE) {
        return false;
    }


    if (reply[1] != 0xFF) {
        return false;
    }


    if (reply[2] != reg_addr) {
        return false;
    }


    debug_raw_value = ((uint32_t)reply[3] << 24) |
                      ((uint32_t)reply[4] << 16) |
                      ((uint32_t)reply[5] << 8)  |
                      ((uint32_t)reply[6]);


    debug_calc_crc = CalcCRC(reply, 7);
    debug_recv_crc = reply[7];
    debug_crc_match = (debug_calc_crc == debug_recv_crc);

    if (!debug_crc_match) {
        driver->isCRCError = true;
        return false;
    }

    driver->isCRCError = false;
    *value = debug_raw_value;

    return true;
}
