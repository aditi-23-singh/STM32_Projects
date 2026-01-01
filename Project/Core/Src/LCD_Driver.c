#include "LCD_Driver.h"
#include "LCD.h"
void LCD_Driver_Init(void)
{
	HAL_Delay(50);
	LCD_Command(0x33);
	HAL_Delay(5);
	LCD_Command(0x32);
	HAL_Delay(5);
	LCD_Command(0x28);
	LCD_Command(0x0C);
	LCD_Command(0x06);
	LCD_Command(0x01);
	HAL_Delay(5);
}


void LCD_Driver_SetCursor(uint8_t row, uint8_t col)
{
    uint8_t addr;
    switch(row)
       {
           case 0:
               addr = 0x80 + col;
               break;
           case 1:
               addr = 0xC0 + col;
               break;
           case 2:
               addr = 0x94 + col;
               break;
           case 3:
               addr = 0xD4 + col;
               break;
           default:
               addr = 0x80;
               break;
       }

    LCD_Command(addr);
}

void LCD_Driver_Print(const char *str)
{
    while (*str)
        LCD_Data(*str++);
}

void LCD_Driver_Clear(void)
{
    LCD_Command(0x01);
    HAL_Delay(2);
}
