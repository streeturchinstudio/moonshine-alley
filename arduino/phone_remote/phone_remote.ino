// Remote control for phone

#include <SPI.h>
// https://github.com/TMRh20/RF24
#include <RF24.h>

#define RADIO_ENABLE_PIN    9
#define RADIO_SELECT_PIN    10
#define RING_CODE           1337
#define DISABLE_CODE        1010
#define RETRIES             10

#define RING_BUTTON_PIN     2
#define DISABLE_BUTTON_PIN  3

RF24 radio(RADIO_ENABLE_PIN, RADIO_SELECT_PIN);
byte RADIO_ADDRESS[6] = "Phone";


void setup() {
    // Enable pull-up resistor for input buttons
    pinMode(RING_BUTTON_PIN, INPUT);
    digitalWrite(RING_BUTTON_PIN, HIGH);

    pinMode(DISABLE_BUTTON_PIN, INPUT);
    digitalWrite(DISABLE_BUTTON_PIN, HIGH);


    Serial.begin(9600);

    radio.begin();
    radio.setPALevel(RF24_PA_LOW);

    radio.openWritingPipe(RADIO_ADDRESS);
    radio.stopListening();
    Serial.println("Init Complete");
}


void transmitCode(unsigned long code) {
    Serial.print("\nTransmitting...");
    for (int x=0; x< RETRIES; x++) {
        if (radio.write(&code, sizeof(unsigned long))) {
            Serial.println("Sent");
            return;
        } else {
            Serial.println("failed");
            delay(100);
        }
    }
    Serial.println("Gave up");
}


void loop() {
    if (digitalRead(RING_BUTTON_PIN) == LOW) {
        transmitCode(RING_CODE);
    } else if (digitalRead(DISABLE_BUTTON_PIN) == LOW) {
        transmitCode(DISABLE_CODE);
    }
    delay(100);
}
