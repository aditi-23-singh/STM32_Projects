#include "Keypad.h"
#include <string.h>

static const KeypadKey_t KeyMap[KEYPAD_ROWS][KEYPAD_COLS] = {

		    { KEY_NONE,   KEY_START,  KEY_NONE,  KEY_NONE },

		    { KEY_NONE,   KEY_NONE,  KEY_NONE,  KEY_NONE },

		    { KEY_NONE,   KEY_CLR,  KEY_NONE,   KEY_NONE },

		    { KEY_NONE,   KEY_ENT,  KEY_NONE,   KEY_NONE },

		    { KEY_NONE,   KEY_NONE,  KEY_NONE,  KEY_NONE }
};

static const char* KeyNames[] = {
    "NONE",   "F1",  "F2", "CLR", "+",  "ENT", "UP",  "LEFT",  "RIGHT",  "ZEUT",   "DN", "-",  "WASS", "AUTO", "LIFT","FACET","START","GRAIN"
};

static const struct {
    GPIO_TypeDef *port;
    uint16_t pin;
} RowPins[KEYPAD_ROWS] = {
    {ROW1_PORT, ROW1_PIN}, {ROW2_PORT, ROW2_PIN}, {ROW3_PORT, ROW3_PIN}, {ROW4_PORT, ROW4_PIN},	{ROW5_PORT, ROW5_PIN}
};

static const struct {
    GPIO_TypeDef *port;
    uint16_t pin;
} ColumnPins[KEYPAD_COLS] = {
    {COL1_PORT, COL1_PIN},    {COL2_PORT, COL2_PIN},    {COL3_PORT, COL3_PIN},    {COL4_PORT, COL4_PIN},

};

void Keypad_Init(Keypad_t *keypad)
{
    if (keypad == NULL) return;

    memset(keypad, 0, sizeof(Keypad_t));
    keypad->current_key = KEY_NONE;
    keypad->previous_key = KEY_NONE;
    keypad->pending_event = KEYPAD_EVENT_NONE;
    keypad->pressed_key = KEY_NONE;

    for (int i = 0; i < KEYPAD_ROWS; i++) {
        HAL_GPIO_WritePin(RowPins[i].port, RowPins[i].pin, GPIO_PIN_SET);
    }
}

static uint8_t Keypad_ReadColumns(void)
{
    uint8_t columnstate = 0;

    for (int i = 0; i < KEYPAD_COLS; i++) {
        if (HAL_GPIO_ReadPin(ColumnPins[i].port, ColumnPins[i].pin) == GPIO_PIN_RESET) {
            columnstate |= (1 << i);
        }
    }

    return columnstate;
}

static KeypadKey_t Keypad_ScanMatrix(void)
{
    KeypadKey_t detectedkey = KEY_NONE;

    for (int row = 0; row < KEYPAD_ROWS; row++)
    {
        HAL_GPIO_WritePin(RowPins[row].port, RowPins[row].pin, GPIO_PIN_RESET);
        for (volatile int i = 0; i < 10; i++);
        uint8_t col_state = Keypad_ReadColumns();
        HAL_GPIO_WritePin(RowPins[row].port, RowPins[row].pin, GPIO_PIN_SET);
        if (col_state != 0) {
            for (int col = 0; col < KEYPAD_COLS; col++) {
                if (col_state & (1 << col)) {
                    detectedkey = KeyMap[row][col];
                    return detectedkey;
                }
            }
        }
    }

    return detectedkey;
}

void Keypad_Scan(Keypad_t *keypad)
{
    if (keypad == NULL) return;

    uint32_t currenttime = HAL_GetTick();
    KeypadKey_t scannedkey = Keypad_ScanMatrix();

    if (scannedkey != keypad->current_key)
    {
        keypad->debounce_timer++;

        if (keypad->debounce_timer >= KEYPAD_DEBOUNCE_MS) {
            keypad->previous_key = keypad->current_key;
            keypad->current_key = scannedkey;
            keypad->debounce_timer = 0;

            if (keypad->current_key != KEY_NONE && keypad->previous_key == KEY_NONE) {
                keypad->press_start_time = currenttime;
                keypad->pending_event = KEYPAD_EVENT_KEY_PRESSED;
                keypad->pressed_key = keypad->current_key;
                keypad->key_hold_triggered = false;
            }

            else if (keypad->current_key == KEY_NONE && keypad->previous_key != KEY_NONE) {

                keypad->pending_event = KEYPAD_EVENT_KEY_RELEASED;
                keypad->pressed_key = keypad->previous_key;

                keypad->key_hold_triggered = false;
            }
        }
    } else
    {
        keypad->debounce_timer = 0;
    }

    if (keypad->current_key != KEY_NONE && !keypad->key_hold_triggered) {

        if ((currenttime - keypad->press_start_time) >=  KEYPAD_HOLD_TIME_MS) {
            keypad->pending_event = KEYPAD_EVENT_KEY_HOLD;
            keypad->pressed_key = keypad->current_key;
            keypad->key_hold_triggered = true;
        }
    }
}
Keypad_Event_t Keypad_GetEvent(Keypad_t *keypad, KeypadKey_t *key)
{
    if (keypad == NULL) return KEYPAD_EVENT_NONE;

    Keypad_Event_t event = keypad->pending_event;

    if (event != KEYPAD_EVENT_NONE && key != NULL) {
        *key = keypad->pressed_key;
    }

    keypad->pending_event = KEYPAD_EVENT_NONE;

    return event;
}

char Keypad_GetChar(KeypadKey_t key)
{
    if (key >= 0 && key < sizeof(KeyNames)/sizeof(KeyNames[0])) {
        return KeyNames[key][0];
    }
    return '\0';
}

const char* Keypad_GetKeyName(KeypadKey_t key)
{
    if (key >= 0 && key < sizeof(KeyNames)/sizeof(KeyNames[0])) {
        return KeyNames[key];
    }
    return "NONE";
}
