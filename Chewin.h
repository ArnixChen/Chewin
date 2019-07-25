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

#ifndef __CHEWIN__
#define __CHEWIN__

#define sentenceBufferSize 48
#define memoSlotSize 10
#define spellBufferSize 5
#define chewinStartNumber  12
#define ROWS 7           //number of rows of keypad
#define COLS 10          //number of columns of keypad

#include <Arduino.h>
#include <avr/pgmspace.h>
#include <EEPROM.h>
#include <SoftwareSerial.h>
#include <DFPlayerMini_Fast.h>

#include "spellList.h"
#include "quickIndexTable.h"
#include "toneFixTable.h"

#define ledPin  13
#define defaultVolume 0x16

#define checkVccPeriod  20000 // unit in mini-second
#define ledBlinkPeriod  1000  // unit in mini-second
#define batteryVoltageLowThreshold 3650 // unit in mini-volt

// EEPROM will be updated after romUpdateRequestDelay time
#define romUpdateRequestDelay 500 // unit in mili-second

#define SND_SYSTEM_START 1
#define SND_SENTENCE_RESTART 2
#define SND_SENTENCE_DEL 3
#define SND_SENTENCE_MEMORIZE 4
#define SND_VOLUME_DOWN 5
#define SND_VOLUME_UP 6
#define SND_SPELL_RESTART 7
#define SND_SPELL_SOUND_NOT_PREPARED 8
#define SND_SPELL_ILLEGAL 9
#define SND_IT_NEEDS_CHARGING 10
#define SND_SILENCE 11

#define TONE_KEY1 ' '
#define TONE_KEY2 '6'
#define TONE_KEY3 '3'
#define TONE_KEY4 '4'
#define TONE_KEY5 '7'
#define SILENCE_KEY '='

#ifndef NO_KEY
#define NO_KEY 0x0
#endif

#define KEY_TYPE_A 0x01 // ㄅ ~ ㄙ
#define KEY_TYPE_B 0x02 // Reserved 
#define KEY_TYPE_C 0x04 // ㄧ ㄨ ㄩ
#define KEY_TYPE_D 0x08 // ㄚ ~ ㄥ
#define KEY_TYPE_E 0x10 // ㄦ
#define KEY_TYPE_F 0x20 // [ ˊˇˋ˙ =]
#define KEY_TYPE_G 0x40 // Function Keys

typedef struct {
  char symbol[4]; // unicode of chewin symbol
  char ascii;  // ascii mapping of chewin symbol
  uint8_t keyType;  // type of chewin symbol
  uint8_t hid_id; // USB HID usage ID mapping */
} chewinMapEntry;

// A sentence is fromed of a series of sentenceEntrys.
typedef struct {
  char keys[spellBufferSize];
  uint16_t sndIndex;
} sentenceEntry;

typedef struct {
  uint8_t volume;
  uint8_t mode;
  uint8_t checkSum;
} eepromHeader;

// Structure to store sentence in EEPROM when press memoSlot key
typedef struct {
  uint16_t sndIndex[sentenceBufferSize];
  uint8_t length;
  uint32_t checkSum;
} memoRecord;

class Chewin {
 public:
  Chewin();
  ~Chewin();
  chewinMapEntry* getChewinMapEntry(char scanCode);
  void begin(uint8_t pinForTx, uint8_t pinForRx);
  char getScanCodeFromHID(uint8_t mod, uint8_t hid);
  
  virtual void doHousekeeping();
  virtual void processScanCode(char scanCode);
  virtual void processKeyCode(char key, char scanCode);

 protected:
  SoftwareSerial* _mp3Serial = NULL;
  DFPlayerMini_Fast* _mp3Module = NULL;
  
  bool romUpdateRequest = false;
  uint16_t memoSlotUpdateRequest = 0;
  unsigned long romUpdateRequestTime = 0;
  uint8_t currVolume = 0;
  uint8_t currMode = 0;

  memoRecord memoSlot;
  sentenceEntry sentenceBuffer[sentenceBufferSize];
  uint8_t sentenceBufferIdx = 0;
  char spellBuffer[spellBufferSize];
  uint8_t spellBufferIdx = 0;
  uint8_t toneFixCounter = 0;

 private:
  bool do3SpellToneFix();
  void saveSentenceToMemoSlot(uint8_t slotIdx);
  void playSentenceFromMemoSlot(uint8_t slotIdx);
  void updateEEprom();
  void restoreFromEEprom();
  uint16_t getKeySoundIdx(char key);

  chewinMapEntry chewinMapEntryBuffer;
  spell spellLookupBuffer;
  spell * getSpell(uint16_t idx) {
    memcpy_P(&spellLookupBuffer, &spellList[idx], sizeof(spell));
#ifdef __SERIAL_DEBUG__DEEP__
    Serial.print(F("spellLookupBuffer = "));
    Serial.println(spellLookupBuffer.keys);
#endif
    return (spell *) &spellLookupBuffer;
  }

  uint16_t getSpellSoundIdx(char * keys, char firstScanCode);
  void updateShortCutTable();

  toneFixEntry toneFixEntryBuffer;
  toneFixEntry * getToneFixEntry(uint8_t idx) {
    memcpy_P(&toneFixEntryBuffer, &toneFixTable[idx], sizeof(toneFixEntry));
    return (toneFixEntry *) &toneFixEntryBuffer;
  }
};

#endif