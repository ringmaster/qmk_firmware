#include QMK_KEYBOARD_H

enum uno_keycode
{
  UNO = SAFE_RANGE
};

static uint16_t pressTimer = 0xFFFF;
#define DOT_DASH_PRESS 200
#define ENTER_PRESS 750
#define CHAR_WAIT 350
#define SPACE_WAIT 1000

uint16_t code = 0;
char letters[32] = "                               ";

uint8_t presetCounter = 0;
bool waiting_space = false;

#define COUNTER X_A


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
                waiting_space = false;
                if (code == 0) {
                    code = 1;
                }
            } else {
                uint16_t timeElapsed = timer_elapsed(pressTimer);

                if (timeElapsed < DOT_DASH_PRESS) {
                    code = (code << 1);
                } else if (timeElapsed < ENTER_PRESS) {
                    code = (code << 1) + 1;
                } else {
                    SEND_STRING(SS_TAP(X_ENTER));
                    code = 64;
                }
                if (code >= 64) {
                    code = 0;
                    waiting_space = false;
                }
                else {
                    waiting_space = true;
                    pressTimer = timer_read();
                }
            }
        break;
    }
    return false;
}

void matrix_init_user(void) {
    letters[0b101] = 'a';
    letters[0b11000] = 'b';
    letters[0b11010] = 'c';
    letters[0b1100] = 'd';
    letters[0b10] = 'e';
    letters[0b10010] = 'f';
    letters[0b1110] = 'g';
    letters[0b10000] = 'h';
    letters[0b100] = 'i';
    letters[0b10111] = 'j';
    letters[0b1101] = 'k';
    letters[0b10100] = 'l';
    letters[0b111] = 'm';
    letters[0b110] = 'n';
    letters[0b1111] = 'o';
    letters[0b10110] = 'p';
    letters[0b11101] = 'q';
    letters[0b1010] = 'r';
    letters[0b1000] = 's';
    letters[0b11] = 't';
    letters[0b1001] = 'u';
    letters[0b10001] = 'v';
    letters[0b1011] = 'w';
    letters[0b11001] = 'x';
    letters[0b11011] = 'y';
    letters[0b11100] = 'z';
}

void matrix_scan_user(void) {
    uint16_t timeElapsed = timer_elapsed(pressTimer);

    if (waiting_space) {
        if (timeElapsed > SPACE_WAIT) {
            waiting_space = false;
            SEND_STRING(" ");
        }
        else if (timeElapsed > CHAR_WAIT && code != 0) {
            send_char(letters[code]);
            code = 0;
        }
    }

}

