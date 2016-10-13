/*
 * Cornstarch + water. 1:1 by weight.
 * 20 - 50Hz for best results.
 *
 */
#include <Arduino.h>


#define OUTPUT_PIN  8

// Interblogs say 20 - 50Hz is best. Might tear speaker :-(
#define FREQUENCY   44

#define ENCODER_0   2
#define ENCODER_1   3


// volatile int enc0 = 0;
// volatile int enc1 = 0;


// void enc0Select() {
// }

// void enc1Select() {
// }


void setup() {
    // attachInterrupt(ENCODER_0, enc0Select, RISING);
    // attachInterrupt(ENCODER_1, enc1Select, RISING);

    // set a fixed tone
    pinMode(OUTPUT_PIN, OUTPUT);
    tone(OUTPUT_PIN, FREQUENCY);
}


void loop() {

}
