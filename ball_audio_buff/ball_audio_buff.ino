// Double-buffered 8-bit Adafruit_GFX-compatible framebuffer for PicoDVI.
// Animates without redraw flicker. Requires Adafruit_GFX >= 1.11.4

#include <PicoDVI.h>

// 8 bit double buffer
DVIGFX8 display(DVI_RES_320x240p60, true, adafruit_feather_dvi_cfg);

int16_t w,h;
int16_t counter, bigCount;
float speed;
int8_t dir; 
float audioBuff[10];
bool persist;

int pin1 = A0;
int pin2 = A1;
int pin3 = A2;
int pin4 = A3;


int raw_audIn = 0;  // variable to store the value read
int raw_k1 = 0;
int raw_k2 = 0;
int raw_k3 = 0;


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
  dir = 1;
  speed = 1;
  counter = 1;
  
}

void loop() {
  raw_audIn = analogRead( pin1 );  // read the input pin for the audio
  raw_k1 = analogRead( pin2 );  // read the input pin pot 1
  raw_k2 = analogRead( pin3 );  // read the input pin pot 2
  raw_k3 = analogRead( pin4 );  // read the input pin pot 3

  //Serial.println(val);          // debug value
  counter++;
  if(counter>10){
    counter = 0;
    if(persist==true){
      persist = false;
    } else {
      persist = true;
    }
  } 
  for(int i=0; i<10; i++) {
    audioBuff[i] = scale_analogIn(raw_audIn,1,0);
  }
  //audioBuff[counter] = scale_analogIn(raw_audIn,1,0);
  int red = scale_analogIn(raw_k1,255,0);
  int green = scale_analogIn(raw_k2,255,0);
  int blue = scale_analogIn(raw_k3,255,0);
  
  // Clear back framebuffer and draw circle

  display.fillScreen(0);
  display.setColor(1, color_mixer(red,green,blue) );
  
  // draw three circles
  display.fillCircle(10, audioBuff[0]*h, 20, 1);
  display.fillCircle(w/2, audioBuff[178]*h, 20, 1);
  display.fillCircle(w-10, audioBuff[255]*h, 20, 1);

  display.drawLine(0,h/2,w,h/2,1);
  
  // Swap front/back buffers, do not duplicate current screen state to next frame,
  // we'll draw it new from scratch each time.
  display.swap();
}

uint16_t color_mixer(int int_r, int int_g, int int_b) {
  uint16_t mixed = ((int_r & 0xf8) << 8) + ((int_g & 0xfc) << 3) + (int_b >>3);
  return mixed;
}

float scale_analogIn(int audio, int size, int min) {
  float scale = (float)audio / 1023.0;
  return (scale*size) + min;
}




