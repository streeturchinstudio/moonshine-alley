// Test an encoder using interrupts


// http://www.pjrc.com/teensy/td_libs_Encoder.html
#include <Encoder.h>


#define LED         13


Encoder knob(3, 2);
volatile long position = -999;

void setup() {
    delay(50);
    position = knob.read();
    Serial.begin(9600);
    Serial.println("ready...");
}


void loop() {
    long new_pos = knob.read();
    if (new_pos != position) {
        long delta = new_pos - position;
        if (delta > 100) {
            position = new_pos;
            return;
        }
        Serial.print("position: ");
        Serial.print(new_pos);
        Serial.print(", delta: ");
        Serial.println(delta);
    }
    position = new_pos;
}







