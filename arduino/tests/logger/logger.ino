#include <arduino.h>
#include <swRTC.h>
#include <SPI.h>
#include <SD.h>

// Card select pin for SD card
#define SDPIN  10

swRTC clock;

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
    File dataFile = SD.open("log.txt", FILE_WRITE);
    if (dataFile) {
        dataFile.print(dateTimeStr(clock));
        dataFile.print(" ");
        dataFile.println(value);
        dataFile.close();

        Serial.println("Wrote data");
    }
    else {
        Serial.println("error opening log.txt");
    }
}


void setup() {
    clock.stopRTC();
    clock.setTime(12, 0, 0);
    clock.setDate(9, 16, 2016);
    clock.startRTC();

    Serial.begin(9600);
    if (!SD.begin(SDPIN)) {
        Serial.println("initialization failed!");
        return;
    }
    Serial.println("SD ready");
}


void loop() {
    writeLog(clock, "test");
    delay(1000);
}