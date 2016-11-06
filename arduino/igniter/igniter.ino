// Igniter for still

#include <SPI.h>
// https://github.com/TMRh20/RF24
#include <RF24.h>
// http://www.pjrc.com/teensy/td_libs_Encoder.html
#include <Encoder.h>


#define RADIO_ENABLE_PIN    9
#define RADIO_SELECT_PIN    10
#define RINGER_CODE         7331
#define DISABLE_CODE        2020

#define AUDIO_SELECT_PIN    4
#define RINGER_PIN          5
#define IGN_BUTTON          19 // A5
#define BRIGHT1             6
#define BRIGHT2             7

#define LED1                14 // A0
#define LED2                15 // A1
#define LED3                16 // A2
#define LED4                17 // A3

#define RESET_TIMEOUT       90 // seconds

// Setup radio
RF24 radio(RADIO_ENABLE_PIN, RADIO_SELECT_PIN);
byte RADIO_ADDRESS[6] = "Still\0";

// Setup the rotary encoder on pins 2 & 3
Encoder knob(3, 2);
volatile long position = 0;
volatile int enc_value = 0;
volatile int lights = 0;
volatile bool enabled = false;
volatile unsigned long last_time = 0;
volatile unsigned long elapsed = 0;


void updateLights() {
    Serial.print("Lights: ");
    Serial.println(lights);

    switch (lights) {
        case 0:
            digitalWrite(LED1, LOW);
            digitalWrite(LED2, LOW);
            digitalWrite(LED3, LOW);
            digitalWrite(LED4, LOW);

            digitalWrite(BRIGHT1, LOW);
            digitalWrite(BRIGHT2, LOW);
            break;

        case 1:
            digitalWrite(LED1, HIGH);
            digitalWrite(LED2, LOW);
            digitalWrite(LED3, LOW);
            digitalWrite(LED4, LOW);

            digitalWrite(BRIGHT1, LOW);
            digitalWrite(BRIGHT2, HIGH);
            break;

        case 2:
            digitalWrite(LED1, HIGH);
            digitalWrite(LED2, HIGH);
            digitalWrite(LED3, LOW);
            digitalWrite(LED4, LOW);

            digitalWrite(BRIGHT1, HIGH);
            digitalWrite(BRIGHT2, LOW);
            break;

        case 3:
            digitalWrite(LED1, HIGH);
            digitalWrite(LED2, HIGH);
            digitalWrite(LED3, HIGH);
            digitalWrite(LED4, LOW);

            digitalWrite(BRIGHT1, HIGH);
            digitalWrite(BRIGHT2, HIGH);
            break;

        case 4:
            digitalWrite(LED1, HIGH);
            digitalWrite(LED2, HIGH);
            digitalWrite(LED3, HIGH);
            digitalWrite(LED4, HIGH);

            digitalWrite(BRIGHT1, HIGH);
            digitalWrite(BRIGHT2, HIGH);
            break;

        default:
            digitalWrite(LED1, HIGH);
            digitalWrite(LED2, HIGH);
            digitalWrite(LED3, HIGH);
            digitalWrite(LED4, HIGH);

            digitalWrite(BRIGHT1, LOW);
            digitalWrite(BRIGHT2, LOW);
            break;
    }
}


bool processEncoder() {
    bool ret = false;
    long new_pos = knob.read();
    if (new_pos != position) {
        long delta = new_pos - position;
        if (delta > 100 || delta == 2) {
            position = new_pos;
            return ret;
        }

        if (delta > 0) {
            enc_value += 1;
            ret = true;
        } else {
            enc_value -= 2;
            ret = true;
        }
        enc_value = constrain(enc_value, 0, 25);
        lights = map(enc_value, 0, 25, 0, 4);
        updateLights();

        // Serial.print("position: ");
        // Serial.print(position);
        // Serial.print(", new_pos: ");
        // Serial.print(new_pos);
        // Serial.print(", enc_Value: ");
        // Serial.print(enc_value);
        // Serial.print(", delta: ");
        // Serial.println(delta);
    }
    position = new_pos;
    return ret;
}


void toggleLights() {
    digitalWrite(LED1, !digitalRead(LED1));
    digitalWrite(LED2, !digitalRead(LED2));
    digitalWrite(LED3, !digitalRead(LED3));
    digitalWrite(LED4, !digitalRead(LED4));
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
    enabled = false;
    lights = 0;
    enc_value = 0;
    updateLights();
}


void setup() {
    pinMode(IGN_BUTTON, INPUT_PULLUP);

    // Set Ringer to off by default
    pinMode(RINGER_PIN, OUTPUT);
    digitalWrite(RINGER_PIN, LOW);

    // Set audio select to default (low)
    pinMode(AUDIO_SELECT_PIN, OUTPUT);
    digitalWrite(AUDIO_SELECT_PIN, LOW);

    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
    pinMode(LED3, OUTPUT);
    pinMode(LED4, OUTPUT);
    updateLights();

    last_time = millis();
    enabled = false;


    delay(20);
    position = knob.read();
    Serial.begin(9600);
    Serial.println("ready...");

    radio.begin();
    radio.setPALevel(RF24_PA_MAX);

    radio.openReadingPipe(1, RADIO_ADDRESS);
    radio.enableDynamicAck();
    radio.startListening();
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

    if (digitalRead(IGN_BUTTON) == HIGH) {
        enabled = true;
        lights = 1;
        enc_value = 6;
        updateLights();
        Serial.println("button");
    }

    if (enabled) {
        unsigned long new_time = millis();
        elapsed = new_time - last_time;
        Serial.print("elapsed: ");
        Serial.println(elapsed);

        // in-active timeout
        if (elapsed > RESET_TIMEOUT*1000) {
            elapsed = 0;
            enabled = false;
            lights = 0;
            updateLights();
            Serial.println("timeout");
        }

        if (processEncoder()) {
            last_time = new_time;
        }

     } else {
        // element is in-active
        unsigned long new_time = millis();
        if (new_time - last_time >= 500) {
            toggleLights();
            last_time = new_time;
            Serial.println("toggle");
        }
    }
}