



void setup() {
    Serial.begin(9600);
}


void loop() {
    int sensor_value = analogRead(A0);
    Serial.println(sensor_value);
    delay(1);
}