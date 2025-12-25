#include "App.h"
#include "LCD.h"
#include "LEDApplication.h"
#include "Button.h"


void App_Init(void)
{
    LCD_Init();

    LCD_SetCursor(0,0);
    LCD_Print("Mode: IDLE");

    LCD_SetCursor(1,0);
    LCD_Print("Delay: 250 ms");

    LEDInit();
}
