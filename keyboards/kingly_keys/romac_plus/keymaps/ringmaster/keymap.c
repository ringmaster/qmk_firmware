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
#include <print.h>

enum layers {
    _BAS = 0,
    _ARR,
    _GAM,
    _MOU,
    _MID,
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
uint16_t key_is_down = KC_NO;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [_BAS] = LAYOUT(
        KC_7, KC_8, KC_9_DEL,
        KC_4, KC_5, KC_6,
        KC_1, KC_2, KC_3,
        KC_LMENU, KC_0, KC_DOT_ENTER
    ),

    [_ARR] = LAYOUT(
        KC_HOME, KC_UP, KC_PGUP,
        KC_LEFT, KC_SPACE, KC_RIGHT,
        KC_END, KC_DOWN, KC_PGDN,
        KC_TRNS, KC_BSPC, KC_DOT_ENTER
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
        KC_LSHIFT, KC_LCTL, KC_F,
        KC_TRNS, KC_SPACE, KC_ENT
    ),

    [_MID] = LAYOUT(
        MI_B_1, MI_C_2, MI_D_2,
        MI_F_1, MI_G_1, MI_A_1,
        MI_C_1, MI_D_1, MI_E_1,
        KC_TRNS, MI_OCTD, MI_OCTU
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

void keyboard_post_init_user(void) {
  // Customise these values to desired behaviour
  debug_enable=true;
  //debug_matrix=true;
  //debug_keyboard=true;
  //debug_mouse=true;
}

//#ifdef OLED_DRIVER_ENABLE
extern rgblight_config_t rgblight_config;

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return OLED_ROTATION_270;  // flips the display 180 degrees if offhand
}

void blank(int lines) {
    int z;

    for(z = 0; z < lines; z++) {
        oled_write_P(PSTR("     "), false);
    }
}

void oled_task_user(void) {
    static char led_buf[50];
    //oled_clear();

    //*
    // Host Keyboard Layer Status
    if (menu_active) {
        snprintf(led_buf, sizeof(led_buf) - 1, "%c", 0x10);
        oled_write(led_buf, false);
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
        case _MID:
            oled_write_P(PSTR("MID"), false);
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
        snprintf(led_buf, sizeof(led_buf) - 1, "%c", 0x11);
        oled_write(led_buf, false);
    }
    else {
        oled_write_ln_P(PSTR(" "), false);
    }
    oled_write_P(PSTR("-----"), false);
    oled_write_P(PSTR("     "), false);

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
            snprintf(led_buf, sizeof(led_buf) - 1, " H%c%c  %c %c  E%c%c\n", 0x18, 0x1E, 0x1B, 0x1A, 0x19, 0x1F);
            oled_write(led_buf, false);
            blank(10);
            break;

        case _GAM:
            oled_write_P(PSTR(" QWE "), false);
            oled_write_P(PSTR(" ASD "), false);
            oled_write_P(PSTR(" scF "), false);
            oled_write_P(PSTR("  se "), false);
            blank(0);
            break;

        case _MOU:
            snprintf(led_buf, sizeof(led_buf) - 1, " 2%c1  %c%c%c ", 0x18, 0x1B, 0x19, 0x1A);
            oled_write(led_buf, false);
            blank(10);
            break;

        case _MID:
            snprintf(led_buf, sizeof(led_buf) - 1, "O: %d", midi_config.octave);
            oled_write(led_buf, false);
            blank(10);
            break;

        case _CAL:
            blank(10);
            break;

        case _COL:
            // Host Keyboard RGB backlight status

            //0uint8_t led_mode = rgblight_get_mode();

            snprintf(led_buf, sizeof(led_buf) - 1, "RGB:%cM: %2d\nh: %2ds: %2dv: %2d\nS: %2d\n",
                rgblight_config.enable ? '*' : '.', (uint8_t)rgblight_config.mode,
                (uint8_t)(rgblight_config.hue / RGBLIGHT_HUE_STEP),
                (uint8_t)(rgblight_config.sat / RGBLIGHT_SAT_STEP),
                (uint8_t)(rgblight_config.val / RGBLIGHT_VAL_STEP),
                (uint8_t)rgblight_get_speed());
            oled_write(led_buf, false);
            blank(1);
            break;
        default:
            blank(10);
            break;
    }
    //*/
}
//#endif

bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
    static uint16_t zero_timer;
    static uint16_t shift_timer;
    static uint16_t ctrl_timer;
    static bool shift_lock = false;
    static bool ctrl_lock = false;
    static bool shift_multi = false;
    static bool ctrl_multi = false;

    if(record->event.pressed) {
        key_is_down = keycode;
    }
    else {
        key_is_down = KC_NO;
    }

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

        case KC_LOCK_LSHIFT:
            if(record->event.pressed) {
                if (!shift_lock) {
                    register_code16(KC_LSFT);
                    shift_timer = timer_read();
                }
                shift_multi = false;
            } else {
                if (timer_elapsed(shift_timer) < TAPPING_TERM || shift_lock || shift_multi) {
                    unregister_code16(KC_LSFT);
                    shift_lock = false;
                }
                else {
                    shift_lock = !shift_lock;
                }
            }
            return false;
        case KC_LOCK_LCTL:
            if(record->event.pressed) {
                if (!ctrl_lock) {
                    ctrl_timer = timer_read();
                    register_code16(KC_LCTL);
                }
                ctrl_multi = false;
            } else {
                if (timer_elapsed(ctrl_timer) < TAPPING_TERM || ctrl_lock || ctrl_multi) {
                    unregister_code16(KC_LCTL);
                    ctrl_lock = false;
                }
                else {
                    ctrl_lock = !ctrl_lock;
                }
            }
            return false;
    }

    if (keycode != KC_LOCK_LSHIFT) {
        shift_lock = false;
        shift_multi = true;
    }
    if (keycode != KC_LOCK_LCTL) {
        ctrl_lock = false;
        ctrl_multi = true;
    }

    return true;
};

void knob_color(bool clockwise) {
    switch (key_is_down) {
        case RGB_M_B:
            if (clockwise) {
                rgblight_increase_speed_noeeprom();
            } else {
                rgblight_decrease_speed_noeeprom();
            }
            break;

        case RGB_VAI:
        case RGB_VAD:
            if (clockwise) {
                rgblight_increase_val_noeeprom();
            } else {
                rgblight_decrease_val_noeeprom();
            }
            break;

        case RGB_SAI:
        case RGB_SAD:
            if (clockwise) {
                rgblight_increase_sat_noeeprom();
            } else {
                rgblight_decrease_sat_noeeprom();
            }
            break;

        case RGB_HUI:
        case RGB_HUD:
            if (clockwise) {
                rgblight_increase_hue_noeeprom();
            } else {
                rgblight_decrease_hue_noeeprom();
            }
            break;

        default:
            if (clockwise) {
                rgblight_step_noeeprom();
            } else {
                rgblight_step_reverse_noeeprom();
            }
            break;
    }
}

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
        switch (get_highest_layer(layer_state)) {
            case _BAS:
                if (clockwise) {
                    tap_code16(KC__VOLUP);
                    //tap_code16(S(G(KC_Z)));
                } else {
                    tap_code16(KC__VOLDOWN);
                    //tap_code16(G(KC_Z));
                }
                break;
            case _COL:
                knob_color(clockwise);
                break;
            case _MOU:
                if (clockwise) {
                    tap_code16(KC_WH_U);
                } else {
                    tap_code16(KC_WH_D);
                }
                break;
            case _MID:
                if (clockwise) {
                    dprint("midi clockwise");
                    if (midi_config.octave < (MIDI_OCTAVE_MAX - MIDI_OCTAVE_MIN)) {
                        midi_config.octave++;
                        dprintf("midi octave %d\n", midi_config.octave);
                    }
                } else {
                    dprint("midi counter-clockwise");
                    if (midi_config.octave > 0) {
                        midi_config.octave--;
                        dprintf("midi octave %d\n", midi_config.octave);
                    }
                }
                break;
            default:
                break;
        }
    }
}
//#endif
