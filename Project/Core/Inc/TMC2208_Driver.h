#ifndef INC_TMC2208_DRIVER_H_
#define INC_TMC2208_DRIVER_H_

#include "main.h"
#define TMC_GCONF 0x00
#define TMC_GSTAT 0x01
#define TMC_IFCNT 0x02
#define TMC_VERSION 0x03
#define TMC_IOIN 0x06
#define TMC_IHOLD_IRUN 0x10
#define TMC_VACTUAL 0x22

void TMC2208_Init(UART_HandleTypeDef *huart);
uint32_t TMC2208_ReadRegister(uint8_t reg_addr,uint32_t *out_value);
void TMC2208_WriteRegister(uint8_t reg_addr,uint32_t data);

#endif /* INC_TMC2208_DRIVER_H_ */
