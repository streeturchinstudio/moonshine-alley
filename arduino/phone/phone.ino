// Create phone

// used by TMRpcm.h
#define SDPIN 10

#define SPEAKER_PIN 9


#include <SPI.h>
#include <SD.h>
// https://github.com/TMRh20/TMRpcm
#include <TMRpcm.h>


TMRpcm audio;


void writeLog(String value) {
    File dataFile = SD.open("log.txt", FILE_WRITE);
    if (dataFile) {
        dataFile.println(value);
        dataFile.close();

        Serial.println("Wrote data");
    }
    else {
        Serial.println("error opening log.txt");
    }
}

void setup() {
    pinMode(SPEAKER_PIN, OUTPUT);
    digitalWrite(SPEAKER_PIN, LOW);

    Serial.begin(9600);
    audio.speakerPin = SPEAKER_PIN;
    audio.setVolume(6);
    while (true) {
        if (!SD.begin(SDPIN)) {
            Serial.println("initialization failed!");
            writeLog("init failed");
            delay(100);
        } else if (!SD.exists("audio.wav")) {
            writeLog("missing file");
            delay(1000);
        } else {
            break;
        }
    }

    Serial.println("playing audio");
    writeLog("playing");
    audio.play("audio.wav");
}


void loop() {
    if (!audio.isPlaying()) {
        writeLog("done");
        // wait 10sec then play again
        delay(10000);
        writeLog("repeat");
        audio.play("audio.wav");
    } else {
        delay(100);
    }
}