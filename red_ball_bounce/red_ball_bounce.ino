// Double-buffered 8-bit Adafruit_GFX-compatible framebuffer for PicoDVI.
// Animates without redraw flicker. Requires Adafruit_GFX >= 1.11.4

#include <PicoDVI.h>

// 8 bit double buffer
DVIGFX8 display(DVI_RES_320x240p60, true, adafruit_feather_dvi_cfg);

int16_t w,h;
int16_t counter, bigCount;
float speed;
int8_t dir; 



int analogPin = A0; // potentiometer wiper (middle terminal) connected to analog pin 3
                    // outside leads to ground and +5V
int val = 0;  // variable to store the value read

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
  val = analogRead(analogPin);  // read the input pin
  //Serial.println(val);          // debug value
  //float scale = (float)val / 1023.0;
  float moveY = scale_audioIn(val,h,0);

  
  // Clear back framebuffer and draw circle
  display.fillScreen(0);
  display.setColor(1, color_mixer(200,2,200));
  display.setColor(2, color_mixer(200,200,200));
  display.setColor(3, color_mixer(10,250,100));
  

  display.fillCircle(10, moveY, 20, 1);
  display.fillCircle(w/2, moveY, 20, 2);
  display.fillCircle(w-10, moveY, 20, 3);
  
  // Swap front/back buffers, do not duplicate current screen state to next frame,
  // we'll draw it new from scratch each time.
  display.swap();
}

uint16_t color_mixer(int int_r, int int_g, int int_b) {
  uint16_t mixed = ((int_r & 0xf8) << 8) + ((int_g & 0xfc) << 3) + (int_b >>3);
  return mixed;
}

float scale_audioIn(int audio, int size, int min) {
  float scale = (float)audio / 1023.0;
  return (scale*size) + min;
}

