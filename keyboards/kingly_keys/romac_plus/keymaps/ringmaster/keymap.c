/* Copyright 2018 Jack Humbert
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include QMK_KEYBOARD_H
#include <stdio.h>

enum layers {
    _BAS = 0,
    _ARR,
    _GAM,
    _MOU,
    _CAL,
    _COL,
    _DOC
};

enum custom_keycodes {
    KC_LMENU = SAFE_RANGE,
    KC_DOT_ENTER,
    KC_LOCK_LSHIFT,
    KC_LOCK_LCTL,
    KC_DOCS,
    KC_9_DEL
};

bool menu_active = false;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [_BAS] = LAYOUT(
        KC_7, KC_8, KC_9_DEL,
        KC_4, KC_5, KC_6,
        KC_1, KC_2, KC_3,
        KC_LMENU, KC_0, KC_DOT_ENTER
    ),

    [_ARR] = LAYOUT(
        KC_HOME, KC_UP, KC_END,
        KC_LEFT, KC_DOWN, KC_RIGHT,
        KC_TRNS, KC_TRNS, KC_BSPC,
        KC_TRNS, KC_TRNS, KC_ENT
    ),

    [_GAM] = LAYOUT(
        KC_Q, KC_W, KC_E,
        KC_A, KC_S, KC_D,
        KC_LOCK_LSHIFT, KC_LOCK_LCTL, KC_F,
        KC_TRNS, KC_SPACE, KC_ENT
    ),

    [_MOU] = LAYOUT(
        KC_BTN2, KC_MS_U, KC_BTN1,
        KC_MS_L, KC_MS_D, KC_MS_R,
        KC_LOCK_LSHIFT, KC_LOCK_LCTL, KC_F,
        KC_TRNS, KC_SPACE, KC_ENT
    ),

    [_CAL] = LAYOUT(
        KC_7, KC_8, KC_9,
        KC_4, KC_5, KC_6,
        KC_1, KC_2, KC_3,
        KC_TRNS, KC_0, KC_DOT_ENTER
    ),

    [_COL] = LAYOUT(
        RGB_VAI, RGB_SAI, RGB_HUI,
        RGB_VAD, RGB_SAD, RGB_HUD,
        RGB_M_K, RGB_M_B, RGB_M_P,
        KC_TRNS, RGB_MOD, RGB_TOG
    ),

    [_DOC] = LAYOUT(
        KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO,
        KC_LMENU, KC_NO, KC_DOCS
    )
};

//#ifdef OLED_DRIVER_ENABLE
extern rgblight_config_t rgblight_config;

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return OLED_ROTATION_270;  // flips the display 180 degrees if offhand
}

/*
static void render_logo(void) {
    static const char PROGMEM qmk_logo[] = {
        0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F, 0x90, 0x91, 0x92, 0x93, 0x94,
        0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF, 0xB0, 0xB1, 0xB2, 0xB3, 0xB4,
        0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF, 0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0x00
    };

    oled_write_P(qmk_logo, false);
}
*/

void blank(int lines) {
    int z;

    for(z = 0; z < lines; z++) {
        oled_write_P(PSTR("     "), false);
    }
}

void oled_task_user(void) {
    static char led_buf[30];
    //oled_clear();

    //*
    // Host Keyboard Layer Status
    if (menu_active) {
        oled_write_P(PSTR(">"), false);
    }
    else {
        oled_write_P(PSTR(" "), false);
    }
    switch (get_highest_layer(layer_state)) {
        case _BAS:
            oled_write_P(PSTR("BAS"), false);
            break;
        case _ARR:
            oled_write_P(PSTR("ARR"), false);
            break;
        case _GAM:
            oled_write_P(PSTR("GAM"), false);
            break;
        case _COL:
            oled_write_P(PSTR("COL"), false);
            break;
        case _MOU:
            oled_write_P(PSTR("MOU"), false);
            break;
        case _CAL:
            oled_write_P(PSTR("CAL"), false);
            break;
        case _DOC:
            oled_write_P(PSTR("DOC"), false);
            break;
        default:
            // Or use the write_ln shortcut over adding '\n' to the end of your string
            oled_write_P(PSTR("???"), false);
    }
    if (menu_active) {
        oled_write_ln_P(PSTR("<"), false);
    }
    else {
        oled_write_ln_P(PSTR(" "), false);
    }
    oled_write_P(PSTR("-----"), false);

    //*/

    /*
    // Host Keyboard LED Status
    uint8_t led_usb_state = host_keyboard_leds();
    oled_write_P(PSTR("-----"), false);
    oled_write_P(PSTR("Stats"), false);
    oled_write_P(led_usb_state & (1<<USB_LED_NUM_LOCK) ? PSTR("num:*") : PSTR("num:."), false);
    oled_write_P(led_usb_state & (1<<USB_LED_CAPS_LOCK) ? PSTR("cap:*") : PSTR("cap:."), false);
    oled_write_P(led_usb_state & (1<<USB_LED_SCROLL_LOCK) ? PSTR("scr:*") : PSTR("scr:."), false);
    //*/

    //*
    switch (biton32(layer_state)) {
        case _BAS:
            oled_write_P(PSTR("     "), false);
            oled_write_P(PSTR(" 789 "), false);
            oled_write_P(PSTR(" 456 "), false);
            oled_write_P(PSTR(" 123 "), false);
            oled_write_P(PSTR("  0. "), false);
            oled_write_P(PSTR("     "), false);
            oled_write_P(PSTR("Holds"), false);
            oled_write_P(PSTR("-----"), false);
            oled_write_P(PSTR(". Ent"), false);
            oled_write_P(PSTR("9 Del"), false);
            blank(2);
            break;
        case _ARR:
            oled_write_P(PSTR(" HUE "), false);
            oled_write_P(PSTR(" LDR "), false);
            blank(12);
            break;

        case _GAM:
            oled_write_P(PSTR(" QWE "), false);
            oled_write_P(PSTR(" ASD "), false);
            oled_write_P(PSTR(" scF "), false);
            oled_write_P(PSTR("  se "), false);
            blank(0);
            break;

        case _COL:
            // Host Keyboard RGB backlight status

            //0uint8_t led_mode = rgblight_get_mode();

            snprintf(led_buf, sizeof(led_buf) - 1, "RGB:%cM: %2d\nh: %2ds: %2dv: %2d\n",
                rgblight_config.enable ? '*' : '.', (uint8_t)rgblight_config.mode,
                (uint8_t)(rgblight_config.hue / RGBLIGHT_HUE_STEP),
                (uint8_t)(rgblight_config.sat / RGBLIGHT_SAT_STEP),
                (uint8_t)(rgblight_config.val / RGBLIGHT_VAL_STEP));
            oled_write(led_buf, false);
            blank(1);
            break;
        default:
            blank(14);
            break;
    }
    //*/
}
//#endif

bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
    static uint16_t zero_timer;

    switch (keycode) {
        case KC_9_DEL:
            if(record->event.pressed) {
                zero_timer = timer_read();
            } else {
                if (timer_elapsed(zero_timer) < TAPPING_TERM) {
                    tap_code16(KC_9);
                }
                else {
                    tap_code16(KC_BSPC);
                }
            }
            return false; // We handled this keypress

        case KC_DOT_ENTER:
            if(record->event.pressed) {
                zero_timer = timer_read();
            } else {
                if (timer_elapsed(zero_timer) < TAPPING_TERM) {
                    tap_code16(KC_DOT);
                }
                else {
                    tap_code16(KC_ENTER);
                }
            }
            return false; // We handled this keypress

        case KC_LMENU:
            if (!record->event.pressed) {
                menu_active = !menu_active;
            }
            return false;
        case KC_DOCS:
            if (!record->event.pressed) {
                SEND_STRING("Hi!  This is some doumentation.");
            }
            return false;
    }
    return true;
};

//#ifdef ENCODER_ENABLE
void encoder_update_user(uint8_t index, bool clockwise) {
    uint8_t current_layer;

    if (menu_active) {
        current_layer = get_highest_layer(layer_state);
        if (clockwise) {
            current_layer++;
        } else {
            current_layer--;
        }
        if (current_layer < 0) {
            current_layer = 0;
        }
        if (current_layer >= sizeof(keymaps)) {
            current_layer = sizeof(keymaps) - 2;
        }
        layer_move(current_layer);
    }
    else {
        switch (biton32(layer_state)) {
            case _BAS:
                // History scrubbing. For Adobe products, hold shift while moving
                // backward to go forward instead.
                if (clockwise) {
                    tap_code16(S(G(KC_Z)));
                } else {
                    tap_code16(G(KC_Z));
                }
                break;
            case _COL:
                // History scrubbing. For Adobe products, hold shift while moving
                // backward to go forward instead.
                if (clockwise) {
                    rgblight_step();
                } else {
                    rgblight_step_reverse();
                }
                break;
            case _MOU:
                // History scrubbing. For Adobe products, hold shift while moving
                // backward to go forward instead.
                if (clockwise) {
                    tap_code16(KC_WH_U);
                } else {
                    tap_code16(KC_WH_D);
                }
                break;
            default:
                break;
        }
    }
}
//#endif
