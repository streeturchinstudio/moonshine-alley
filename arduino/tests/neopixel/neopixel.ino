#include <Adafruit_NeoPixel.h>

#define PIN   2
#define LEDS  24
 
// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(24, PIN, NEO_GRB + NEO_KHZ800);
uint32_t yellow = strip.Color(255, 160, 102);
uint32_t red = strip.Color(255, 0, 0);
uint32_t half_red = strip.Color(128, 0, 0);
uint32_t off = strip.Color(0, 0, 0);
int intensity = 0;
int amt = 10;

void setup() {
  // put your setup code here, to run once:
  strip.begin();
  strip.show();
  
//  strip.setPixelColor(11, 255, 0, 255);
//  for (int x=0; x<LEDS; x++) {
//    strip.setPixelColor(x, yellow);
//  }
  strip.setBrightness(255);
  strip.show();

}

void loop() {
  // put your main code here, to run repeatedly:
  for (int x=0; x < LEDS; x++) {
    strip.setPixelColor(x, yellow);
//    if (x == 0) {
//      strip.setPixelColor(LEDS-1, red);
//    } else {
//      strip.setPixelColor(x-1, red);
//    }
    
    for (int y=0; y < LEDS; y++) {
      if (y == x) {
        continue;
      }
      strip.setPixelColor(y, intensity, 0, 0);
    }
    intensity += amt;
    if (intensity > 255) {
      intensity = 255;
      amt = -10;
    } else if (intensity < 10) {
      intensity = 10;
      amt = 10;
    }
    strip.show();
    delay(50);
  }
}
