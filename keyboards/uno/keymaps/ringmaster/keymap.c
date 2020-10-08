#include QMK_KEYBOARD_H

enum uno_keycode
{
  UNO = SAFE_RANGE
};

static uint16_t pressTimer = 0xFFFF;
#define CUSTOM_LONGPRESS 150
#define CUSTOM_LONGERPRESS 750
#define CUSTOM_STRING "I can put a whole buncha text in here and type it all with a single keypress."
#define RESET_LENGTH 3000
const uint8_t PROGMEM RGBLED_RAINBOW_MOOD_INTERVALS[] = { 10, 25, 50 };

char stringToSend[2] = "a";
char maxLetter = 'z';

uint8_t presetCounter = 0;


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
        UNO
    )
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
	switch (keycode) {
		case UNO:
            if (record->event.pressed) {
                pressTimer = timer_read();
            } else {
                uint16_t timeElapsed = timer_elapsed(pressTimer);
                switch (presetCounter) {
                    case 0:
                        SEND_STRING(SS_LCMD("n"));
                    break;
                    case 1:
                        SEND_STRING("Hello!");
                    break;
                    case 2:
                        SEND_STRING(SS_TAP(X_ENTER)SS_TAP(X_ENTER)"I am uno!");
                    break;
                    case 3:
                        SEND_STRING(SS_TAP(X_ENTER)SS_TAP(X_ENTER)"I can do all sorts of useless things!");
                    break;
                    case 4:
                        SEND_STRING(SS_TAP(X_ENTER)SS_TAP(X_ENTER)"And I have a built-in RGB LED!"SS_TAP(X_ENTER)SS_TAP(X_ENTER)SS_TAP(X_ENTER));
                        rgblight_sethsv_noeeprom(255, 255, 255);
                        rgblight_mode_noeeprom(RGBLIGHT_MODE_RAINBOW_MOOD);
                    break;
                    default:
                        if (timeElapsed < CUSTOM_LONGPRESS) {
                            // Normal press.  We're going to send the current letter and increment the counter.
                            SEND_STRING(SS_TAP(X_BSPACE));
                            send_string(stringToSend);
                            stringToSend[0]++;
                            if (stringToSend[0] > maxLetter) {
                                stringToSend[0] = 'a';
                            }
                        } else if (timeElapsed < CUSTOM_LONGERPRESS) {
                            // Long press, confirm the current letter, reset counter
                            stringToSend[0] = 'a';
                            send_string(stringToSend);
                        } else if (timeElapsed < RESET_LENGTH) {
                            // Longer press, display macro.
                            SEND_STRING(CUSTOM_STRING);
                        } else {
                            reset_keyboard();
                        }
                        presetCounter--;
                    break;
                }
                presetCounter++;
            }
        break;
    }
    return false;
}

void keyboard_post_init_user(void) {
    rgblight_enable_noeeprom();
    rgblight_sethsv_noeeprom(0, 0, 0);
    rgblight_mode(1);
    //rgblight_mode_noeeprom(RGBLIGHT_MODE_RAINBOW_MOOD);
}
