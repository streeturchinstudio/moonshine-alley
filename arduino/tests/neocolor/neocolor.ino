// Tool for testing colors

// https://github.com/adafruit/Adafruit_NeoPixel
#include <Adafruit_NeoPixel.h>

#define PIN   2
#define LEDS  24


Adafruit_NeoPixel strip = Adafruit_NeoPixel(24, PIN, NEO_GRB + NEO_KHZ800);
int r;
int g;
int b;
String buffer;


void printColor(int r, int g, int b) {
    Serial.println("");
    Serial.print("R: ");
    Serial.print(r);
    Serial.print(", G: ");
    Serial.print(g);
    Serial.print(", B: ");
    Serial.println(b);
}

void processBuffer() {
    buffer.toLowerCase();
    char c = buffer.charAt(0);
    int value = buffer.substring(1, buffer.length()-1).toInt();
    switch (c) {
        case 'r':
            r = value;
            break;
        case 'g':
            g = value;
            break;
        case 'b':
            b = value;
            break;
        default:
            Serial.println("error");
            return;
    }

    for (int x=0; x< LEDS; x++) {
        strip.setPixelColor(x, r, g, b);
    }
    strip.show();
    printColor(r, g, b);
}

void setup() {
    r = 128;
    g = 128;
    b = 128;
    buffer = "";

    strip.begin();
    strip.setBrightness(255);
    for (int x=0; x< LEDS; x++) {
        strip.setPixelColor(x, r, g, b);
    }
    strip.show();

    Serial.begin(9600);
    Serial.println("Format: {r|g|b}<0-255>");
}

void loop() {
    if (Serial.available()) {
        char c = Serial.read();
        if ( c == '\n') {
            processBuffer();
            buffer = "";
        } else {
            buffer += c;
        }
    }
    delay(100);
}
