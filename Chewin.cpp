#include "Chewin.h"
#include "readVcc.h"
#include <SoftwareSerial.h>

//#define __SERIAL_DEBUG__
//#define __SERIAL_DEBUG_XX__
//#define __SERIAL_DEBUG_DEEP1__

uint16_t shortCutTableForSound[6][10] = {
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF},
  {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF},
};

/*const char ascIIMap[ROWS][COLS] PROGMEM = {
  {'1', '2', TONE_KEY3, TONE_KEY4, '5', TONE_KEY2, TONE_KEY5, '8', '9', '0'},
  {'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p'},
  {'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';'},
  {'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/'},
  {'!', '@', '#', '$', '%', '^', '&', '*', '(', ')'},
  {'~', '{', '}', '[', ']', '\\', '-', TONE_KEY1, '<', '>'},
  {SILENCE_KEY,  0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},
  };*/
  
Keypad* Chewin::keypadObj = NULL;
char Chewin::_scanCodeWhileAudioPlaying = NO_KEY;

Chewin::Chewin(uint8_t rows, uint8_t cols, const chewinMapEntry* chewinMap, uint16_t battLowThreshold) {
  ROWS = rows;
  COLS = cols;
  _chewinMap = chewinMap;
  batteryLowThreshold = battLowThreshold;
  updateShortCutTable();
}

Chewin::Chewin(uint8_t rows, uint8_t cols, const chewinMapEntry* chewinMap) {
  ROWS = rows;
  COLS = cols;
  _chewinMap = chewinMap;
  batteryLowThreshold = defaultBatteryLowThreshold;
  updateShortCutTable();
  _scanCodeWhileAudioPlaying = NO_KEY;
}

Chewin::~Chewin() {
  if (_mp3Serial != NULL) {
    delete _mp3Serial;
  }

  if (mp3Module != NULL) {
    delete mp3Module;
  }
}

bool Chewin::idleWorkerForMp3Module() {
  if (keypadObj != NULL) {
    char result = keypadObj->getKey();
    if (result != NO_KEY) {
      _scanCodeWhileAudioPlaying = result;
      return true;
    }
  }
  return false;
}

void Chewin::audioInit(uint8_t pinForTx, uint8_t pinForRx) {
  _mp3Serial = new SoftwareSerial(pinForTx, pinForRx);
  _mp3Serial->begin(9600);
  mp3Module = new DFPlayerMini_Arnix();
  mp3Module->setIdleWorker(idleWorkerForMp3Module);
  mp3Module->begin(* _mp3Serial);

  restoreFromEEprom();
  delay(1250);
  mp3Module->volume(currVolume);
  mp3Module->play(SND_SYSTEM_START);
}

void Chewin::playNumber(uint8_t number) {
  uint16_t sndIdx;
  switch (number) {
    case 0: sndIdx = SND_NO_ZERO; break;
    case 1: sndIdx = SND_NO_ONE; break;
    case 2: sndIdx = SND_NO_TWO; break;
    case 3: sndIdx = SND_NO_THREE; break;
    case 4: sndIdx = SND_NO_FOUR; break;
    case 5: sndIdx = SND_NO_FIVE; break;
    case 6: sndIdx = SND_NO_SIX; break;
    case 7: sndIdx = SND_NO_SEVEN; break;
    case 8: sndIdx = SND_NO_EIGHT; break;
    case 9: sndIdx = SND_NO_NINE; break;
  }
#ifdef  __SERIAL_DEBUG__
  Serial.print(F("sndIdx="));
  Serial.println(sndIdx);
#endif
  mp3Module->playAndWait(sndIdx);
}

chewinMapEntry* Chewin::getChewinMapEntry(char scanCode) {
  uint8_t row;
  uint8_t col;

  row = ((scanCode & 0xF0) >> 4) - 1;
  col = (scanCode & 0x0F) - 1;

  /*if (row < 6) {
    return (chewinMapEntry *) memcpy_P(&chewinMapEntryBuffer, &chewinMap[0][0] + row * COLS + col, sizeof(chewinMapEntry));
    } else {
    return NULL;
    }*/

  return (chewinMapEntry *) memcpy_P(&chewinMapEntryBuffer, _chewinMap + row * COLS + col, sizeof(chewinMapEntry));
}

void Chewin::setKeypad(Keypad* keypad) {
  keypadObj = keypad;
}

char Chewin::getKey() {
  if (_scanCodeWhileAudioPlaying != NO_KEY) {
    char tmpScanCode = _scanCodeWhileAudioPlaying;
    _scanCodeWhileAudioPlaying = NO_KEY;
    return tmpScanCode;
  } else {
    return keypadObj->getKey();
  } 
}

uint8_t Chewin::getToneOfSpell(char keys[spellBufferSize]) {
  char lastKey=0x0;
  
  lastKey = keys[strlen(keys) - 1];
  switch (lastKey) {
    case TONE_KEY2:
      return 2;
      break;

    case TONE_KEY3:
      return 3;
      break;
      
    case TONE_KEY4:
      return 4;
      break;
      
    case TONE_KEY5:
      return 5;
      break;
      
    default:
      return 1;
  }
  
  return 0;
}

uint8_t Chewin::getToneFixCounter(uint8_t currIdx) {
  uint8_t toneFixCounter = 0;
  uint8_t currSpellTone = 0;
  uint8_t nextSpellTone = 0;
  uint8_t next2SpellTone = 0;

  currSpellTone = getToneOfSpell(sentenceBuffer[currIdx].keys);
#ifdef __SERIAL_DEBUG_DEEP1__
  Serial.print(F("tone["));
  Serial.print(currIdx);
  Serial.print(F("]: "));
  Serial.print(currSpellTone);
#endif

  if (currSpellTone == 3) {
    toneFixCounter++;
    if (currIdx+1 < sentenceBufferIdx) {
      nextSpellTone = getToneOfSpell(sentenceBuffer[currIdx+1].keys);
      if (nextSpellTone == 3) {
        toneFixCounter++;
        if (currIdx+2 < sentenceBufferIdx) {
          next2SpellTone = getToneOfSpell(sentenceBuffer[currIdx+2].keys);
          if (next2SpellTone == 3) {
            toneFixCounter++;
          }
        }
      }
    }
  }
#ifdef __SERIAL_DEBUG_DEEP1__
  Serial.print(F(" tfc="));
  Serial.print(toneFixCounter);
  Serial.println();
#endif
  return toneFixCounter;  
}

// return true for toneFix success , false for toneFix no match
bool Chewin::do3SpellToneFix(uint8_t currIdx) {
  uint8_t i = 0;
  uint8_t result;
  uint8_t j = 0;

  getToneFixEntry(i++);
  while (strlen(toneFixEntryBuffer.origin[0].keys) != 0) {
    result = 0;
    for (j = 0; j < 3; j++) {
      result += strcmp(sentenceBuffer[currIdx+j].keys, toneFixEntryBuffer.origin[j].keys);
      if (result != 0) break; // If the first spell does not match , then skip rest spell compares for this entry
    }

    if (result == 0) {
      for (j = 0; j < 3; j++) {
        //strcpy(sentenceBuffer[sentenceBufferIdx + j].keys, toneFixEntryBuffer.fixed[j].keys);
        sentenceBuffer[currIdx+j].sndIndex = getSpellSoundIdx(sentenceBuffer[currIdx+j].keys, 0x11) + toneFixEntryBuffer.diff[j];
      }
#ifdef __SERIAL_DEBUG_XX__
      Serial.println(F("\nTone fixed by table\n"));
#endif
      return true;
    }

    getToneFixEntry(i++);
  }

  // For most popular case (3 3 3) to (6 6 3), we fix tone by rule
  //uint8_t len = strlen(sentenceBuffer[sentenceBufferIdx - 2].keys);
  //sentenceBuffer[sentenceBufferIdx - 2].keys[len - 1] = TONE_KEY2;
  //sentenceBuffer[currIdx].sndIndex = getSpellSoundIdx(sentenceBuffer[currIdx].keys, 0x11) - 1;
  //sentenceBuffer[currIdx+1].sndIndex = getSpellSoundIdx(sentenceBuffer[currIdx+1].keys, 0x11) - 1;;

  return false;
}

uint8_t Chewin::processToneFix(uint8_t i) {
  toneFixCounter = getToneFixCounter(i);
  switch (toneFixCounter) {
    case 2:
#ifdef __SERIAL_DEBUG_DEEP1__
      Serial.print(F("tf2("));
      Serial.print(sentenceBuffer[i].keys);
      Serial.print(F(") "));
      Serial.print(sentenceBuffer[i].sndIndex);
      Serial.print(F("->"));
#endif
      sentenceBuffer[i].sndIndex = getSpellSoundIdx(sentenceBuffer[i].keys, 0x11) - 1;
#ifdef __SERIAL_DEBUG_DEEP1__
      Serial.println(sentenceBuffer[i].sndIndex);
#endif
      // process 2 spells internally and i skip 2
      mp3Module->playAndWait(sentenceBuffer[i].sndIndex);
      if (_scanCodeWhileAudioPlaying != NO_KEY) return i;
      mp3Module->playAndWait(sentenceBuffer[i+1].sndIndex);
      i+=1;
      break;
      
    case 3:
      if (do3SpellToneFix(i)) {
        // process 3 spells internally and i skip 3
        mp3Module->playAndWait(sentenceBuffer[i].sndIndex);
        if (_scanCodeWhileAudioPlaying != NO_KEY) return i;
        mp3Module->playAndWait(sentenceBuffer[i+1].sndIndex);
        if (_scanCodeWhileAudioPlaying != NO_KEY) return i;
        mp3Module->playAndWait(sentenceBuffer[i+2].sndIndex);
        if (_scanCodeWhileAudioPlaying != NO_KEY) return i;
        i+=2;
      } else {
        // try toneFix2
#ifdef __SERIAL_DEBUG_DEEP1__
        Serial.print(F("tf2("));
        Serial.print(sentenceBuffer[i].keys);
        Serial.print(F(") "));
        Serial.print(sentenceBuffer[i].sndIndex);
        Serial.print(F("->"));
#endif
        sentenceBuffer[i].sndIndex = getSpellSoundIdx(sentenceBuffer[i].keys, 0x11) - 1;
#ifdef __SERIAL_DEBUG_DEEP1__
        Serial.println(sentenceBuffer[i].sndIndex);
#endif
        // process 2 spells internally and i skip 2
        mp3Module->playAndWait(sentenceBuffer[i].sndIndex);
        if (_scanCodeWhileAudioPlaying != NO_KEY) return i;
        mp3Module->playAndWait(sentenceBuffer[i+1].sndIndex);
        i+=1;
      }
      break;
  }
  return i;
}

// test sentence: 你好我想炒米粉也可以當晚餐
void Chewin::playSentenceFrom(sentenceSrc src) {
  uint16_t sndIdx;
  uint8_t resultIdx;
  
  switch(src) {
    case SENTENCE_BUFFER:
      for (int i = 0; i < sentenceBufferIdx; i++) {
        if (idleWorkerForMp3Module())
          return;
          
        if (toneFixEnabled == true) {
#ifdef __SERIAL_DEBUG_DEEP1__
          Serial.println(F("playSentenceFrom:toneFixEnabled"));
#endif
          resultIdx = processToneFix(i);
          if (_scanCodeWhileAudioPlaying != NO_KEY) return;
          if (resultIdx != i) {
            i = resultIdx;
            continue;
          }
        }
        
        sndIdx = sentenceBuffer[i].sndIndex;
        if (sndIdx == SND_SILENCE) {
          if (playSilenceAsSound != 0) {
            sndIdx = SND_WOOD_FISH - 1 + playSilenceAsSound;
          }
        }

        mp3Module->playAndWait(sndIdx);
        if (_scanCodeWhileAudioPlaying != NO_KEY) return;
      }
      break;
  
    case MEMO_SLOT:
      for (uint8_t i = 0; i < memoSlot.length; i++) {
        if (idleWorkerForMp3Module())
          return;
          
        sndIdx = memoSlot.sndIndex[i];
        if (sndIdx == SND_SILENCE) {
          if (playSilenceAsSound != 0) {
            sndIdx = SND_WOOD_FISH - 1 + playSilenceAsSound;
          }
        }
        mp3Module->playAndWait(sndIdx);
        if (_scanCodeWhileAudioPlaying != NO_KEY)
          return;
      }
  }
}

uint16_t Chewin::getKeySoundIdx(char key) {
  uint16_t i = 0;

  do {
    getSpell(i);

    if (spellLookupBuffer.keys[0] == 0) {
      break;
    }

    if (key == spellLookupBuffer.keys[0]) {
#ifdef __SERIAL_DEBUG_DEEP__
      Serial.print(spellLookupBuffer.keys);
      Serial.print(F(" <-- Match!!\n"));
#endif
      return (i + chewinStartNumber);
    }
    i++;
  } while (i < 37); // 37 is the note number of chewin
#ifdef __SERIAL_DEBUG_DEEP__
  Serial.println(F(" nothing matched!"));
#endif
  return 0xFFFF;
}

char Chewin::getScanCodeFromHID(uint8_t mod, uint8_t hid) {
  char scanCode;
#ifdef __SERIAL_DEBUG__
  Serial.print(F("hidKey from USB: 0x"));
  Serial.println(hid, HEX);
#endif
  for (uint8_t i = 0; i < ROWS; i++) {
    for (uint8_t j = 0; j < COLS; j++) {
      scanCode = (((i + 1) << 4) + j + 1);
      if (hid == getChewinMapEntry(scanCode)->hid_id) {
        return scanCode;
      }
    }
  }
  return NO_KEY;
}

// result status which indicate this scancode need to do processKeyCode()
// true -- need processKeyCode()
// false -- noneed to do processKeyCode()
void Chewin::processScanCode(char scanCode, unsigned long scanTime=millis()) {
  static char prevScanCode = 0;
  static unsigned long prevScanTime;
  static uint8_t eqSelect = 0;
  bool result = false;
  // Add time limitation to prevent user from playing sound effects of those implicit switches (0x63 + 0x13...)
  bool justInTime = ((scanTime - prevScanTime) < 900) ? true : false;

  switch (scanCode) {
    case 0x68: // Press spacebar key to add silence
      if ((spellBufferIdx == 0) && (sentenceBufferIdx != 0)) {
        if (sentenceBufferIdx < sentenceBufferSize) {
          if (sentenceBuffer[sentenceBufferIdx-1].keys[0] != TONE_KEY1) {
            if (playSilenceAsSound == 0) {
              mp3Module->play(SND_SILENCE);  
            } else {
              mp3Module->play(SND_WOOD_FISH - 1 + playSilenceAsSound);
            }
            sentenceBuffer[sentenceBufferIdx].keys[0] = TONE_KEY1;
            sentenceBuffer[sentenceBufferIdx].keys[1] = 0x0;
            sentenceBuffer[sentenceBufferIdx++].sndIndex = SND_SILENCE;
            result = false;
          }
        }
      } else {
        result = true;
      }
      break;

    case 0x65:  // '>'
      if (volumeKeyLocked) {
      } else {
        currVolume = mp3Module->volumeUp();
#ifdef __SERIAL_DEBUG__
        Serial.print(F("vol = "));
        Serial.println(currVolume);
#endif
        mp3Module->play(SND_VOLUME_UP);
        romUpdateRequestTime = millis();
        romUpdateRequest = true;
      }
      break;

    case 0x64:  // '<'
      if (volumeKeyLocked) {
      } else {
        currVolume = mp3Module->volumeDown();
#ifdef __SERIAL_DEBUG__
        Serial.print(F("vol = "));
        Serial.println(currVolume);
#endif
        mp3Module->play(SND_VOLUME_DOWN);
        romUpdateRequestTime = millis();
        romUpdateRequest = true;
      }
      break;

    case 0x61: // Reset sentence buffer;
      if (sentenceBufferIdx == 0)
        break;

      sentenceBufferIdx = 0;
      spellBufferIdx = 0;
      mp3Module->play(SND_SENTENCE_RESTART);
#ifdef __SERIAL_DEBUG__
      Serial.println(F("\nReset sentence buffer and spell buffer!"));
#endif
      break;

    case 0x66: // Play sentence buffer
#ifdef __SERIAL_DEBUG__XX
      Serial.print(F("\nPlay sentence buffer: "));
#endif
      playSentenceFrom(SENTENCE_BUFFER);
      break;

    case 0x62: // Backspace a word
      if (sentenceBufferIdx == 0)
        break;

      sentenceBufferIdx--;
      mp3Module->playAndWait(SND_SENTENCE_DEL);
      delay(300);
      
      playSentenceFrom(SENTENCE_BUFFER);
      break;

    case 0x63: // dump spell buffer
      if (sentenceBufferIdx == 0) {
        // We should not save sentence to memo slot if sentenceBuffer is empty
        break;
      }
      mp3Module->play(SND_SENTENCE_MEMORIZE);
#ifdef __SERIAL_DEBUG__
      Serial.print(F("\nDump spell buffer: "));
      Serial.println(spellBuffer);
#endif
      break;

    case 0x51 ... 0x5A:
      if (prevScanCode == 0x63) {
        if (sentenceBufferIdx == 0) {
          // Clear saved sentence in memo slot if sentenceBuffer is empty
          saveSentenceToMemoSlot(scanCode & 0x0F);
          break;
        }

#ifdef __SERIAL_DEBUG__
        Serial.print(F("\nSave to slot "));
        Serial.println((scanCode & 0x0F));
#endif
        saveSentenceToMemoSlot(scanCode & 0x0F);

        // After record, we reset the sentenceBuffer and spellBuffer
        sentenceBufferIdx = 0;
        spellBufferIdx = 0;

        playSentenceFromMemoSlot(scanCode & 0x0F);
      } else {
        if (memoKeyBlocked) { // While in memoKey blocked mode , press memo key will play key number
          uint16_t sndIdx = 0;
          switch (scanCode & 0x0F) {
            case 0x01: sndIdx =SND_NO_ONE; break;
            case 0x02: sndIdx =SND_NO_TWO; break;
            case 0x03: sndIdx =SND_NO_THREE; break;
            case 0x04: sndIdx =SND_NO_FOUR; break;
            case 0x05: sndIdx =SND_NO_FIVE; break;
            case 0x06: sndIdx =SND_NO_SIX; break;
            case 0x07: sndIdx =SND_NO_SEVEN; break;
            case 0x08: sndIdx =SND_NO_EIGHT; break;
            case 0x09: sndIdx =SND_NO_NINE; break;
            case 0x0A: sndIdx =SND_NO_TEN; break;
          } // End of Switch
          mp3Module->play(sndIdx);
        } else {
#ifdef __SERIAL_DEBUG__
          Serial.print(F("\nPlay slot "));
          Serial.println(scanCode & 0x0F);
          Serial.print(F("prevScanCode=0x"));
          Serial.println(prevScanCode, HEX);
#endif
          playSentenceFromMemoSlot(scanCode & 0x0F);
        } // End of If memoKeyBlocked
      } // End of If prevScanCode == 0x63
      break;
      
    case 0x11: // Enable/Disable spell speed grows up sound effect
      result = true;
      if (prevScanCode == 0x63 && justInTime) {
        result = false;
        // spellSpeedupSoundEnabled switch
        spellSpeedupSoundEnabled = (spellSpeedupSoundEnabled) ? false : true;
        if (spellSpeedupSoundEnabled) {
          mp3Module->play(SND_DRIP);
        } else {
          mp3Module->play(SND_GLASS);
        }
        romUpdateRequestTime = millis();
        romUpdateRequest = true;
      }
      break;

    case 0x12: // report average spell time
      result = true;
      if (prevScanCode == 0x63 && justInTime) {
        mp3Module->stop();
        result = false;
        uint8_t value = 0;
        uint16_t sndIdx = 0;
        uint16_t div = 0;
        
        for (uint8_t i = 0; i < 3; i++) {
          switch (i) {
            case 0: div = 100; break;
            case 1: div = 10; break;
            case 2: div = 1; break;
          }
          value = (averageSpellTime / div) % 10;
#ifdef  __SERIAL_DEBUG__
          Serial.print(F("value="));
          Serial.println(value);
#endif
          playNumber(value);
        }
      }
      break;

    case 0x13:
      result = true;
      if (prevScanCode == 0x63 && justInTime) {
	// Read current voltage
        mp3Module->stop();
        result = false;
        uint16_t vbat = readVBat();
        uint8_t value = 0;
        uint16_t sndIdx = 0;
        uint16_t div = 0;
#ifdef  __SERIAL_DEBUG__
        Serial.print(F("vbat="));
        Serial.println(vbat);
#endif
        for (uint8_t i = 0; i < 4; i++) {
          switch (i) {
            case 0: div = 1000; break;
            case 1: div = 100; break;
            case 2: div = 10; break;
            case 3: div = 1; break;
          }
          value = (vbat / div) % 10;
#ifdef  __SERIAL_DEBUG__
          Serial.print(F("value="));
          Serial.println(value);
#endif
          playNumber(value);
        }
      }
      break;

    case 0x14:
      result = true;
      if (prevScanCode == 0x63 && justInTime) {
        result = false;
        // MemoKey block switch
        memoKeyBlocked = (memoKeyBlocked) ? false : true;
        if (memoKeyBlocked) {
          mp3Module->play(SND_DRIP);
        } else {
          mp3Module->play(SND_GLASS);
        }
        romUpdateRequestTime = millis();
        romUpdateRequest = true;
      }
      break;
      
    case 0x15:
      result = true;
      if (prevScanCode == 0x63 && justInTime) {
        result = false;
        // Play silence as click switch
        playSilenceAsSound = (playSilenceAsSound < 3) ? playSilenceAsSound+1 : 0;
        if (playSilenceAsSound == 0) {
          mp3Module->play(SND_DRIP);
        } else {
          mp3Module->play(SND_WOOD_FISH - 1 + playSilenceAsSound);
        }
        romUpdateRequestTime = millis();
        romUpdateRequest = true;
      }
      break;

    case 0x16:
      result = true;
      if (prevScanCode == 0x63 && justInTime) {
        result = false;
        // VolumeKey locked mode switch
        volumeKeyLocked = (volumeKeyLocked) ? false : true;
        if (volumeKeyLocked) {
          mp3Module->play(SND_DRIP);
        } else {
          mp3Module->play(SND_GLASS);
        }
        romUpdateRequestTime = millis();
        romUpdateRequest = true;
      }
      break;

    case 0x17:
      result = true;
      if (prevScanCode == 0x63 && justInTime) {
        result = false;
	// twice Mute enabl switch -- Prevent chewin key from being pressed several time
        twiceMuteEnabled = (twiceMuteEnabled) ? false : true;
        if (twiceMuteEnabled) {
          mp3Module->play(SND_DRIP);
        } else {
          mp3Module->play(SND_GLASS);
        }
        romUpdateRequestTime = millis();
        romUpdateRequest = true;
      }
      break;

    case 0x18:
      result = true;
      if (prevScanCode == 0x63 && justInTime) {
        result = false;
	// toneFix enabl switch
        toneFixEnabled = (toneFixEnabled) ? false : true;
        if (toneFixEnabled) {
          mp3Module->play(SND_DRIP);
        } else {
          mp3Module->play(SND_GLASS);
        }
        romUpdateRequestTime = millis();
        romUpdateRequest = true;
      }
      break;

    default:
      result = true;
  } // End of switch case

  prevScanCode = scanCode;
  prevScanTime = scanTime;

#ifdef __SERIAL_DEBUG__
  static chewinMapEntry * chewin;
  chewin = getChewinMapEntry(scanCode);
  if (chewin != NULL) {
    Serial.println(chewin->symbol);
  }
#endif
  if (result == true)
    processKeyCode(getChewinMapEntry(scanCode)->ascii, scanCode);
}

#define resetSpellBuffer \
  spellBufferIdx = 0; \
  firstScanCode = scanCode;

void Chewin::processKeyCode(char key, char scanCode) {
  static chewinMapEntry * chewin;
  static char firstScanCode;
  static uint8_t prevKeyType;
  static char prevKey = 0x0;

  uint16_t sndIdx = 0;

  // Play key sound
  if (key == NO_KEY)
    return;

  chewin = getChewinMapEntry(scanCode);
  if (chewin == NULL)
    return;

#ifdef __SERIAL_DEBUG_DEEP__
  switch (chewin->keyType) {
    case KEY_TYPE_A: Serial.println(F("KEY_TYPE_A")); break;
    case KEY_TYPE_B: Serial.println(F("KEY_TYPE_B")); break;
    case KEY_TYPE_C: Serial.println(F("KEY_TYPE_C")); break;
    case KEY_TYPE_D: Serial.println(F("KEY_TYPE_D")); break;
    case KEY_TYPE_E: Serial.println(F("KEY_TYPE_E")); break;
    case KEY_TYPE_F: Serial.println(F("KEY_TYPE_F")); break;
    case KEY_TYPE_G: Serial.println(F("KEY_TYPE_G")); break;
    default: Serial.println(F("KEY UNKNOWN!!"));
  }
#endif

  //if ((key != TONE_KEY1) && (key != TONE_KEY2) && (key != TONE_KEY3) && (key != TONE_KEY4) && (key != TONE_KEY5)) {

  if (chewin->keyType == KEY_TYPE_G)
    return;

  if (chewin->keyType != KEY_TYPE_F) {
    sndIdx = getKeySoundIdx(key);
    if (sndIdx != 0xFFFF) {
      if (twiceMuteEnabled) {
        // Check here to prevent hitting the same key repeatly
        if (key == prevKey && spellBufferIdx > 0) {

        } else {
          mp3Module->play(sndIdx);
          prevKey = key;
        }
      } else {
        mp3Module->play(sndIdx);
      }
    }
  }
  // End of play key sound

  // Collect keys to form spellBuffer
  if ((spellBufferIdx >= spellBufferSize)) {
    mp3Module->play(SND_SENTENCE_BUFFER_FULL_WARNING);
    return;
  }

#ifdef __SERIAL_DEBUG_XX__
  Serial.print(F("scanCode=0x"));
  Serial.print(scanCode, HEX);
  Serial.print(F(" spellBufferIdx="));
  Serial.println(spellBufferIdx);
#endif

  switch (chewin->keyType) {
    case KEY_TYPE_A:  // ㄅ .. ㄒ ㄓ .. ㄙ
    case KEY_TYPE_B:
    case KEY_TYPE_E:  // ㄦ
      resetSpellBuffer;
#ifdef __SERIAL_DEBUG_XX__
      Serial.print(F("set firstScanCode=0x")); \
      Serial.println(firstScanCode, HEX);
#endif
      spellBuffer[spellBufferIdx++] = key;
      spellBuffer[spellBufferIdx] = 0;
      prevKeyType = chewin->keyType;
      spellBeginTime = millis();
      break;

    case KEY_TYPE_F:  // [ ˊˇˋ˙=]
      if (key != TONE_KEY1) {
        if (key == SILENCE_KEY) { // silence processing

        } else {
          spellBuffer[spellBufferIdx++] = key;
          spellBuffer[spellBufferIdx] = 0;
        }
      }
      break;

    default:
      if (spellBufferIdx == 0) {
        resetSpellBuffer;
#ifdef __SERIAL_DEBUG_XX__
        Serial.print(F("set firstScanCode=0x")); \
        Serial.println(firstScanCode, HEX);
#endif
        spellBuffer[spellBufferIdx++] = key;
        spellBuffer[spellBufferIdx] = 0;
        prevKeyType = chewin->keyType;
        spellBeginTime = millis();
      } else {
        if (prevKeyType == chewin->keyType) {
          spellBuffer[spellBufferIdx - 1] = key;
          prevKeyType = chewin->keyType;
          spellBeginTime = millis();
        } else {
          if (prevKeyType == KEY_TYPE_D && chewin->keyType == KEY_TYPE_C) {
            mp3Module->play(SND_SPELL_ILLEGAL);
            spellBufferIdx = 0;
            // Illegal spell sequence
#ifdef __SERIAL_DEBUG__
            Serial.println(F("Illegal spell sequence!"));
#endif
            return;
          } else {
            spellBuffer[spellBufferIdx++] = key;
            spellBuffer[spellBufferIdx] = 0;
            prevKeyType = chewin->keyType;
          }
        }
      }
  } // End of switch

  if (chewin->keyType != KEY_TYPE_F) {
#ifdef __SERIAL_DEBUG_XX__
    Serial.println(F("keyType != tonekeys --> return"));
#endif
    return;
  }

  // When a chewin spell is completed!!
#ifdef __SERIAL_DEBUG__
  Serial.print(" ");
  Serial.print(spellBuffer);
#endif

#ifdef __SERIAL_DEBUG_XX__
  Serial.print(F(" firstScanCode=0x"));
  Serial.println(firstScanCode, HEX);
#endif

  if (key != SILENCE_KEY) {
    sndIdx = getSpellSoundIdx(spellBuffer, firstScanCode);
  } else {
    // When silence key is pressing
    sndIdx = SND_SILENCE;
  }

#ifdef __SERIAL_DEBUG_XX__
  Serial.print(F("sndIdx="));
  Serial.println(sndIdx);
#endif

  if (sndIdx != 0xFFFF) { // If spellList has this spell
    mp3Module->playAndWait(sndIdx);
    uint8_t spellTime = (uint8_t) ((millis() - spellBeginTime)/1000);
    uint8_t prevAverageSpellTime = averageSpellTime;

#ifdef __SERIAL_DEBUG_XX__
  Serial.print(F("spellTime="));
  Serial.println(spellTime);
  Serial.print(F("averageSpellTime="));
  Serial.println(averageSpellTime);
#endif

    if (averageSpellTime == 0) {
      averageSpellTime = spellTime;
      romUpdateRequestTime = millis();
      romUpdateRequest = true;
    } else {
      averageSpellTime = (averageSpellTime + spellTime)/2;
      if (averageSpellTime != prevAverageSpellTime) {
        romUpdateRequestTime = millis();
        romUpdateRequest = true;
      }
      if (spellTime < prevAverageSpellTime) {
#ifdef __SERIAL_DEBUG_XX__
  Serial.println(F("Average spell speed grows UP!"));
  Serial.print(prevAverageSpellTime);
  Serial.print(F(" --> "));
  Serial.println(averageSpellTime);
#endif
        if (spellSpeedupSoundEnabled) {
          mp3Module->play(SND_INPUT_SPEED_GROWUP);
        }
      }
    }

    if (sentenceBufferIdx < sentenceBufferSize) {
      if (key == SILENCE_KEY) {
        // Process Silence Key
        sentenceBuffer[sentenceBufferIdx].keys[0] = TONE_KEY1;
        sentenceBuffer[sentenceBufferIdx].keys[1] = 0x0;
      } else {
        strncpy(sentenceBuffer[sentenceBufferIdx].keys, spellBuffer, 5);
      }
      sentenceBuffer[sentenceBufferIdx].sndIndex = sndIdx;

      sentenceBufferIdx++;
    }

#ifdef __SERIAL_DEBUG__
    Serial.print(" ");
    Serial.print(sndIdx);
    Serial.println(F(".mp3"));
#endif
  } else { // If spellList has no such spell
    if (spellBufferIdx > 1)
      mp3Module->play(SND_SPELL_SOUND_NOT_PREPARED);
#ifdef __SERIAL_DEBUG_XX__
    Serial.println(F(" nothing matched!!"));
#endif
  } // End of if (sndIdx != 0xFFFF)
  // Reset spell buffer
  spellBufferIdx = 0;
  spellBuffer[0] = 0;
}

void Chewin::saveSentenceToMemoSlot(uint8_t slotIdx) {
  for (uint8_t i = 0; i < sentenceBufferIdx; i++) {
    memoSlot.sndIndex[i] = sentenceBuffer[i].sndIndex;
  }
  memoSlot.length = sentenceBufferIdx;

  slotIdx--;

  memoSlot.checkSum = 0;
  for (uint8_t i = 0; i < memoSlot.length; i++) {
    memoSlot.checkSum += memoSlot.sndIndex[i];
  }
  memoSlot.checkSum = (~memoSlot.checkSum) + 1;
#ifdef __SERIAL_DEBUG_DEEP__
  Serial.print(F("checkSum=0x"));
  Serial.println(memoSlot.checkSum, HEX);
#endif

  EEPROM.put(sizeof(eepromHeader) + sizeof(memoRecord) * (slotIdx), memoSlot);

  delay(100);
}

void Chewin::playSentenceFromMemoSlot(uint8_t slotIdx) {
  slotIdx--;

  EEPROM.get(sizeof(eepromHeader) + sizeof(memoRecord) * (slotIdx), memoSlot);

  uint32_t reCalcCheckSum = 0;
  for (uint8_t i = 0; i < memoSlot.length; i++) {
    reCalcCheckSum += memoSlot.sndIndex[i];
  }

  reCalcCheckSum = (~reCalcCheckSum) + 1;
  if (reCalcCheckSum != memoSlot.checkSum) {
#ifdef __SERIAL_DEBUG_DEEP__
    Serial.print(F(":e0x"));
    Serial.print(memoSlot.checkSum, HEX);
    Serial.print(" ");
#endif
    return;
  }

  playSentenceFrom(MEMO_SLOT);
}

void Chewin::updateEEprom() {
  eepromHeader header;

  header.volume = currVolume;
  header.mode = currMode;
  header.memoKeyBlocked = memoKeyBlocked;
  header.volumeKeyLocked = volumeKeyLocked;
  header.twiceMuteEnabled = twiceMuteEnabled;
  header.playSilenceAsSound = playSilenceAsSound;
  header.toneFixEnabled = toneFixEnabled;
  header.averageSpellTime = averageSpellTime;
  header.spellSpeedupSoundEnabled = spellSpeedupSoundEnabled;
  header.checkSum = (~(header.volume + header.mode + header.memoKeyBlocked + header.volumeKeyLocked + header.twiceMuteEnabled + header.playSilenceAsSound + header.toneFixEnabled + header.averageSpellTime + header.spellSpeedupSoundEnabled)) + 1; // 2's Complement
  EEPROM.put(0, header);
  delay(100);

#ifdef __SERIAL_DEBUG_XX__
  Serial.println(F("upEEprom(): Done!"));
#endif
  romUpdateRequest = false;
}

void Chewin::restoreFromEEprom() {
  uint8_t checkSum;
  eepromHeader header;

  EEPROM.get(0, header);
  checkSum = (~(header.volume + header.mode + header.memoKeyBlocked + header.volumeKeyLocked + header.twiceMuteEnabled + header.playSilenceAsSound + header.toneFixEnabled + header.averageSpellTime + header.spellSpeedupSoundEnabled)) + 1; // 2's Complement
  if (checkSum == header.checkSum) {
    currVolume = header.volume;
    currMode = header.mode;
    memoKeyBlocked = header.memoKeyBlocked;
    volumeKeyLocked = header.volumeKeyLocked;
    twiceMuteEnabled = header.twiceMuteEnabled;
    playSilenceAsSound = header.playSilenceAsSound;
    toneFixEnabled = header.toneFixEnabled;
    averageSpellTime = header.averageSpellTime;
    spellSpeedupSoundEnabled = header.spellSpeedupSoundEnabled;
#ifdef __SERIAL_DEBUG_XX__
    Serial.println(F("\nresEEprom(): Done!"));
    Serial.println(currVolume);
#endif
  } else {
    // report eeprom has error
    currVolume = defaultVolume;
#ifdef __SERIAL_DEBUG_XX__
    Serial.println(F("\nEEprom failed!!"));
    Serial.print(F(" checksum= 0x"));
    Serial.print(header.checkSum, HEX);
    Serial.println();
#endif
  }
}

void Chewin::doHousekeeping() {
  uint16_t vbat = 0;
  unsigned long currTime = millis();
  static unsigned long prevCheckVccTime;
  static unsigned long prevCheckVolumeTime;

  if (romUpdateRequest == true) {
    if ((currTime - romUpdateRequestTime) > romUpdateRequestDelay) {
      updateEEprom();
    }
  }

  if ((currTime - prevCheckVccTime) > checkVccPeriod) {
    vbat = readVBat();

    if (vbat < batteryLowThreshold) mp3Module->playAndWait(SND_IT_NEEDS_CHARGING);
    prevCheckVccTime = currTime;
  }
  
  if ((currTime - prevCheckVolumeTime) > checkVolumePeriod) {
  	mp3Module->volume(currVolume);
  }
}

void Chewin::updateShortCutTable() {
  uint8_t row;
  uint8_t col;
  uint8_t idx = 0;
  char key;
  quickEntry currEntry;
#ifdef __SERIAL_DEBUG_XX__
  Serial.println(F("updateShortCutTable"));
#endif

  // Once I have made a mistake in 發音檔整理_v19.ods BASIC MACRO on function prepSoundFrom(),
  // where I create shortCutTable entry with wrong condition rule ~
  //
  // if (row = 2) then
  //  addToSheet("All List", quickIndexRow, 4, "(const quickEntry PROGMEM) {" & trim(fileCount -1) & ", '" & Left(keyCell.String, 1) & "'}, // " & chewinCell.String)
  //  quickIndexRow = quickIndexRow + 1
  // endIf
  //
  // which will create a zero index for the first entry, and that will made
  // shortCutTable check to escape after checking the first entry.
  //
  // So, we should be careful to maintain the MACRO in LibreOffice, because
  // it is a possible place to generate wrong behaviros.
  //
  do {
    memcpy_P(&currEntry, &quickIndex[idx], sizeof(quickEntry));
#ifdef __SERIAL_DEBUG_DEEP__
    Serial.print(F("idx= "));
    Serial.print(idx);
    Serial.print(F(" currEntry.key= "));
    Serial.print(currEntry.key);
#endif

    for (row = 0; row < ROWS; row++) {
      for (col = 0; col < COLS; col++) {
        key = getChewinMapEntry(((row + 1) << 4)  + col + 1)->ascii;
#ifdef __SERIAL_DEBUG_DEEP__
        Serial.print(F(" key= "));
        Serial.print(key);
        Serial.println();
#endif

        if (key == currEntry.key) {
#ifdef __SERIAL_DEBUG_DEEP__
          Serial.print(F(" ascIIMap ["));
          Serial.print(row);
          Serial.print(F("]["));
          Serial.print(col);
          Serial.print(F("]= "));
          Serial.print(key);
          Serial.print(F(" <-- Matched!"));
          Serial.print(F(" currEntry.index= "));
          Serial.print(currEntry.index);
#endif
          shortCutTableForSound[row][col] = currEntry.index;
          row = ROWS;
          col = COLS;
#ifdef __SERIAL_DEBUG_DEEP__
          Serial.println();
#endif
        }
      }
    }

    idx++;
  } while (currEntry.index != 0);

#ifdef __SERIAL_DEBUG_XX__
  Serial.println(F("\nshortCutTable = {"));
  for (row = 0; row < ROWS; row++) {
    Serial.print(F("  {"));
    for (col = 0; col < COLS; col++) {
      Serial.print(F(" "));
      Serial.print(shortCutTableForSound[row][col]);
      Serial.print(F(","));
    }
    Serial.println(F("},"));
  }
  Serial.println(F("};"));
#endif
}

uint16_t Chewin::getSpellSoundIdx(char * keys, char firstScanCode) {
  uint16_t i = 0;
  uint8_t row;
  uint8_t col;
  uint16_t shortCut;
  row = (firstScanCode & 0xF0) >> 4;
  col = (firstScanCode & 0x0F);

  shortCut = shortCutTableForSound[row - 1][col - 1];
  if (shortCut != 0 && shortCut != 0xFFFF) {
#ifdef __SERIAL_DEBUG_XX__
    Serial.print(F(" Found short cut for "));
    Serial.print(keys);
    Serial.print(F(" shortCutIndex="));
    Serial.println(shortCut);
    Serial.print("chewinStartNumber=");
    Serial.println(chewinStartNumber);
#endif
    i = shortCut - chewinStartNumber;
  }

  do {
    getSpell(i);

    if (spellLookupBuffer.keys[0] == 0) {
      break;
    }

#ifdef __SERIAL_DEBUG_DEEP__
    Serial.print(F("i= "));
    Serial.print(i);
    Serial.print(" ");
    Serial.println(spellLookupBuffer.keys);
#endif

    if (strcmp(keys, spellLookupBuffer.keys) == 0) {
#ifdef __SERIAL_DEBUG_DEEP__
      Serial.print(F(" <-- Match!!\n"));
#endif
      return (i + chewinStartNumber);
    }
    i++;
  } while (i < 1500);
#ifdef __SERIAL_DEBUG_DEEP__
  Serial.println(F(" nothing matched!"));
#endif
  return 0xFFFF;
}
