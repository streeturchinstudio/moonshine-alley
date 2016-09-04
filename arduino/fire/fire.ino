#include <arduino.h>


#define YELLOW1     9
#define YELLOW2     10
#define RED1        11

#define RAND_RANGE       120
#define BRIGHTNESS  135


void setup() {
    pinMode(YELLOW1, OUTPUT);
    pinMode(YELL, OUTPUT);
    pinMode(RED1, OUTPUT);
}


void loop() {
    analogWrite(YELLOW1, random(RAND_RANGE) + BRIGHTNESS);
    analogWrite(YELLOW2, random(RAND_RANGE) + BRIGHTNESS);
    analogWrite(RED1, random(RAND_RANGE) + BRIGHTNESS);
    delay(random(100));
}