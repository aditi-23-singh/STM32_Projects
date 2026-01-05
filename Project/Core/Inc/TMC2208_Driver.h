#ifndef TMC2208_DRIVER_H
#define TMC2208_DRIVER_H

#include "stm32f0xx_hal.h"
#include <stdint.h>
#include <stdbool.h>

// --- Register Addresses (Common) ---
#define TMC2208_GCONF      0x00
#define TMC2208_GSTAT      0x01
#define TMC2208_IFCNT      0x02
#define TMC2208_OTP_PROG   0x04
#define TMC2208_OTP_READ   0x05
#define TMC2208_IOIN       0x06
#define TMC2208_FACTORY_CONF 0x07
#define TMC2208_IHOLD_IRUN 0x10
#define TMC2208_TPOWERDOWN 0x11
#define TMC2208_TSTEP      0x12
#define TMC2208_TPWMTHRS   0x13
#define TMC2208_VACTUAL    0x22
#define TMC2208_CHOPCONF   0x6C
#define TMC2208_PWMCONF    0x70


typedef struct {
    UART_HandleTypeDef *uartHandle; // Pointer to the HAL UART handle
    uint8_t slaveAddress;           // TMC2208 Slave Address (usually 0-3)
    bool isCRCError;                // Status flag for last transaction
} TMC2208_t;


void TMC_Init(TMC2208_t *driver, UART_HandleTypeDef *uart_handle, uint8_t slave_addr);


void TMC_WriteRegister(TMC2208_t *driver, uint8_t reg_addr, uint32_t data);

bool TMC_ReadRegister(TMC2208_t *driver, uint8_t reg_addr, uint32_t *value);

#endif // TMC2208_DRIVER_H
