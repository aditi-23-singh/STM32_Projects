#include "KeypadHandler.h"
#include "Keypad.h"

Keypad_t HardwareKeypad;
KeyPadHandlerTypedefStruct KeypadData;

static EVENT_CODES_ENUM MapKeyToEvent(KeypadKey_t key, Keypad_Event_t event_type)
{
    switch (key) {
        case KEY_CLR:
            if (event_type == KEYPAD_EVENT_KEY_PRESSED) {
                return EVT_SINGLE_CLICK;
            }
            break;

        case KEY_ENT:
            if (event_type == KEYPAD_EVENT_KEY_PRESSED) {
                return EVT_DOUBLE_CLICK;
            }
            break;

        case KEY_START:
            if (event_type == KEYPAD_EVENT_KEY_HOLD) {
                              return EVT_HOLD_START;
            }
            else if (event_type == KEYPAD_EVENT_KEY_RELEASED) {
                           return EVT_HOLD_END;
            }
            break;

        case KEY_FACET_LEFT:
                    if (event_type == KEYPAD_EVENT_KEY_PRESSED) {
                        return EVT_FACET_CW;  // Left button = Clockwise
                    }
                    else if (event_type == KEYPAD_EVENT_KEY_RELEASED) {
                        return EVT_HOLD_END;  // Stop when released
                    }
                    break;

                case KEY_FACET_RIGHT:
                    if (event_type == KEYPAD_EVENT_KEY_PRESSED) {
                        return EVT_FACET_CCW;  // Right button = Counter-clockwise
                    }
                    else if (event_type == KEYPAD_EVENT_KEY_RELEASED) {
                        return EVT_HOLD_END;  // Stop when released
                    }
                    break;

                case KEY_GRAIN_LEFT:
                        if (event_type == KEYPAD_EVENT_KEY_PRESSED) {
                            return EVT_GRAIN_CW;  // Left button = Clockwise
                        }
                        else if (event_type == KEYPAD_EVENT_KEY_RELEASED) {
                            return EVT_HOLD_END;  // Stop when released
                        }
                        break;

                    case KEY_GRAIN_RIGHT:
                        if (event_type == KEYPAD_EVENT_KEY_PRESSED) {
                            return EVT_GRAIN_CCW;  // Right button = Counter-clockwise
                        }
                        else if (event_type == KEYPAD_EVENT_KEY_RELEASED) {
                            return EVT_HOLD_END;  // Stop when released
                        }
                        break;


    }

    return EVT_NONE;
}

void KeypadHandler_Init(void)
{
    Keypad_Init(&HardwareKeypad);

    KeypadData.CurrentEvent = EVT_NONE;
    KeypadData.PreviousEvent = EVT_NONE;
    KeypadData.NewEventReceived = false;
    KeypadData.CurrentKey = KEY_NONE;
    KeypadData.CurrentChar = '\0';
}

void UpdateKeypad(void)
{
    Keypad_Scan(&HardwareKeypad);

    KeypadKey_t key;
    Keypad_Event_t eventtype = Keypad_GetEvent(&HardwareKeypad, &key);

    if (eventtype == KEYPAD_EVENT_NONE) {
        return;
    }

    KeypadData.CurrentKey = key;
    KeypadData.CurrentChar = Keypad_GetChar(key);

    EVENT_CODES_ENUM appevent = MapKeyToEvent(key, eventtype);

    if (appevent != EVT_NONE) {
        KeypadData.PreviousEvent = KeypadData.CurrentEvent;
        KeypadData.CurrentEvent = appevent;
        KeypadData.NewEventReceived = true;
    }
}
