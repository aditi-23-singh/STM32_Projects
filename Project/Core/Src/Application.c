#include "Application.h"
#include "ButtonHandler.h"
#include "LEDApplication.h"

#include "UART_Application.h"
#include "LCD_Driver.h"
#include "LED_Controller.h"
#include "MotorManager.h"
#include "Keypad.h"
#include "KeypadHandler.h"

extern UARTApplicationTypedefStruct UartData;
extern ButtonApplicationTypedefStruct ButtonData;
extern LED_Controller_t local_ctrl;
extern LED_Controller_t remote_ctrl;
extern KeypadApplicationTypedefStruct     KeypadData;

void HandleTick(void)
{
	UpdateButton();
	UpdateLED();
	UpdateKeypad();
}


void ApplicationInit()
{
	LED_Init();
	LCD_Driver_Init();
	UART_AppInit();
	KeypadHandler_Init();
	//MotorManager_Init();
}

void ApplicationProcess()
{


		LCDApplication_Process();
//		MotorManager_Update();

		if (UartData.NewEventReceived)
	    {
//			MotorManager_HandleEvent(UartData.CurrentEvent);
	        SetRemoteMode(UartData.CurrentEvent);

	        LCD_SyncRemoteEvent(UartData.CurrentEvent);

	        UartData.NewEventReceived = false;
	    }

	    if (ButtonData.NewEventReceived)
	    {
//	    	MotorManager_HandleEvent(ButtonData.CurrentEvent);
	        UART_SendEvent(ButtonData.CurrentEvent);
	        SetLocalMode(ButtonData.CurrentEvent);

	        LCD_SyncLocalEvent(ButtonData.CurrentEvent);

	        ButtonData.NewEventReceived = false;
	    }
	    if (KeypadData.NewEventReceived)
	    	{
	    		//MotorManager_HandleEvent(KeypadData.CurrentEvent);
	    		UART_SendEvent(KeypadData.CurrentEvent);
	    		SetLocalMode(KeypadData.CurrentEvent);
	    		LCD_SyncLocalEvent(KeypadData.CurrentEvent);

	    		// Optional: Display pressed key on LCD
	    		// LCD_DisplayKeypadKey(KeypadData.CurrentChar);

	    		KeypadData.NewEventReceived = false;
	    	}
	    LCD_UpdateLocalStatus(local_ctrl.mode);
	    LCD_UpdateRemoteStatus(remote_ctrl.mode);
}
