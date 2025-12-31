#include "LCDApplication.h"
#include "LCD_Driver.h"
#include <stdio.h>
#define TEMP_DISPLAY_DURATION_MS 2000

//static DisplayMode_t current_mode = DISPLAY_MODE_IDLE;
//static DisplayMode_t prev_mode = 255;
//static DisplayMode_t display_mode = DISPLAY_MODE_IDLE;
//static uint32_t last_mode_change = 0;

//static LEDStatus_t local_status = LED_STATUS_OFF;
//static LEDStatus_t remote_status = LED_STATUS_OFF;
//static uint8_t led_status_changed = 0;

// LOCAL board
static DisplayMode_t local_mode = DISPLAY_MODE_IDLE;
static DisplayMode_t local_prev_mode = 255;
static uint32_t local_last_mode_change = 0;

static LEDStatus_t local_led_status = LED_STATUS_OFF;

// REMOTE board
static DisplayMode_t remote_mode = DISPLAY_MODE_IDLE;
static DisplayMode_t remote_prev_mode = 255;

static LEDStatus_t remote_led_status = LED_STATUS_OFF;
static uint32_t remote_last_mode_change = 0;

//static void UpdateModeDisplay(void);
//static void UpdateLEDDisplay(void);
static void DisplayLocalMode(void);
static void DisplayLocalLED(void);
static void DisplayRemoteMode(void);
static void DisplayRemoteLED(void);
// LOCAL board updates
void LCD_UpdateLocalMode(DisplayMode_t mode);
void LCD_UpdateLocalLED(LEDStatus_t status);

// REMOTE board updates
void LCD_UpdateRemoteMode(DisplayMode_t mode);
void LCD_UpdateRemoteLED(LEDStatus_t status);
//static const char* GetLEDStatusString(LEDStatus_t local, LEDStatus_t remote);

void LCD_UpdateLocalMode(DisplayMode_t mode)
{
    if (local_mode != mode)
    {
        local_mode = mode;
        local_prev_mode = 255;   // force LCD refresh
        local_last_mode_change = HAL_GetTick();
    }
}
void LCD_UpdateLocalLED(LEDStatus_t status)
{
    local_led_status = status;
}
void LCD_UpdateRemoteMode(DisplayMode_t mode)
{
    if (remote_mode != mode)
    {
        remote_mode = mode;
        remote_prev_mode = 255;
        remote_last_mode_change=HAL_GetTick();

    }
}

void LCD_UpdateRemoteLED(LEDStatus_t status)
{
    remote_led_status = status;
}

void LCDApplication_Init(void)
{
	LCD_Driver_Init();

	    LCD_Driver_Clear();

	    LCD_Driver_SetCursor(0, 0); LCD_Driver_Print("L-MODE: IDLE   ");
	    LCD_Driver_SetCursor(1, 0); LCD_Driver_Print("LD3: OFF       ");
	    LCD_Driver_SetCursor(2, 0); LCD_Driver_Print("R-MODE: IDLE   ");
	    LCD_Driver_SetCursor(3, 0); LCD_Driver_Print("LD4: OFF       ");
}

//void LCDApplication_UpdateMode(DisplayMode_t mode)
//{
//    current_mode = mode;
//}

//void LCDApplication_UpdateLEDStatus(LEDStatus_t local, LEDStatus_t remote)
//{
//    if (local_status != local || remote_status != remote)
//    {
//        local_status = local;
//        remote_status = remote;
//        led_status_changed = 1;
//    }
//}

void LCDApplication_Process(void)
{

	if ((local_mode == DISPLAY_MODE_SINGLE || local_mode == DISPLAY_MODE_DOUBLE) &&
	        (HAL_GetTick() - local_last_mode_change >= TEMP_DISPLAY_DURATION_MS))
	    {
	        local_mode = DISPLAY_MODE_IDLE;
	        local_prev_mode = 255;
	    }
	if ((remote_mode == DISPLAY_MODE_SINGLE || remote_mode == DISPLAY_MODE_DOUBLE) &&
	        (HAL_GetTick() - remote_last_mode_change >= TEMP_DISPLAY_DURATION_MS))
	    {
	        remote_mode = DISPLAY_MODE_IDLE;
	        remote_prev_mode = 255;
	    }

	    DisplayLocalMode();
	    DisplayLocalLED();
	    DisplayRemoteMode();
	    DisplayRemoteLED();
}
static void DisplayLocalMode(void)
{
    LCD_Driver_SetCursor(0, 0);

    switch (local_mode)
    {
        case DISPLAY_MODE_SINGLE: LCD_Driver_Print("L-MODE: SINGLE "); break;
        case DISPLAY_MODE_DOUBLE: LCD_Driver_Print("L-MODE: DOUBLE "); break;
        case DISPLAY_MODE_HOLD:   LCD_Driver_Print("L-MODE: HOLD   "); break;
        default:                  LCD_Driver_Print("L-MODE: IDLE   "); break;
    }
}
static void DisplayLocalLED(void)
{
    LCD_Driver_SetCursor(1, 0);

    switch (local_led_status)
    {
        case LED_STATUS_HOLD:       LCD_Driver_Print("L-LED: ON (HOLD) "); break;
        case LED_STATUS_FAST_BLINK: LCD_Driver_Print("L-LED: FAST BLNK "); break;
        case LED_STATUS_SLOW_BLINK: LCD_Driver_Print("L-LED: SLOW BLNK "); break;
        default:                    LCD_Driver_Print("L-LED: OFF       "); break;
    }
}
static void DisplayRemoteMode(void)
{
    LCD_Driver_SetCursor(2, 0);

    switch (remote_mode)
    {
        case DISPLAY_MODE_SINGLE: LCD_Driver_Print("R-MODE: SINGLE "); break;
        case DISPLAY_MODE_DOUBLE: LCD_Driver_Print("R-MODE: DOUBLE "); break;
        case DISPLAY_MODE_HOLD:   LCD_Driver_Print("R-MODE: HOLD   "); break;
        default:                  LCD_Driver_Print("R-MODE: IDLE   "); break;
    }
}
static void DisplayRemoteLED(void)
{
    LCD_Driver_SetCursor(3, 0);

    switch (remote_led_status)
    {
        case LED_STATUS_HOLD:       LCD_Driver_Print("R-LED: ON (HOLD) "); break;
        case LED_STATUS_FAST_BLINK: LCD_Driver_Print("R-LED: FAST BLNK "); break;
        case LED_STATUS_SLOW_BLINK: LCD_Driver_Print("R-LED: SLOW BLNK "); break;
        default:                    LCD_Driver_Print("R-LED: OFF       "); break;
    }
}


//static void UpdateModeDisplay(void)
//{
//
//    if (current_mode != prev_mode)
//    {
//        LCD_Driver_SetCursor(0, 0);
//
//        switch (current_mode)
//        {
//            case DISPLAY_MODE_HOLD:
//                LCD_Driver_Print("Mode: HOLD      ");
//                display_mode = DISPLAY_MODE_HOLD;
//                break;
//
//            case DISPLAY_MODE_DOUBLE:
//                LCD_Driver_Print("Mode: DOUBLE    ");
//                display_mode = DISPLAY_MODE_DOUBLE;
//                last_mode_change = HAL_GetTick();
//                break;
//            case DISPLAY_MODE_SINGLE:
//                           LCD_Driver_Print("Mode: SINGLE    ");
//                           display_mode = DISPLAY_MODE_SINGLE;
//                           last_mode_change = HAL_GetTick();
//                           break;
//
//                       case DISPLAY_MODE_IDLE:
//                           // Only update to IDLE if not in a temporary display state
//                           if (display_mode == DISPLAY_MODE_IDLE || display_mode == DISPLAY_MODE_HOLD)
//                           {
//                               LCD_Driver_Print("Mode: IDLE      ");
//                               display_mode = DISPLAY_MODE_IDLE;
//                           }
//                           break;
//
//                       default:
//                           LCD_Driver_Print("Mode: IDLE      ");
//                           display_mode = DISPLAY_MODE_IDLE;
//                           break;
//                   }
//
//                   prev_mode = current_mode;
//    }
//    if ((display_mode == DISPLAY_MODE_SINGLE || display_mode == DISPLAY_MODE_DOUBLE) &&
//           (HAL_GetTick() - last_mode_change >= TEMP_DISPLAY_DURATION_MS))
//       {
//           LCD_Driver_SetCursor(0, 0);
//           LCD_Driver_Print("Mode: IDLE      ");
//           display_mode = DISPLAY_MODE_IDLE;
//       }
//   }

//static void UpdateLEDDisplay(void)
//{
//    char buffer[20];
//    LCD_Driver_SetCursor(1, 0);
//
//    const char* status_str = GetLEDStatusString(local_status, remote_status);
//    snprintf(buffer, sizeof(buffer), "LED: %-11s", status_str);
//    LCD_Driver_Print(buffer);
//}
//static const char* GetLEDStatusString(LEDStatus_t local, LEDStatus_t remote)
//{
//    if (local == LED_STATUS_OFF && remote == LED_STATUS_OFF)
//    {
//        return "OFF";
//    }
//    else if (local == LED_STATUS_HOLD || remote == LED_STATUS_HOLD)
//    {
//        return "ON (HOLD)";
//    }
//    else if (local == LED_STATUS_FAST_BLINK || remote == LED_STATUS_FAST_BLINK)
//    {
//        return "FAST BLINK";
//    }
//    else if (local == LED_STATUS_SLOW_BLINK || remote == LED_STATUS_SLOW_BLINK)
//    {
//        return "SLOW BLINK";
//    }
//
//    return "OFF";
//}



