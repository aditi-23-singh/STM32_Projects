#include "Application.h"
#include "ButtonHandler.h"
#include "LEDApplication.h"

#include "UART_Application.h"
#include "LCD_Driver.h"
#include "LED_Controller.h"
#include "Motor_Application.h"
#include "Keypad.h"
#include "KeypadHandler.h"

extern UARTApplicationTypedefStruct UartData;
extern ButtonApplicationTypedefStruct ButtonData;
extern LED_Controller_t local_ctrl;
extern LED_Controller_t remote_ctrl;
extern KeyPadHandlerTypedefStruct     KeypadData;

void HandleTick(void)
{
	UpdateButton();
	UpdateLED();
	UpdateKeypad();

	MotorApplication_Update();
}


void ApplicationInit()
{
	LED_Init();
	LCD_Driver_Init();
	UART_AppInit();
	KeypadHandler_Init();
	MotorApplication_Init();

}

void ApplicationProcess()
{


		LCDApplication_Process();


		if (UartData.NewEventReceived)
		{
	        SetRemoteMode(UartData.CurrentEvent);
	        MotorApplication_SetMotorMode(UartData.CurrentEvent);
	        LCD_SyncRemoteEvent(UartData.CurrentEvent);

	        UartData.NewEventReceived = false;
	    }

	    if (ButtonData.NewEventReceived)
	    {

	        UART_SendEvent(ButtonData.CurrentEvent);
	        SetLocalMode(ButtonData.CurrentEvent);
	        MotorApplication_SetMotorMode(ButtonData.CurrentEvent);
	        LCD_SyncLocalEvent(ButtonData.CurrentEvent);

	        ButtonData.NewEventReceived = false;
	    }
	    if (KeypadData.NewEventReceived)
	    	{

	    		UART_SendEvent(KeypadData.CurrentEvent);
	    		MotorApplication_SetMotorMode(KeypadData.CurrentEvent);
	    		SetLocalMode(KeypadData.CurrentEvent);
	    		LCD_SyncLocalEvent(KeypadData.CurrentEvent);



	    		KeypadData.NewEventReceived = false;
	    	}
	    LCD_UpdateLocalStatus(local_ctrl.mode);
	    LCD_UpdateRemoteStatus(remote_ctrl.mode);
}
