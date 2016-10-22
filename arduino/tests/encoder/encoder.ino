// Test an encoder using interrupts


#define LED         13
#define INT_PIN2    2
#define INT_PIN3    3


volatile byte state = HIGH;
volatile byte state2 = LOW;


void enc0() {
    state = !state;
}

void enc1() {
    state2 = !state2;
}

void setup() {
    pinMode(LED, OUTPUT);
    pinMode(INT_PIN2, INPUT_PULLUP);
    pinMode(INT_PIN3, INPUT_PULLUP);

    attachInterrupt(digitalPinToInterrupt(INT_PIN2), enc0, LOW);
    attachInterrupt(digitalPinToInterrupt(INT_PIN3), enc1, LOW);
}


void loop() {
    digitalWrite(LED, state);
}