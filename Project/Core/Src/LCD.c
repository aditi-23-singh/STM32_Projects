#include "LCD.h"

void LCD_EnablePulse(void)
{
    HAL_GPIO_WritePin(E_GPIO_Port, E_Pin, GPIO_PIN_SET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(E_GPIO_Port, E_Pin, GPIO_PIN_RESET);
    HAL_Delay(1);
}

void LCD_Send4Bit(uint8_t data)
{
    HAL_GPIO_WritePin(D4_GPIO_Port, D4_Pin, (data & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(D5_GPIO_Port, D5_Pin, (data & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(D6_GPIO_Port, D6_Pin, (data & 0x04) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(D7_GPIO_Port, D7_Pin, (data & 0x08) ? GPIO_PIN_SET : GPIO_PIN_RESET);

    LCD_EnablePulse();
}

void LCD_Command(uint8_t cmd)
{
    HAL_GPIO_WritePin(RS_GPIO_Port, RS_Pin, GPIO_PIN_RESET);

    LCD_Send4Bit(cmd >> 4);
    LCD_Send4Bit(cmd & 0x0F);

    HAL_Delay(2);
}

void LCD_Data(uint8_t data)
{
    HAL_GPIO_WritePin(RS_GPIO_Port, RS_Pin, GPIO_PIN_SET);

    LCD_Send4Bit(data >> 4);
    LCD_Send4Bit(data & 0x0F);

    HAL_Delay(2);
}
