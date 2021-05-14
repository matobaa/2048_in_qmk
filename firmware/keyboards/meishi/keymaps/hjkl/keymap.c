/* Copyright 2019 matobaa
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
#include "game.c"

void rewind(void) {
  register_code(KC_LSFT);
  tap_code(KC_UP);
  tap_code(KC_UP);
  tap_code(KC_UP);
  tap_code(KC_UP);
  tap_code(KC_UP);
  unregister_code(KC_LSFT);
}

unsigned int pressed = 0x00;  // bitmap represents key status
bool sent = false;

enum custom_keycode {
  LEFT = SAFE_RANGE,
  DOWN,
  UP,
  RIGHT
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT( LEFT, DOWN, UP, RIGHT ), /* Default layer */
};

const uint16_t PROGMEM fn_actions[] = {

};


void matrix_init_user(void) {
  reset_board();
}

void matrix_scan_user(void) {
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {

  int key = 1 << (keycode - SAFE_RANGE);

  if (IS_PRESSED(record->event)) {
    pressed |= key;
    switch(pressed) {
    case 0x09:
      srand(timer_read());
      reset_board();
      sent = true;
      break;
    case 0x05:
    case 0x0a:
      SEND_STRING("UNDO");
      sent = true;
      break;
    case 0x03:
    case 0x06:
    case 0x0c:
      rewind();
      dump_board();
      sent = true;
      break;
    }
  }
  if (IS_RELEASED(record->event)) {
    pressed &= ~key;
    if(pressed == 0) {
      if(!sent) {
	move(keycode - SAFE_RANGE) && newtile();
	rewind();
	dump_board();
	if(!canmove()) {
	    SEND_STRING("GAME OVER");
	    return false;
	  }
      }
      sent = false;
    }
  }
  return true;
}

void led_set_user(uint8_t usb_led) {

}

