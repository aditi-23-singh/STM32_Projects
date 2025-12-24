#include "LCD.h"

#include "gpio.h"
#include "stm32f0xx_hal.h"
#include "stdio.h"

void LCD_EnablePulse(void)
{
    HAL_GPIO_WritePin(LCD_EN_GPIO_Port, LCD_EN_Pin, GPIO_PIN_SET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(LCD_EN_GPIO_Port, LCD_EN_Pin, GPIO_PIN_RESET);
    HAL_Delay(1);
}

void LCD_Send4Bit(uint8_t data)
{
    HAL_GPIO_WritePin(LCD_D4_GPIO_Port, LCD_D4_Pin, (data & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LCD_D5_GPIO_Port, LCD_D5_Pin, (data & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LCD_D6_GPIO_Port, LCD_D6_Pin, (data & 0x04) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LCD_D7_GPIO_Port, LCD_D7_Pin, (data & 0x08) ? GPIO_PIN_SET : GPIO_PIN_RESET);

    LCD_EnablePulse();
}

void LCD_Command(uint8_t cmd)
{
    HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, GPIO_PIN_RESET);

    LCD_Send4Bit(cmd >> 4);
    LCD_Send4Bit(cmd & 0x0F);

    HAL_Delay(2);
}

void LCD_Data(uint8_t data)
{
    HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, GPIO_PIN_SET);

    LCD_Send4Bit(data >> 4);
    LCD_Send4Bit(data & 0x0F);

    HAL_Delay(2);
}
void LCD_Init(void)
{
    HAL_Delay(20);

    LCD_Command(0x33);
    LCD_Command(0x32);
    LCD_Command(0x28);   // 4-bit, 2 line
    LCD_Command(0x0C);   // Display ON
    LCD_Command(0x06);   // Entry mode
    LCD_Command(0x01);   // Clear
    HAL_Delay(5);
}
void LCD_SetCursor(uint8_t row, uint8_t col)
{
    uint8_t addr;
    if (row == 0) addr = 0x80;
    else if (row == 1) addr = 0xC0;
    else if (row == 2) addr = 0x94;
    else addr = 0xD4;

    LCD_Command(addr + col);
}

void LCD_Print(char *str)
{
    while (*str)
        LCD_Data(*str++);
}


