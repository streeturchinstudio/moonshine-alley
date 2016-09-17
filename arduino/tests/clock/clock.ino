/* This file is part of swRTC library.
   Please check the README file and the notes
   inside the swRTC.h file to get more info

   This example will send different infos every second
   to the computer through the serial port.

   Written by Leonardo Miliani <leonardo AT leonardomiliani DOT com>
   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
    version 3.0 of the License, or (at your option) any later version.
*/

#include <swRTC.h>
swRTC rtc; //create a new istance of the lib

String padded(int number) {
  String tmp = "";
  if (number > 9) {
    return tmp + number;
  }
  return tmp + "0" + number;
}

String dateTimeStr(swRTC clock) {
  String dt = "";

  // 2016-09-16T12:00:00
  dt += clock.getYear();
  dt += "-";
  dt += padded(clock.getMonth());
  dt += "-";
  dt += padded(clock.getDay());
  dt += "T";
  dt += padded(clock.getHours());
  dt += ":";
  dt += padded(clock.getMinutes());
  dt += ":";
  dt += padded(clock.getSeconds());
  return dt;
}

void setup() {
    rtc.stopRTC(); //stop the RTC
    rtc.setTime(12,0,0); //set the time here
    rtc.setDate(4,6,2012); //set the date here
    rtc.startRTC(); //start the RTC
    Serial.begin(9600); //choose the serial speed here
    delay(2000); //delay to let the user opens the serial monitor
}

void loop() {
    // Serial.print(rtc.getYear());
    // Serial.print("-");
    // Serial.print(rtc.getMonth());
    // Serial.print("-");
    // Serial.print(rtc.getDay());
    // Serial.print("T");
    // Serial.print(rtc.getHours());
    // Serial.print(":");
    // Serial.print(rtc.getMinutes());
    // Serial.print(":");
    // Serial.println(rtc.getSeconds());
    Serial.println(dateTimeStr(rtc));

    // Serial.print(rtc.getHours(), DEC);
    // Serial.print(":");
    // Serial.print(rtc.getMinutes(), DEC);
    // Serial.print(":");
    // Serial.print(rtc.getSeconds(), DEC);
    // Serial.print(" -- ");
    // Serial.print(rtc.getDay(), DEC);
    // Serial.print("/");
    // Serial.print(rtc.getMonth(), DEC);
    // Serial.print("/");
    // Serial.print(rtc.getYear(), DEC);
    // Serial.print(" (Leap year: ");
    // if (rtc.isLeapYear()) {
    //     Serial.print("yes");
    // } else {
    //     Serial.print("no");
    // }
    // Serial.println(")");
    // Serial.print("Day of week: ");
    // Serial.print(rtc.getWeekDay(), DEC);
    // Serial.print(" -- ");
    // Serial.print("Timestamp: ");
    // Serial.println(rtc.getTimestamp(), DEC);
    delay(1000);
}