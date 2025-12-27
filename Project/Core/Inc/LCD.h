#ifndef LCD_H
#define LCD_H

#include "main.h"
#include <stdint.h>

/* Function prototypes */
void LCD_Init(void);
void LCD_SetCursor(uint8_t row, uint8_t col);
void LCD_Print(char *str);
void LCD_Command(uint8_t cmd);
void LCD_Data(uint8_t data);

#endif /* LCD_H */
