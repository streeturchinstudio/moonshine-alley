#include <SPI.h>
#include <RF24.h>

#define LED 2

RF24 radio(7,8);
byte address[6] = "1Node";


void setup() {
    pinMode(LED, 1);
    digitalWrite(LED, 1);
    delay(500);
    digitalWrite(LED, 0);
    delay(500);
    digitalWrite(LED, 1);
    delay(500);
    digitalWrite(LED, 0);

    Serial.begin(9600);
    Serial.println("Waiting...");

    radio.begin();
    radio.setPALevel(RF24_PA_LOW);

    radio.openReadingPipe(1, address);
    radio.enableDynamicAck();
    radio.startListening();
}

void loop() {
    char c;
    if (radio.available()) {
        while (radio.available()) {
            radio.read(&c, sizeof(char));
        }
        Serial.print("Got message: ");
        Serial.println(c);
        if (c == 'N') {
            digitalWrite(LED, 1);
        } else if (c == 'F') {
            digitalWrite(LED, 0);
        }
    }
}