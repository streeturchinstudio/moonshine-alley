#include <SPI.h>
#include <SD.h> // preprocessor directives (header files)
#define SDPIN 10
#include <TMRpcm.h>

TMRpcm tmrpcm;

char mychar; // for reading input from Serial console

void setup(){
  Serial.begin(9600);
  tmrpcm.speakerPin = 9; //11 on Mega, 9 on Uno, Nano, etc
  tmrpcm.setVolume(6);
  if (!SD.begin(SDPIN)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("playing wav");
  tmrpcm.play("test4.wav");
}

void loop(){ }