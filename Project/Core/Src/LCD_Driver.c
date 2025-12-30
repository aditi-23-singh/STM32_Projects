#include "LCD_Driver.h"
#include "LCD.h"
void LCD_Driver_Init(void)
{
	HAL_Delay(50);
	LCD_Command(0x33);
	HAL_Delay(5);
	LCD_Command(0x32);
	HAL_Delay(5);
	LCD_Command(0x28);   /* 4-bit, 2 line, 5x8 dots */
	LCD_Command(0x0C);   /* Display ON, Cursor OFF */
	LCD_Command(0x06);   /* Entry mode: increment cursor */
	LCD_Command(0x01);   /* Clear display */
	HAL_Delay(5);
}

void LCD_Driver_Command(uint8_t cmd)
{
	LCD_Command(cmd);
}
void LCD_Driver_Data(uint8_t data){
	  LCD_Data(data);
}
void LCD_Driver_SetCursor(uint8_t row, uint8_t col)
{
    uint8_t addr;
    if (row == 0)
        addr = 0x80 + col;
    else
        addr = 0xC0 + col;

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
