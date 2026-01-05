#include "TMC2208_Driver.h"

#define TMC_SYNC_BYTE  0x05
#define TMC_READ_REQ_LEN 4
#define TMC_WRITE_REQ_LEN 8
#define TMC_REPLY_LEN 8


uint8_t crc=0;
uint8_t rxBuffer[12]={0};
uint8_t txBuffer[8];
static uint8_t calcCRC(uint8_t *datagram, uint8_t len) {
    crc = 0;
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

static void clearRXBuffer(TMC2208_t *driver, uint16_t bytesToClear) {
    // Because of the 1-wire resistor setup, every TX byte is echoed to RX.
    // We must read them out to clear the ORE (Overrun) or buffer.
    uint8_t dummy[12];
    if(bytesToClear > 12) bytesToClear = 12; // Safety cap
    HAL_UART_Receive(driver->uartHandle,  dummy,bytesToClear,10);
}

// --- Public Implementation ---

void TMC_Init(TMC2208_t *driver, UART_HandleTypeDef *uart_handle, uint8_t slave_addr) {
    driver->uartHandle = uart_handle;
    driver->slaveAddress = slave_addr;
    driver->isCRCError = false;
}

void TMC_WriteRegister(TMC2208_t *driver, uint8_t reg_addr, uint32_t data) {


    txBuffer[0] = TMC_SYNC_BYTE;
    txBuffer[1] = driver->slaveAddress;
    txBuffer[2] = reg_addr | 0x80; // Set MSB for Write Access
    txBuffer[3] = (data >> 24) & 0xFF;
    txBuffer[4] = (data >> 16) & 0xFF;
    txBuffer[5] = (data >> 8) & 0xFF;
    txBuffer[6] = data & 0xFF;
    txBuffer[7] = calcCRC(txBuffer, 7);

    // Transmit Write Command
    HAL_UART_Transmit(driver->uartHandle, txBuffer, 8, 100);

    // HARDWARE QUIRK HANDLING:
    // Since TX is connected to RX via resistor, we just received our own 8 bytes.
    // We must consume them so they don't block the next Read.
    clearRXBuffer(driver, 8);
}

bool TMC_ReadRegister(TMC2208_t *driver, uint8_t reg_addr, uint32_t *value) {
    uint8_t txBuffer[8];
    // We only need 8 bytes now, because we will "flush" the 4 echo bytes by clearing the error


    txBuffer[0] = 0x05; // Sync
    txBuffer[1] = driver->slaveAddress;
    txBuffer[2] = reg_addr;
    txBuffer[3] = calcCRC(txBuffer, 3);

    // 1. Send the request (Blocking)
    // During this time, the echo arrives and causes an ORE (Overrun) error.

    HAL_UART_AbortReceive_IT(driver->uartHandle);

    if (HAL_UART_Receive_IT(driver->uartHandle, rxBuffer, 12) != HAL_OK) {
          return false;
    }

	HAL_UART_Transmit(driver->uartHandle, txBuffer, 4, 100);

    // 2. CRITICAL FIX: Clear the Overrun Flag (ORE)
    // This acknowledges the "lost" echo bytes and unlocks the UART for the real reply.
    __HAL_UART_CLEAR_FLAG(driver->uartHandle, UART_CLEAR_OREF);

    // 3. Receive only the Response (8 Bytes)
    // We expect the TMC2208 to reply now.
   // Timeout or Error

    if (rxBuffer[4] != 0x05) {
            // If we read 0x00, the wire is likely broken/grounded.
            return false;
    }

    // 4. Verify CRC (The reply is now at index 0, not 4)
    uint8_t calculatedCRC = calcCRC(&rxBuffer[4],7);
    if (calculatedCRC != rxBuffer[11]) {
        driver->isCRCError = true;
        return false;
    }

    driver->isCRCError = false;

    // 5. Extract Data
    *value = (rxBuffer[7] << 24) | (rxBuffer[8] << 16) | (rxBuffer[9] << 8) | rxBuffer[10];

    return true;
}
