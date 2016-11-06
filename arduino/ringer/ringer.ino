// Wireless triggered Phone ringer
#include <SPI.h>
// https://github.com/TMRh20/RF24
#include <RF24.h>

#define RADIO_ENABLE_PIN    9
#define RADIO_SELECT_PIN    10
#define RINGER_CODE         1337
#define DISABLE_CODE        1010
#define RETRIES             10

#define AUDIO_SELECT_PIN    2
#define RINGER_PIN          3

RF24 radio(RADIO_ENABLE_PIN, RADIO_SELECT_PIN);
byte RADIO_ADDRESS[6] = "Phone\0";


void setup() {
    // Set Ringer to off by default
    pinMode(RINGER_PIN, OUTPUT);
    digitalWrite(RINGER_PIN, LOW);

    // Set audio select to default (low)
    pinMode(AUDIO_SELECT_PIN, OUTPUT);
    digitalWrite(AUDIO_SELECT_PIN, LOW);

    Serial.begin(9600);
    Serial.println("Init Complete. Waiting for signal...");

    radio.begin();
    radio.setPALevel(RF24_PA_MAX);

    radio.openReadingPipe(1, RADIO_ADDRESS);
    radio.enableDynamicAck();
    radio.startListening();
}


void ringRing() {
    // Enable audio2
    digitalWrite(AUDIO_SELECT_PIN, HIGH);

    // ring the phone
    digitalWrite(RINGER_PIN, HIGH);
    delay(400);
    digitalWrite(RINGER_PIN, LOW);
    delay(200);
    digitalWrite(RINGER_PIN, HIGH);
    delay(400);
    digitalWrite(RINGER_PIN, LOW);
}


void resetPhone() {
    // switch back to audio 1
    digitalWrite(AUDIO_SELECT_PIN, LOW);
}


void loop() {
    unsigned long code;
    if (radio.available()) {
        while (radio.available()) {
            radio.read(&code, sizeof(unsigned long));
        }
        Serial.print("Got a message: ");
        Serial.println(code);
        if (code == RINGER_CODE) {
            ringRing();
        } else if (code == DISABLE_CODE) {
            resetPhone();
        }
    }
}