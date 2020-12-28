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
    _WAT,
    _DOC
};

enum custom_keycodes {
    KC_LMENU = SAFE_RANGE,
    KC_DOT_ENTER,
    KC_LOCK_LSHIFT,
    KC_LOCK_LCTL,
    KC_DOCS,
    KC_9_DEL,
    KC_SQ0,
    KC_SQ1,
    KC_SQ2,
    KC_SQ3,
    KC_SQ4,
    KC_SQ5,
    KC_SQ6,
    KC_SQ7,
    KC_SQ8,
    KC_GAME_RESET,
};

bool menu_active = false;
uint16_t key_is_down = KC_NO;


enum states {
    GAME_READY = 0,
    GAME_O,
    GAME_X,
    GAME_OVER,
};

int game_state = GAME_READY;
int game_timer;
int cha = 1;
int watb[9] = {0,0,0,0,0,0,0,0,0};
int hturn = 1;
int score = 0;

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

    [_WAT] = LAYOUT(
        KC_SQ0, KC_SQ1, KC_SQ2,
        KC_SQ3, KC_SQ4, KC_SQ5,
        KC_SQ6, KC_SQ7, KC_SQ8,
        KC_TRNS, KC_NO, KC_GAME_RESET
    ),

    [_DOC] = LAYOUT(
        KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO,
        KC_TRNS, KC_NO, KC_DOCS
    )
};

void keyboard_post_init_user(void) {
  // Customise these values to desired behaviour
  debug_enable=true;
  //debug_matrix=true;
  //debug_keyboard=true;
  //debug_mouse=true;

  game_timer = timer_read();
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

void render_wat(void) {
    static char led_buf[50];

    static char syms[3] = {32, 131, 132};

    snprintf(led_buf, sizeof(led_buf) - 1, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",
        syms[watb[0]], 129, syms[watb[1]], 129, syms[watb[2]],
        128, 130, 128, 130, 128,
        syms[watb[3]], 129, syms[watb[4]], 129, syms[watb[5]],
        128, 130, 128, 130, 128,
        syms[watb[6]], 129, syms[watb[7]], 129, syms[watb[8]]);
    oled_write(led_buf, false);

    snprintf(led_buf, sizeof(led_buf) - 1, "\x10 %c \x11", syms[hturn]);
    oled_write(led_buf, false);
    snprintf(led_buf, sizeof(led_buf) - 1, "S: %d", score);
    oled_write(led_buf, false);
    oled_write_P(PSTR("\n"), false);

    switch (game_state) {
        case GAME_READY:
            oled_write_P(PSTR("READY"), false);
            break;
        case GAME_OVER:
            oled_write_P(PSTR("GAME\nOVER\n"), false);
            break;
        default:
            oled_write_P(PSTR(" GO!"), false);
            break;
    }

    blank(4);
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
        case _WAT:
            oled_write_P(PSTR("WAT"), false);
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
    snprintf(led_buf, sizeof(led_buf) - 1, "%c%c%c%c%c", 128, 128, 128, 128, 128);
    oled_write(led_buf, false);

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

        case _WAT:
            render_wat();
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

int check_line(int a, int b, int c) {
    if (watb[a] == watb[b] && watb[b] == watb[c]) return watb[a];
    return 0;
}

int check_board(void) {
    int wincond[8][3] = {{0,1,2},{3,4,5},{6,7,8},{0,3,6},{1,4,7},{2,5,8},{0,4,8},{2,4,6}};

    for(int z = 0; z < 8; z++) {
        if (check_line(wincond[z][0], wincond[z][1], wincond[z][2]) != 0) {
            return check_line(wincond[z][0], wincond[z][1], wincond[z][2]);
        }
    }
    for(int y = 0; y < 9; y++) {
        if (watb[y] == 0) return 0;
    }
    return -1;
}

int check_board_clear(int move, int player) {
    int wincond[8][3] = {{0,1,2},{3,4,5},{6,7,8},{0,3,6},{1,4,7},{2,5,8},{0,4,8},{2,4,6}};
    int alternate = 3 - player;
    bool clear_line;

    for(int z = 0; z < 8; z++) {
        clear_line = false;
        // if the move is in line with what is being checked
        if (wincond[z][0] == move) {
            if (watb[wincond[z][1]] == alternate && watb[wincond[z][2]] == alternate) {
                clear_line = true;
            }
        }
        if (wincond[z][1] == move) {
            if (watb[wincond[z][0]] == alternate && watb[wincond[z][2]] == alternate) {
                clear_line = true;
            }
        }
        if (wincond[z][2] == move) {
            if (watb[wincond[z][0]] == alternate && watb[wincond[z][1]] == alternate) {
                clear_line = true;
            }
        }

        if (clear_line) {
            score++;
            for (int y = 0; y < 3; y++) {
                watb[wincond[z][y]] = 0;
            }
        }
    }
    return check_board();
}

void clear_board(void) {
    watb[0] = 0;
    watb[1] = 0;
    watb[2] = 0;
    watb[3] = 0;
    watb[4] = 0;
    watb[5] = 0;
    watb[6] = 0;
    watb[7] = 0;
    watb[8] = 0;
}

void wat_ai(int player) {
    int wincond;
    int move;

    wincond = check_board();

    if (wincond == -1) {
        game_state = GAME_OVER;
    }
    else {
        do {
            move = rand() % 9;
        } while (watb[move] != 0);
        dprintf("Move %d\n", move);
        watb[move] = player;
        wincond = check_board();
        if (wincond != 0) {
            game_state = GAME_OVER;
        }
    }
}

void wat_move(int space, int player, bool human) {
    int wincond = 0;

    if (watb[space] != 0) return;

    watb[space] = player;
    wincond = check_board();

    if (wincond != 0) {
        dprintf("%d check board\n", wincond);
        game_state = GAME_OVER;
    }
    else {
        if (human) {
            check_board_clear(space, player);
            hturn = 3 - hturn;
        }
    }
}

void wat_timer(void) {
    switch (game_state) {
    case GAME_READY:
        break;
    case GAME_O:
        if (timer_elapsed(game_timer) >= 3000) {
            wat_ai(2);
            game_timer = timer_read();
            game_state = GAME_X;
        }
        break;
    case GAME_X:
        if (timer_elapsed(game_timer) >= 3000) {
            wat_ai(1);
            game_timer = timer_read();
            game_state = GAME_O;
        }
        break;
    case GAME_OVER:
        break;
    }
}

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
        case KC_GAME_RESET:
            clear_board();
            game_state = GAME_READY;
            score = 0;
            return false;
        case KC_SQ0:
        case KC_SQ1:
        case KC_SQ2:
        case KC_SQ3:
        case KC_SQ4:
        case KC_SQ5:
        case KC_SQ6:
        case KC_SQ7:
        case KC_SQ8:
            if(record->event.pressed) {
                switch (game_state) {
                case GAME_READY:
                    hturn = 1;
                    wat_ai(1);
                    game_state = GAME_O;
                    break;
                case GAME_X:
                case GAME_O:
                    switch (keycode) {
                        case KC_SQ0: wat_move(0, hturn, true); return false;
                        case KC_SQ1: wat_move(1, hturn, true); return false;
                        case KC_SQ2: wat_move(2, hturn, true); return false;
                        case KC_SQ3: wat_move(3, hturn, true); return false;
                        case KC_SQ4: wat_move(4, hturn, true); return false;
                        case KC_SQ5: wat_move(5, hturn, true); return false;
                        case KC_SQ6: wat_move(6, hturn, true); return false;
                        case KC_SQ7: wat_move(7, hturn, true); return false;
                        case KC_SQ8: wat_move(8, hturn, true); return false;
                    }
                    break;
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

void matrix_scan_user(void) {
    uint8_t layer = biton32(layer_state);

    // INSERT CODE HERE: turn off all leds

    switch (layer) {
    case _WAT:
        wat_timer();
        break;
    }
}

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
            case _WAT:
                if (clockwise) {
                    cha++;
                } else {
                    cha--;
                }
                break;
            default:
                break;
        }
    }
}
//#endif
