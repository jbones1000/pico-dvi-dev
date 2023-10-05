// SPDX-FileCopyrightText: 2023 Liz Clark for Adafruit Industries
// SPDX-License-Identifier: MIT

#include <PicoDVI.h>
// move a rectangle to the right

DVIGFX16 display(DVI_RES_320x240p60, adafruit_feather_dvi_cfg);

// colors
#define BLACK    0x0000
#define BLUE     0x001F
#define RED      0xF800
#define GREEN    0x07E0
#define CYAN     0x07FF
#define MAGENTA  0xF81F
#define YELLOW   0xFFE0 
#define WHITE    0xFFFF

// global variables
int w;
int h;
int count;
int bigCount;


// setup
void setup() { 
  if (!display.begin()) { // Blink LED if insufficient RAM
    pinMode(LED_BUILTIN, OUTPUT);
    for (;;) digitalWrite(LED_BUILTIN, (millis() / 500) & 1);
  }
  display.setColor(255, 0xFFFF); // Last palette entry = white
  display.swap(false, true); // Duplicate same palette into front & back buffers
  count = 0;
  bigCount = 0;
  w = display.width();
  h = display.height();
}

// draw loop
void loop() {
  // count loop
  if( count == 145 ) {
    count = 0;
    bigCount++;
  } else {
    count++;
  }

  if( bigCount == w ) {
    bigCount = 0;
  }
  moveRect();
}

void moveRect() {
    display.fillScreen(0);
    display.drawRect(bigCount, h/2, 60, 60, RED);
}