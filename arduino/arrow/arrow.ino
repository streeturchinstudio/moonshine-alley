// Arrow sign

#include <SPI.h>
// https://github.com/TMRh20/RF24
#include <RF24.h>

#define RADIO_ENABLE_PIN    9
#define RADIO_SELECT_PIN    10
#define BLINK_CODE          1337
#define BLINK_PIN           2


RF24 radio(RADIO_ENABLE_PIN, RADIO_SELECT_PIN);
byte RADIO_ADDRESS[6] = "Arrow";


void setup() {
    pinMode(BLINK_PIN, OUTPUT);
    digitalWrite(BLINK_PIN, LOW);

    radio.begin();
    radio.setPALevel(RF24_PA_LOW);

    radio.openReadingPipe(1, RADIO_ADDRESS);
    radio.startListening();

    blink(2);
}


void blink(uint8_t count) {
    for (int x=0; x<count; x++) {
        digitalWrite(BLINK_PIN, HIGH);
        delay(500);
        digitalWrite(BLINK_PIN, LOW);
        delay(500);
    }
}


void loop() {
    unsigned long code = 0;
    while (radio.available()) {
        radio.read(&code, sizeof(unsigned long));
        if (code == BLINK_CODE) {
            blink(8);
        }
        delay(100);
    }
    delay(100);
}