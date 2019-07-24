/*
  Copyright (c) 2019 Arnix Chen All right reserved.
  This is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public
  License as published by the Free Software Foundation; either
  version 3.0 of the License, or (at your option) any later version.
  This software is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.
  You should have received a copy of the GNU General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  Modified 05-03-2019 by Arnix Chen
*/

#ifndef __QUICK_INDEX_TABLE__
#define __QUICK_INDEX_TABLE__

// Quick index entry for speedup pronounce file searching
typedef struct {
  uint16_t index;
  char key;
} quickEntry;

const quickEntry quickIndex[] PROGMEM = {
  (const quickEntry PROGMEM) {48, '1'}, // ㄅㄚ
  (const quickEntry PROGMEM) {115, 'q'}, // ㄆㄚ
  (const quickEntry PROGMEM) {177, 'a'}, // ㄇㄚ
  (const quickEntry PROGMEM) {239, 'z'}, // ㄈㄚ
  (const quickEntry PROGMEM) {271, '2'}, // ㄉㄚ
  (const quickEntry PROGMEM) {344, 'w'}, // ㄊㄚ
  (const quickEntry PROGMEM) {415, 's'}, // ㄋㄚ
  (const quickEntry PROGMEM) {478, 'x'}, // ㄌㄚ
  (const quickEntry PROGMEM) {569, 'e'}, // ㄍㄚ
  (const quickEntry PROGMEM) {648, 'd'}, // ㄎㄚ
  (const quickEntry PROGMEM) {714, 'c'}, // ㄏㄚ
  (const quickEntry PROGMEM) {784, 'r'}, // ㄐㄧ
  (const quickEntry PROGMEM) {840, 'f'}, // ㄑㄧ
  (const quickEntry PROGMEM) {892, 'v'}, // ㄒㄧ
  (const quickEntry PROGMEM) {947, '5'}, // ㄓ
  (const quickEntry PROGMEM) {1016, 't'}, // ㄔ
  (const quickEntry PROGMEM) {1086, 'g'}, // ㄕ
  (const quickEntry PROGMEM) {1153, 'b'}, // ㄖˋ
  (const quickEntry PROGMEM) {1188, 'y'}, // ㄗ
  (const quickEntry PROGMEM) {1248, 'h'}, // ㄘ
  (const quickEntry PROGMEM) {1298, 'n'}, // ㄙ
  (const quickEntry PROGMEM) {1353, '8'}, // ㄚ
  (const quickEntry PROGMEM) {1358, 'i'}, // ㄛ
  (const quickEntry PROGMEM) {1363, 'k'}, // ㄜ
  (const quickEntry PROGMEM) {1367, ','}, // ㄝ
  (const quickEntry PROGMEM) {1369, '9'}, // ㄞ
  (const quickEntry PROGMEM) {1374, 'o'}, // ㄟ
  (const quickEntry PROGMEM) {1379, 'l'}, // ㄠ
  (const quickEntry PROGMEM) {1383, '.'}, // ㄡ
  (const quickEntry PROGMEM) {1387, '0'}, // ㄢ
  (const quickEntry PROGMEM) {1391, 'p'}, // ㄣ
  (const quickEntry PROGMEM) {1393, ';'}, // ㄤ
  (const quickEntry PROGMEM) {1396, '-'}, // ㄦ
  (const quickEntry PROGMEM) {1400, 'u'}, // ㄧ
  (const quickEntry PROGMEM) {1441, 'j'}, // ㄨ
  (const quickEntry PROGMEM) {1477, 'm'}, // ㄩ

  (const quickEntry PROGMEM) { 0, ' ' }, // dummy entry
};

#endif
