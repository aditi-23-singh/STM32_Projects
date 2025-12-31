#include "Application.h"
#include "ButtonHandler.h"
#include "LEDApplication.h"
#include "LCDApplication.h"
#include "UART_Application.h"
#include "LCD_Driver.h"
#include "LED_Controller.h"

extern UARTApplicationTypedefStruct UartData;
extern ButtonApplicationTypedefStruct ButtonData;
extern LED_Controller_t local_ctrl;
extern LED_Controller_t remote_ctrl;

void HandleTick(void)
{
	UpdateButton();
	UpdateLED();
}

void ApplicationInit()
{
	LED_Init();
	LCD_Driver_Init();
	UART_AppInit();
}

void ApplicationProcess()
{
		// *TODO* Integrate this when LCD is enabled
		LCDApplication_Process();

	//remote board event
		if (UartData.NewEventReceived)
	    {
	        SetRemoteMode(UartData.CurrentEvent);

	        LCD_UpdateRemoteMode((DisplayMode_t)UartData.CurrentEvent);

	        UartData.NewEventReceived = false;
	    }
	 	 //Local button event
	    if (ButtonData.NewEventReceived)
	    {
	        UART_SendEvent(ButtonData.CurrentEvent);
	        SetLocalMode(ButtonData.CurrentEvent);

	        LCD_UpdateLocalMode((DisplayMode_t)ButtonData.CurrentEvent);

	        ButtonData.NewEventReceived = false;
	    }
	    LCD_UpdateLocalLED((LEDStatus_t)local_ctrl.mode);
	    LCD_UpdateRemoteLED((LEDStatus_t)remote_ctrl.mode);
}
