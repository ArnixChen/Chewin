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
#define version "ChewinBox-01"

#include <hidboot.h>
//#include <usbhub.h>

#include <avr/pgmspace.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

#include <Chewin.h>
Chewin myChewin;

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

  myChewin.begin(3, 2); // pin 3 for DFPlayerMini TX, pin 2 for DFPlayerMini RX
}

void loop() {
  Usb.Task();
}