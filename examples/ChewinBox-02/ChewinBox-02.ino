/*
   ChewinBox.ino -
   Main program that adapt Chewin Class member functions to KeyboardReportParser(with USB-Host-Shield),
   to provide PC-Keyboard as a Chewin AAC device.

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

   Modified 07-23-2019 by Arnix Chen

*/
#define version "ChewinBox-02"

#include <hidboot.h>
//#include <usbhub.h>

#include <avr/pgmspace.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

#include <Chewin.h>

// User customized ChewinMap
const chewinMapEntry myChewinMap [7][10] PROGMEM = {
  { {"ㄅ", '1', KEY_TYPE_A, 0x1E}, {"ㄉ", '2', KEY_TYPE_A, 0x1F}, {"ˇ", TONE_KEY3, KEY_TYPE_F, 0x20}, {"ˋ", TONE_KEY4, KEY_TYPE_F, 0x21}, {"ㄓ", '5', KEY_TYPE_A, 0x22},
    {"ˊ", TONE_KEY2, KEY_TYPE_F, 0x23}, {"˙", TONE_KEY5, KEY_TYPE_F, 0x24}, {"ㄚ", '8', KEY_TYPE_D, 0x25}, {"ㄞ", '9', KEY_TYPE_D, 0x26}, {"ㄢ", '0', KEY_TYPE_D, 0x27}
  },

  { {"ㄆ", 'q', KEY_TYPE_A, 0x14}, {"ㄊ", 'w', KEY_TYPE_A, 0x1A}, {"ㄍ", 'e', KEY_TYPE_A, 0x08}, {"ㄐ", 'r', KEY_TYPE_A, 0x15}, {"ㄔ", 't', KEY_TYPE_A, 0x17},
    {"ㄗ", 'y', KEY_TYPE_A, 0x1C}, {"ㄧ", 'u', KEY_TYPE_C, 0x18}, {"ㄛ", 'i', KEY_TYPE_D, 0x0C}, {"ㄟ", 'o', KEY_TYPE_D, 0x12}, {"ㄣ", 'p', KEY_TYPE_D, 0x13}
  },

  { {"ㄇ", 'a', KEY_TYPE_A, 0x04}, {"ㄋ", 's', KEY_TYPE_A, 0x16}, {"ㄎ", 'd', KEY_TYPE_A, 0x07}, {"ㄑ", 'f', KEY_TYPE_A, 0x09}, {"ㄕ", 'g', KEY_TYPE_A, 0x0A},
    {"ㄘ", 'h', KEY_TYPE_A, 0x0B}, {"ㄨ", 'j', KEY_TYPE_C, 0x0D}, {"ㄜ", 'k', KEY_TYPE_D, 0x0E}, {"ㄠ", 'l', KEY_TYPE_D, 0x0F}, {"ㄤ", ';', KEY_TYPE_D, 0x33}
  },

  { {"ㄈ", 'z', KEY_TYPE_A, 0x1D}, {"ㄌ", 'x', KEY_TYPE_A, 0x1B}, {"ㄏ", 'c', KEY_TYPE_A, 0x06}, {"ㄒ", 'v', KEY_TYPE_A, 0x19}, {"ㄖ", 'b', KEY_TYPE_A, 0x05},
    {"ㄙ", 'n', KEY_TYPE_A, 0x11}, {"ㄩ", 'm', KEY_TYPE_C, 0x10}, {"ㄝ", ',', KEY_TYPE_D, 0x36}, {"ㄡ", '.', KEY_TYPE_D, 0x37}, {"ㄥ", '/', KEY_TYPE_D, 0x38}
  },

  { {"", '!', KEY_TYPE_G, 0x3A}, {"", '@', KEY_TYPE_G, 0x3B}, {"", '#', KEY_TYPE_G, 0x3C}, {"", '$', KEY_TYPE_G, 0x3D}, {"", '%', KEY_TYPE_G, 0x3E},
    {"", '^', KEY_TYPE_G, 0x3F}, {"", '&', KEY_TYPE_G, 0x40}, {"", '*', KEY_TYPE_G, 0x41}, {"", '(', KEY_TYPE_G, 0x42}, {"", ')', KEY_TYPE_G, 0x43}
  },

  { {"", '~', KEY_TYPE_G, 0x29}, {"", '{', KEY_TYPE_G, 0x2A}, {"", '}', KEY_TYPE_G, 0x45}, {"", '[', KEY_TYPE_G, 0x2F}, {"", ']', KEY_TYPE_G, 0x30},
    {"", '\\', KEY_TYPE_G, 0x28}, {"ㄦ", '-', KEY_TYPE_E, 0x2D}, {" ", TONE_KEY1, KEY_TYPE_F, 0x2C}, {"", '<', KEY_TYPE_G, 0x36}, {"", '>', KEY_TYPE_G, 0x37}
  },

  { {"", SILENCE_KEY, KEY_TYPE_F, 0x2E}, {"", 0x0, KEY_TYPE_G, 0x00}, {"", 0x0, KEY_TYPE_G, 0x00}, {"", 0x0, KEY_TYPE_G, 0x00}, {"", 0x0, KEY_TYPE_G, 0x00},
    {"", 0x0, KEY_TYPE_G, 0x00}, {"", 0x0, KEY_TYPE_G, 0x00}, {"", 0x0, KEY_TYPE_G, 0x00}, {"", 0x0, KEY_TYPE_G, 0x00}, {"", 0x0, KEY_TYPE_G, 0x00}
  },
};

// User customized Chewin Class
class MyChewin: public Chewin {
 public:

  MyChewin(uint8_t rows, uint8_t cols, const chewinMapEntry* chewinMap): Chewin(rows, cols, chewinMap) {
  }

  void doHousekeeping() {
    Chewin::doHousekeeping();
  }

  void processScanCode(char scanCode) {
    Serial.print("scanCode=0x");
    Serial.println(scanCode, HEX);
    Chewin::processScanCode(scanCode);

    switch (scanCode) {
      case 0x53:
        if (sentenceBufferIdx == 0)
          break;

        for (int i = 0; i < sentenceBufferIdx; i++) {
#ifdef __SERIAL_DEBUG__
          Serial.print(sentenceBuffer[i].keys);
          Serial.print(" ");
#endif
          _mp3Module->playAndWait(sentenceBuffer[i].sndIndex);
        }
#ifdef __SERIAL_DEBUG__
        Serial.println();
#endif
        break;

    }
  }

  void processKeyCode(char key, char scanCode) {
    Chewin::processKeyCode(key, scanCode);
  }

};
MyChewin myChewin(7, 10, &myChewinMap[0][0]); // pass customized ChewinMap for initializing Chewin object, where 7 for ROWS, 10 for COLS.

class KbdRptParser : public KeyboardReportParser {
 protected:
  void OnKeyDown	(uint8_t mod, uint8_t key);
};

void KbdRptParser::OnKeyDown(uint8_t mod, uint8_t key) {
  char scanCode = myChewin.getScanCodeFromHID(mod, key);

  if (scanCode == NO_KEY) {
    // Let's do works for housekeeping
    myChewin.doHousekeeping();
  } else {
    myChewin.processScanCode(scanCode);
  }
}

USB     Usb;
HIDBoot<USB_HID_PROTOCOL_KEYBOARD>    HidKeyboard(&Usb);
KbdRptParser Prs;

void setup() {

  Serial.begin( 9600 );
#if !defined(__MIPSEL__)
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  Serial.println("Start");
  // Use F() macro to store version string in FLASH
  Serial.println(F(version));

  if (Usb.Init() == -1)
    Serial.println("OSC did not start.");
  delay( 200 );

  HidKeyboard.SetReportParser(0, &Prs);

  pinMode(ledPin, OUTPUT);

  myChewin.audioInit(3, 2); // pin 3 for DFPlayerMini TX, pin 2 for DFPlayerMini RX
}

void loop() {
  Usb.Task();
}