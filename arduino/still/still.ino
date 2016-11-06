// Decoy Still - Fake fire

// https://github.com/adafruit/Adafruit_NeoPixel
#include <Adafruit_NeoPixel.h>

#define PIN   2
#define LEDS  24

#define INPUT1  4
#define INPUT2  5


Adafruit_NeoPixel strip = Adafruit_NeoPixel(24, PIN, NEO_GRB + NEO_KHZ800);
uint32_t off = strip.Color(0, 0, 0);
int brightness;
String buffer;


void setup() {
    pinMode(INPUT1, INPUT);
    pinMode(INPUT2, INPUT);

    strip.begin();
    strip.setBrightness(255);
    strip.show();

    brightness = 127;
    buffer = "";

    Serial.begin(9600);
    Serial.println("type in a new brightness level (1-256)...");
}


void readBrightness() {
    int a = digitalRead(INPUT1);
    int b = digitalRead(INPUT2);

    if (a == LOW && b == LOW) {
        brightness = 0;
    } else if (a == LOW && b == HIGH) {
        brightness = 40;
    } else if (a == HIGH && b == LOW) {
        brightness = 80;
    } else if (a == HIGH && b == HIGH) {
        brightness = 127;
    }
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

    readBrightness();
    if (brightness == 0) {
        for (int x=0; x< LEDS; x++) {
            strip.setPixelColor(x, off);
        }
        strip.show();
        delay(200);
        return;
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