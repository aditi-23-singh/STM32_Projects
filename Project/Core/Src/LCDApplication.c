#include "LCDApplication.h"
#include "LCD_Driver.h"
#include <stdio.h>

static const uint32_t MESSAGE_EXPIRY_TIME_MS = 1000;
static const uint8_t ROW_LOCAL_MODE  = 0;
static const uint8_t ROW_LOCAL_LED   = 1;
static const uint8_t ROW_REMOTE_MODE = 2;
static const uint8_t ROW_REMOTE_LED  = 3;
static const uint8_t COL_VALUE_START = 8;
static const uint8_t INVALID_VAL     = 255;


typedef enum {
    LCD_INIT_RESET,
    LCD_INIT_READY
} LCD_InitState_t;

static LCD_InitState_t init_state = LCD_INIT_RESET;
typedef struct {
    DisplayMode_t mode;
    DisplayMode_t prev_mode;
    uint32_t last_mode_change;
    LEDStatus_t led_status;
    LEDStatus_t prev_led_status;
    uint8_t row_mode;
    uint8_t row_led;
    const char* label_mode;
    const char* label_led;
} LCD_Entity_t;

static LCD_Entity_t local =  { DISPLAY_MODE_IDLE, INVALID_VAL, 0, LED_STATUS_OFF, INVALID_VAL, ROW_LOCAL_MODE,  ROW_LOCAL_LED,  "L-MODE: ", "L-LED : " };
static LCD_Entity_t remote = { DISPLAY_MODE_IDLE, INVALID_VAL, 0, LED_STATUS_OFF, INVALID_VAL, ROW_REMOTE_MODE, ROW_REMOTE_LED, "R-MODE: ", "R-LED : " };

void LCD_SyncRemoteEvent(uint8_t event)
{
    LCD_UpdateRemoteMode((DisplayMode_t)event);
}
void LCD_SyncLocalEvent(uint8_t event)
{
    LCD_UpdateLocalMode((DisplayMode_t)event);
}
void LCD_UpdateLocalStatus(uint8_t status)
{
	LCD_UpdateLocalLED((LEDStatus_t)status);
}
void LCD_UpdateRemoteStatus(uint8_t status)
{
	LCD_UpdateRemoteLED((LEDStatus_t)status);
}
static void RefreshModeDisplay(LCD_Entity_t* entity) {
    if (entity->mode == entity->prev_mode) return;

    LCD_Driver_SetCursor(entity->row_mode, COL_VALUE_START);
    switch (entity->mode) {
        case DISPLAY_MODE_SINGLE: LCD_Driver_Print("SINGLE "); break;
        case DISPLAY_MODE_DOUBLE: LCD_Driver_Print("DOUBLE "); break;
        case DISPLAY_MODE_HOLD:   LCD_Driver_Print("HOLD   "); break;
        default:                  LCD_Driver_Print("IDLE   "); break;
    }
    entity->prev_mode = entity->mode;
}
static void RefreshLEDDisplay(LCD_Entity_t* entity) {
    if (entity->led_status == entity->prev_led_status) return;

    LCD_Driver_SetCursor(entity->row_led, COL_VALUE_START);
    switch (entity->led_status) {
        case LED_STATUS_HOLD:       LCD_Driver_Print("ON (HOLD) "); break;
        case LED_STATUS_FAST_BLINK: LCD_Driver_Print("FAST BLNK "); break;
        case LED_STATUS_SLOW_BLINK: LCD_Driver_Print("SLOW BLNK "); break;
        default:                    LCD_Driver_Print("OFF       "); break;
    }
    entity->prev_led_status = entity->led_status;
}
static void CheckTimeout(LCD_Entity_t* entity) {
    if ((entity->mode == DISPLAY_MODE_SINGLE || entity->mode == DISPLAY_MODE_DOUBLE) &&
        (HAL_GetTick() - entity->last_mode_change >= MESSAGE_EXPIRY_TIME_MS)) {
        entity->mode = DISPLAY_MODE_IDLE;
    }
}
void LCD_UpdateLocalMode(DisplayMode_t mode) {
    if (local.mode != mode) {
    	local.mode = mode;
    	local.prev_mode = INVALID_VAL;
    	local.last_mode_change = HAL_GetTick();
    }
}

void LCD_UpdateLocalLED(LEDStatus_t status) {
    if (local.led_status != status) {
    	local.led_status = status;
    	local.prev_led_status = INVALID_VAL;
    }
}

void LCD_UpdateRemoteMode(DisplayMode_t mode) {
    if (remote.mode != mode) { remote.mode = mode; remote.prev_mode = INVALID_VAL; remote.last_mode_change = HAL_GetTick(); }
}

void LCD_UpdateRemoteLED(LEDStatus_t status) {
    if (remote.led_status != status) { remote.led_status = status; remote.prev_led_status = INVALID_VAL; }
}
void LCDApplication_Init(void) {
    LCD_Driver_Init();
    LCD_Driver_Clear();
    init_state = LCD_INIT_RESET;
    local.prev_mode = remote.prev_mode = INVALID_VAL;
    local.prev_led_status = remote.prev_led_status = INVALID_VAL;
}
void LCDApplication_Process(void) {
    if (init_state == LCD_INIT_RESET) {
        LCD_Driver_SetCursor(local.row_mode, 0);  LCD_Driver_Print(local.label_mode);
        LCD_Driver_SetCursor(local.row_led, 0);   LCD_Driver_Print(local.label_led);
        LCD_Driver_SetCursor(remote.row_mode, 0); LCD_Driver_Print(remote.label_mode);
        LCD_Driver_SetCursor(remote.row_led, 0);  LCD_Driver_Print(remote.label_led);
        init_state = LCD_INIT_READY;
        return;
    }

    CheckTimeout(&local);
    CheckTimeout(&remote);

    RefreshModeDisplay(&local);
    RefreshLEDDisplay(&local);
    RefreshModeDisplay(&remote);
    RefreshLEDDisplay(&remote);
}

