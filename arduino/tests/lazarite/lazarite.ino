/*
 * Cornstarch + water. 1:1 by weight.
 * 20 - 50Hz for best results.
 *
 */
#include <Arduino.h>


// Interblogs say 20 - 50Hz is best. Might tear speaker :-(
// Speaker/AMP wont go below 30
#define FREQUENCY   40
#define OUTPUT_PIN  8


void setup() {
    pinMode(OUTPUT_PIN, OUTPUT);
    tone(OUTPUT_PIN, FREQUENCY);
}


void loop() {
}
