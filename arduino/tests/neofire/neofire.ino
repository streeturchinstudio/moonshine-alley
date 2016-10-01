// Fake fire test

// https://github.com/adafruit/Adafruit_NeoPixel
#include <Adafruit_NeoPixel.h>

#define PIN   2
#define LEDS  24


Adafruit_NeoPixel strip = Adafruit_NeoPixel(24, PIN, NEO_GRB + NEO_KHZ800);
int brightness;
String buffer;


void setup() {
    strip.begin();
    strip.setBrightness(255);
    strip.show();

    brightness = 127;
    buffer = "";

    Serial.begin(9600);
    Serial.println("type in a new brightness level (1-256)...");
}

void loop() {
    if (Serial.available()) {
        char c = Serial.read();
        if ( c == '\n') {
            brightness = buffer.toInt();
            buffer = "";
        } else if (c >= '0' && c <= '9') {
            buffer += c;
        }
    }

    int r1 = min(255, brightness + random(brightness/2));
    int g1 = r1/2;

    int r2 = min(255, brightness + random(brightness/2));
    int g2 = r2/2;

    int r3 = random(brightness);

    for (int x=0; x < LEDS; x+=3) {
        strip.setPixelColor(x, r1, g1, 0);
        strip.setPixelColor(x+1, r2, g2, 0);
        strip.setPixelColor(x+2, r3, 0, 0);
    }
    strip.show();
    delay(random(20, 150));
}