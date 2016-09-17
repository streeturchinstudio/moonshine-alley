#include <SPI.h>
#include <RF24.h>


RF24 radio(9,10);
byte address[6] = "1Node";


void setup() {
    Serial.begin(11500);
    Serial.println("Press N, F to transmit");

    radio.begin();
    radio.setPALevel(RF24_PA_LOW);
    radio.enableDynamicAck();

    radio.stopListening();
    radio.openWritingPipe(address);
}

void loop() {
    if (Serial.available()) {
        char c = toupper(Serial.read());
        if (c == 'N' || c == 'F') {
            Serial.println("Sending...");
            if (!radio.write(&c, sizeof(char))) {
                Serial.println("Failed");
            }
        }
    }
    delay(100);
}