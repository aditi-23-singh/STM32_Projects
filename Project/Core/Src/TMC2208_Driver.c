#include "TMC2208_Driver.h"

static UART_HandleTypeDef *tmc_huart;
uint8_t raw_tmc_reply[8];
uint8_t echo[8];
void TMC2208_Init(UART_HandleTypeDef *huart)
{
	tmc_huart=huart;
}
uint8_t TMC_CalcCRC(uint8_t *data, uint8_t len)
{
	uint8_t crc=0;
	for(uint8_t i=0;i<len;i++)
	{
		uint8_t currentByte=data[i];
		for(uint8_t j=0;j<8;j++)
		{
			if((crc>>7)^(currentByte &0x01)){
				crc=(crc<<1)^0x07;

			}else{
				crc=(crc<<1);
			}
			currentByte>>=1;
		}

	}
	return crc;
}


uint32_t TMC2208_ReadRegister(uint8_t reg_addr, uint32_t *out_value)
{
    uint8_t request[4];
    uint8_t response[8] = {0};

    request[0] = 0x05;
    request[1] = 0x00;
    request[2] = reg_addr;
    request[3] = TMC_CalcCRC(request, 3);

    if (HAL_UART_Transmit(tmc_huart, request, 4, 100) != HAL_OK)
        return 0;

    while (__HAL_UART_GET_FLAG(tmc_huart, UART_FLAG_TC) == RESET);

HAL_Delay(1);
    if (HAL_UART_Receive(tmc_huart, response, 8, 200) != HAL_OK)
        return 0;

    for (int i = 0; i < 8; i++)
        raw_tmc_reply[i] = response[i];

    if (response[0] != 0x05)
        return 0;

    if (response[2] != reg_addr)
        return 0;

    if (TMC_CalcCRC(response, 7) != response[7])
        return 0;   // CRC fail

    *out_value =
        ((uint32_t)response[3] << 24) |
        ((uint32_t)response[4] << 16) |
        ((uint32_t)response[5] << 8)  |
        ((uint32_t)response[6]);

    return 1;
}

void TMC2208_WriteRegister(uint8_t reg_addr, uint32_t data) {
    uint8_t msg[8];
    msg[0] = 0x05;
    msg[1] = 0x00;
    msg[2] = reg_addr | 0x80;
    msg[3] = (data >> 24) & 0xFF;
    msg[4] = (data >> 16) & 0xFF;
    msg[5] = (data >> 8) & 0xFF;
    msg[6] = data & 0xFF;
    msg[7] = TMC_CalcCRC(msg, 7);

    HAL_UART_Transmit(tmc_huart, msg, 8, 100);

}
