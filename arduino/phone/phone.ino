// Crate phone

// used by TMRpcm.h
#define SDPIN 10

#define SPEAKER_PIN         9
#define LATCH_PIN           8
#define AUDIO_SELECT_PIN    3
#define PHONE_NUMBER        "TestPhone"
#define LOG_FILE            "TestPhone.log"

#define AUDIO_FILE_1        "audio.wav"
#define AUDIO_FILE_2        "audio2.wav"


#include <SPI.h>
#include <SD.h>
// https://github.com/leomil72/swRTC
#include <swRTC.h>
// https://github.com/TMRh20/TMRpcm
#include <TMRpcm.h>


swRTC clock;
TMRpcm audio;
bool latch;
int sleeping;


String padded(int number) {
  String tmp = "";
  if (number > 9) {
    return tmp + number;
  }
  return tmp + "0" + number;
}

String dateTimeStr(swRTC clk) {
  String dt = "";

  // 2016-09-16T12:00:00
  dt += clk.getYear();
  dt += "-";
  dt += padded(clk.getMonth());
  dt += "-";
  dt += padded(clk.getDay());
  dt += "T";
  dt += padded(clk.getHours());
  dt += ":";
  dt += padded(clk.getMinutes());
  dt += ":";
  dt += padded(clk.getSeconds());
  return dt;
}


void writeLog(swRTC clock, String value) {
    File dataFile = SD.open(LOG_FILE, FILE_WRITE);
    if (dataFile) {
        // Write to the log file
        dataFile.print(dateTimeStr(clock));
        dataFile.print(" ");
        dataFile.println(value);
        dataFile.close();

        // Start the log line with success marker
        Serial.print("+ ");
    }
    else {
        // Start the log line with failed marker
        Serial.print("! ");
    }

    // Write the log to the serial port
    Serial.print("LOG ");
    Serial.print(dateTimeStr(clock));
    Serial.print(" ");
    Serial.println(value);
}


void setup() {
    // Setup arduino inputs/outputs
    pinMode(SPEAKER_PIN, OUTPUT);
    digitalWrite(SPEAKER_PIN, LOW);

    // Enable pull-up resistor for receiver latch
    pinMode(LATCH_PIN, INPUT);
    digitalWrite(LATCH_PIN, HIGH);

    // Enable audio select pin
    pinMode(AUDIO_SELECT_PIN, INPUT);

    // Setup clock
    clock.stopRTC();
    clock.setTime(12, 0, 0);
    clock.setDate(9, 16, 2016);
    clock.startRTC();

    // Setup serial
    Serial.begin(9600);

    // Setup audio playback
    audio.speakerPin = SPEAKER_PIN;
    audio.setVolume(6);

    String boot = "Booting Phone: ";
    boot += PHONE_NUMBER;
    writeLog(clock, boot);

    // Check SD card
    while (true) {
        if (!SD.begin(SDPIN)) {
            writeLog(clock, "init failed");
            delay(100);
        } else if (!SD.exists("audio.wav")) {
            writeLog(clock, "missing file");
            delay(1000);
        } else {
            break;
        }
    }

    latch = false;
    sleeping = 0;
    writeLog(clock, "init complete");
}


void playAudio() {
    String msg = "audio file: ";
    if (digitalRead(AUDIO_SELECT_PIN) == LOW) {
        writeLog(clock, msg + AUDIO_FILE_1);
        audio.play(AUDIO_FILE_1);
    } else {
        writeLog(clock, msg + AUDIO_FILE_2);
        audio.play(AUDIO_FILE_2);
    }
}

void loop() {
    // Production value should be LOW
    if (digitalRead(LATCH_PIN) == LOW) {
        if (latch == false) {
            // The receiver was just picked up. Play the audio
            latch = true;
            writeLog(clock, "playing audio");
            playAudio();
            return;
        }

        // receiver is still up
        if (!audio.isPlaying()) {
            // Audio has finished playing.
            if (sleeping == 0) {
                writeLog(clock, "playback complete");
            }

            // Should audio repeat in 2 file mode?
            if (sleeping >= 50) {
                // Time to repeat (50 * 200ms = 10sec)
                sleeping = 0;
                writeLog(clock, "repeating audio");
                playAudio();
            } else {
                sleeping++;
                delay(200);
            }

            return;
        } else {
            // The audio is still playing
            delay(100);
        }
    } else {
        // The phone is on the latch
        if (audio.isPlaying()) {
            audio.stopPlayback();
        }
        if (latch == true) {
            writeLog(clock, "hung up");
        }
        latch = false;
        sleeping = 0;

        delay(200);
    }
}