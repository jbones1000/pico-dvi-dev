// SPDX-FileCopyrightText: 2023 Liz Clark for Adafruit Industries
// SPDX-License-Identifier: MIT

#include <PicoDVI.h>
#include <Fonts/FreeSansBold18pt7b.h> // A custom font

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

// global variable
int w;
int h;

// setup
void setup() { 
  if (!display.begin()) { // Blink LED if insufficient RAM
    pinMode(LED_BUILTIN, OUTPUT);
    for (;;) digitalWrite(LED_BUILTIN, (millis() / 500) & 1);
  }

  w = display.width();
  h = display.height();


}

// draw loop
void loop() {
  //display.fillCircle(w/2, h/2, 65, MAGENTA);
  display.fillScreen(0);
  display.drawRect(100, 100, 50, 50, RED);
  
  
}