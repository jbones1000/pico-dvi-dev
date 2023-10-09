// Double-buffered 8-bit Adafruit_GFX-compatible framebuffer for PicoDVI.
// Animates without redraw flicker. Requires Adafruit_GFX >= 1.11.4

#include <PicoDVI.h>

// 8 bit double buffer
DVIGFX8 display(DVI_RES_320x240p60, true, adafruit_feather_dvi_cfg);
// global width and height
int16_t w,h;
// define an rgb struct to hold color data

struct rgb_vec {
  int8_t red; // red
  int8_t grn; // green
  int8_t blu; // blue
};

rgb_vec first_col;
rgb_vec second_col;
rgb_vec grad_step;

// analog sensors
int pin1 = A0;
int pin2 = A1;
int pin3 = A2;
int pin4 = A3;

// analog global parameters
int raw_audIn = 0;  // variable to store the value read
int raw_k1 = 0;
int raw_k2 = 0;
int raw_k3 = 0;

int red,green,blue;
int wowCol;

// colors
#define BLACK    0x0000
#define BLUE     0x001F
#define RED      0xF800
#define GREEN    0x07E0
#define CYAN     0x07FF
#define MAGENTA  0xF81F
#define YELLOW   0xFFE0 
#define WHITE    0xFFFF

void setup() { // Runs once on startup
  if (!display.begin()) { // Blink LED if insufficient RAM
    pinMode(LED_BUILTIN, OUTPUT);
    for (;;) digitalWrite(LED_BUILTIN, (millis() / 500) & 1);
  }

  Serial.begin(9600);           //  setup serial
  
  w = display.width();
  h = display.height();
  first_col.red = 0;
  first_col.blu = 125;
  first_col.grn = 255;
  second_col.red = 200;
  second_col.blu = 0;
  second_col.grn = 40;
  grad_step.red = 1;
  grad_step.grn = 1;
  grad_step.blu = 1;

}

void loop() {
  // update global parameters of analog control
  raw_audIn = analogRead( pin1 );  // read the input pin for the audio
  raw_k1 = analogRead( pin2 );  // read the input pin pot 1
  raw_k2 = analogRead( pin3 );  // read the input pin pot 2
  raw_k3 = analogRead( pin4 );  // read the input pin pot 3
  
  // Clear back framebuffer and draw circle
  display.setColor(BLACK,0);
  display.fillScreen(0);
  
  first_col.grn = int8_t( scale_analogIn(raw_k1,255,0) );
  grad_step.red = scale_analogIn(raw_k2,40,-20 );
  //display.setColor(1, color_mixer(scale_analogIn(raw_k2,255,0), 0, 0));
  //display.drawCircle( w/2, h/2, 20, 1);
  drawGrad(0,0,w,h, first_col, grad_step);
  
  /*
  int shift = scale_analogIn(raw_k1,255,0);
  for( int i=1; i<=w; i++) {
    display.setColor(i, color_mixer( i%255, shift, 150));
    display.drawFastVLine(20+i, 20, h-20, i);
  }
  */

  // Swap front/back buffers, do not duplicate current screen state to next frame,
  // we'll draw it new from scratch each time.
  display.swap();
}

// FUNCTIONS

uint16_t color_mixer(int int_r, int int_g, int int_b) {
  uint16_t mixed = ((int_r & 0xf8) << 8) + ((int_g & 0xfc) << 3) + (int_b >>3);
  return mixed;
}

float scale_analogIn(int audio, int size, int min) {
  float scale = float( audio / 1023.0 );
  return (scale*size) + min;
}

void drawGrad(int x, int y, int width, int height, rgb_vec col1, rgb_vec step) {
  int r_space = step.red;
  int g_space = step.grn;
  int b_space = step.blu;
  // i = column
  for( int i=1; i<=width; i++){
    //display.setColor(i, color_mixer( 20+(i*10), 122, 200));
    
    display.setColor(i, color_mixer( 
                          (col1.red + ( r_space*i )) % 255, 
                          (col1.grn + ( g_space*i )) % 255, 
                          (col1.blu + ( b_space*i )) % 255  ) );
    
    display.drawFastVLine(i+x, y, height, i);
  }
}


