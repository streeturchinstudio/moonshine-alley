// Still

#include <SPI.h>
// https://github.com/TMRh20/RF24
#include <RF24.h>

#define RADIO_ENABLE_PIN    9
#define RADIO_SELECT_PIN    10
#define BLINK_CODE          1337
#define RETRIES             10


RF24 radio(RADIO_ENABLE_PIN, RADIO_SELECT_PIN);
byte RADIO_ADDRESS[6] = "Arrow";


void setup() {
    // TODO: setup bellow input
    // TODO: setup ringer

    Serial.begin(9600);

    radio.begin();
    radio.setPALevel(RF24_PA_LOW);

    radio.openWritingPipe(RADIO_ADDRESS);
    radio.stopListening();
    Serial.println("Press 'T' to transmit...");
}


void loop() {
    // TODO: create real trigger
    if (Serial.available()) {
        char c = toupper(Serial.read());
        if (c == 'T') {
            Serial.print("\nTransmitting...");
            unsigned long code = BLINK_CODE;
            for (int x=0; x< RETRIES; x++) {
                if (radio.write(&code, sizeof(unsigned long))) {
                    Serial.println("Received");
                    return;
                } else {
                    Serial.println("failed");
                    delay(100);
                }
            }
            Serial.println("Gave up");
        }
    }
    delay(100);
}