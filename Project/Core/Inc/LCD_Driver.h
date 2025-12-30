#ifndef INC_LCD_DRIVER_H_
#define INC_LCD_DRIVER_H_

#include<stdint.h>

void LCD_Driver_Init(void);
void LCD_Driver_Commad(uint8_t cmd);
void LCD_Driver_Data(uint8_t data);
void LCD_Driver_SetCursor(uint8_t row, uint8_t col);
void LCD_Driver_Print(const char *str);
void LCD_Driver_Clear(void);


#endif /* INC_LCD_DRIVER_H_ */
