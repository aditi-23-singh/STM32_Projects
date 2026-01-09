#include "TMC2208_Driver.h"
#include <string.h>
#define TMC_SYNC_BYTE  0x05
#define TMC_READ_REQ_LEN 4
#define TMC_WRITE_REQ_LEN 8
#define TMC_REPLY_LEN 8


uint8_t crc1=0;
uint8_t rxBuffer[12]={0};
uint8_t txBuffer[8];
static uint8_t calcCRC(uint8_t *datagram, uint8_t len) {
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
        crc1=crc;
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

static void clearRXBuffer(TMC2208_t *driver, uint16_t bytesToClear) {

    uint8_t dummy[12];
    if(bytesToClear > 12) bytesToClear = 12;
    HAL_UART_Receive(driver->uartHandle,  dummy,bytesToClear,10);

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


    txBuffer[0] = TMC_SYNC_BYTE;
    txBuffer[1] = driver->slaveAddress;
    txBuffer[2] = reg_addr | 0x80;
    txBuffer[3] = (data >> 24) & 0xFF;
    txBuffer[4] = (data >> 16) & 0xFF;
    txBuffer[5] = (data >> 8) & 0xFF;
    txBuffer[6] = data & 0xFF;
    txBuffer[7] = calcCRC(txBuffer, 7);

    HAL_UART_Transmit(driver->uartHandle, txBuffer, 8, 100);


    clearRXBuffer(driver, 12);
}
bool TMC2208_SyncUART(TMC2208_t *driver)
{
    if (driver == NULL || driver->uartHandle == NULL)
        return false;

    uint32_t dummy;
    UART_FlushRx(driver->uartHandle);
    TMC_ReadRegister(driver, TMC2208_GCONF, &dummy);
    HAL_Delay(2);
    UART_FlushRx(driver->uartHandle);

    return true;
}

bool TMC_ReadRegister(TMC2208_t *driver, uint8_t reg_addr, uint32_t *value) {
    uint8_t txBuffer[8];


    txBuffer[0] = 0x05;
    txBuffer[1] = driver->slaveAddress;
    txBuffer[2] = reg_addr;
    txBuffer[3] = calcCRC(txBuffer, 3);

    HAL_UART_AbortReceive_IT(driver->uartHandle);
    __HAL_UART_CLEAR_FLAG(driver->uartHandle, UART_CLEAR_OREF);

    if (HAL_UART_Receive_IT(driver->uartHandle, rxBuffer, 12) != HAL_OK) {
          return false;
    }

	HAL_UART_Transmit(driver->uartHandle, txBuffer, 4, 100);
    __HAL_UART_CLEAR_FLAG(driver->uartHandle, UART_CLEAR_OREF);
    if (rxBuffer[4] != 0x05) {

            return false;
    }
    uint8_t calculatedCRC = calcCRC(&rxBuffer[4],7);
    if (calculatedCRC != rxBuffer[11]) {
        driver->isCRCError = true;
        return false;
    }

    driver->isCRCError = false;
    *value = (rxBuffer[7] << 24) | (rxBuffer[8] << 16) | (rxBuffer[9] << 8) | rxBuffer[10];

    return true;
}
