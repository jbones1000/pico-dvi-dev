// Double-buffered 8-bit Adafruit_GFX-compatible framebuffer for PicoDVI.
// Animates without redraw flicker. Requires Adafruit_GFX >= 1.11.4

#include <PicoDVI.h>

// 8 bit double buffer
DVIGFX8 display(DVI_RES_320x240p60, true, adafruit_feather_dvi_cfg);

int16_t w,h;
int16_t count, bigCount;
float speed;
int8_t dir; 

// colors
#define BLACK    0x0000
#define BLUE     0x001F
#define RED      0xF800
#define GREEN    0x07E0
#define CYAN     0x07FF
#define MAGENTA  0xF81F
#define YELLOW   0xFFE0 
#define WHITE    0xFFFF

struct {
  int16_t pos[3];
  
} linez[32];

void setup() { // Runs once on startup
  if (!display.begin()) { // Blink LED if insufficient RAM
    pinMode(LED_BUILTIN, OUTPUT);
    for (;;) digitalWrite(LED_BUILTIN, (millis() / 500) & 1);
  }
  
  w = display.width();
  h = display.height();
  dir = 1;
  speed = 1;

  for(int i=0; i<32; i++) {
    linez[i].pos[0] = 0;
    linez[i].pos[1] = 30;
    linez[i].pos[2] = 60;
  }
  
  
}

void loop() {
  count = count + (speed*dir);
  if( count > (w-10) ){
    dir = -1;
  }
  if(count<10){
    dir = 1;
  }
  // Clear back framebuffer and draw circle
  display.fillScreen(0);
  // draw the lines
  for(int i=0; i<16; i++) {
    display.setColor(1, color_mixer(255,2,200));
    display.setColor(2, color_mixer(120,1,100));
    display.setColor(3, color_mixer(25,0,20));
    
    display.drawLine(linez[i].pos[0], h/2, h-(i*20), h, 1);
    display.drawLine(linez[i].pos[1], h/2, h-(i*20), h, 2);
    display.drawLine(linez[i].pos[2], h/2, h-(i*20), h, 3);
  }
  
  for(int i=0; i<16; i++) {
    display.setColor(1, color_mixer(255,2,200));
    display.setColor(2, color_mixer(120,1,100));
    display.setColor(3, color_mixer(50,0,50));
    
    display.drawLine(linez[i+16].pos[0], h/2, h-(i*20), 0, 1);
    display.drawLine(linez[i+16].pos[1], h/2, h-(i*20), 0, 2);
    display.drawLine(linez[i+16].pos[2], h/2, h-(i*20), 0, 3);
  }
  
  // move the lines
  for(int i=0; i<32; i++) {
    linez[i].pos[2] = linez[i].pos[1];
    linez[i].pos[1] = linez[i].pos[0];
    linez[i].pos[0] = count;
  }
  // Swap front/back buffers, do not duplicate current screen state to next frame,
  // we'll draw it new from scratch each time.
  display.swap();
}

uint16_t color_mixer(int int_r, int int_g, int int_b) {
  uint16_t mixed = ((int_r & 0xf8) << 8) + ((int_g & 0xfc) << 3) + (int_b >>3);
  return mixed;
}


