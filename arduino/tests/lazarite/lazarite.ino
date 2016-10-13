/*
 * Cornstarch + water. 1:1 by weight.
 * 20 - 50Hz for best results.
 *
 */
#include <Arduino.h>


#define OUTPUT_PIN  8

// Interblogs say 20 - 50Hz is best. Might tear speaker :-(
#define INIT_FREQUENCY   44

#define BTN_0   2
#define BTN_1   3


int frequency;


void setup() {
    pinMode(BTN_0, INPUT);
    pinMode(BTN_1, INPUT);

    digitalWrite(BTN_0, HIGH);
    digitalWrite(BTN_1, HIGH);

    // set a fixed tone
    frequency = INIT_FREQUENCY;
    pinMode(OUTPUT_PIN, OUTPUT);
    tone(OUTPUT_PIN, frequency);
}


void loop() {
    if (digitalRead(BTN_0) == LOW) {
        frequency += 1;
        tone(OUTPUT_PIN, frequency);
    } else if (digitalRead(BTN_1) == LOW) {
        frequency -= 1;
        tone(OUTPUT_PIN, frequency);
    }
    delay(250);
}
