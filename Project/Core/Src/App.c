/* Modified App.c - ADD UART INITIALIZATION */
#include "App.h"
#include "LCD.h"
#include "LEDApplication.h"
#include "Button.h"
#include "UARTCommunication.h"  // NEW: Add this include

void App_Init(void)
{
    LCD_Init();

    LCD_SetCursor(0,0);
    LCD_Print("LOCAL - IDLE");        // Changed for clarity
    LCD_SetCursor(1,0);
    LCD_Print("Delay: 250 ms");

    LCD_SetCursor(2,0);
    LCD_Print("REMOTE - IDLE");       // NEW: Remote board status
    LCD_SetCursor(3,0);
    LCD_Print("Delay: 250 ms");

    LEDInit();

    UART_Comm_Init();  // NEW: Initialize UART
}
