// Remote control for Still

#include <SPI.h>
// https://github.com/TMRh20/RF24
#include <RF24.h>

#define RADIO_ENABLE_PIN    9
#define RADIO_SELECT_PIN    10
#define RING_CODE           7331
#define DISABLE_CODE        2020
#define RETRIES             10

#define RING_BUTTON_PIN     2
#define DISABLE_BUTTON_PIN  3
#define SUCCESS_LED_PIN     4
#define FAILED_LED_PIN      5


RF24 radio(RADIO_ENABLE_PIN, RADIO_SELECT_PIN);
byte RADIO_ADDRESS[6] = "Still\0";


void setup() {
    // Enable pull-up resistor for input buttons
    pinMode(RING_BUTTON_PIN, INPUT);
    digitalWrite(RING_BUTTON_PIN, HIGH);

    pinMode(DISABLE_BUTTON_PIN, INPUT);
    digitalWrite(DISABLE_BUTTON_PIN, HIGH);

    // Enable LEDS
    pinMode(SUCCESS_LED_PIN, OUTPUT);
    digitalWrite(SUCCESS_LED_PIN, LOW);

    pinMode(FAILED_LED_PIN, OUTPUT);
    digitalWrite(FAILED_LED_PIN, LOW);

    Serial.begin(9600);

    radio.begin();
    radio.setPALevel(RF24_PA_MAX);

    radio.openWritingPipe(RADIO_ADDRESS);
    radio.enableDynamicAck();
    radio.stopListening();
    Serial.println("Init Complete");
}


void success() {
    digitalWrite(SUCCESS_LED_PIN, HIGH);
    digitalWrite(FAILED_LED_PIN, LOW);
    Serial.println("Sent");
    delay(1000);
    digitalWrite(SUCCESS_LED_PIN, LOW);

}


void failed() {
    digitalWrite(SUCCESS_LED_PIN, LOW);
    digitalWrite(FAILED_LED_PIN, HIGH);
    Serial.println("Gave up");
    delay(1000);
    digitalWrite(FAILED_LED_PIN, LOW);
}


void transmitCode(unsigned long code) {
    Serial.print("\nTransmitting...");
    for (int x=0; x< RETRIES; x++) {
        if (radio.write(&code, sizeof(unsigned long))) {
            success();
            return;
        } else {
            Serial.println("failed");
            delay(100);
        }
    }
    failed();
}


void loop() {
    if (digitalRead(RING_BUTTON_PIN) == LOW) {
        transmitCode(RING_CODE);
    } else if (digitalRead(DISABLE_BUTTON_PIN) == LOW) {
        transmitCode(DISABLE_CODE);
    }
    delay(100);
}
