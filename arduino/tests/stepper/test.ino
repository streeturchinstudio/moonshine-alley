#include "stepper.h"

StepperMotor motor(2,3,4,5);

void setup(){
  motor.setStepDuration(1);
}

void loop(){
  motor.step(1000);
  delay(2000);
  motor.step(-1000);
  delay(2000);
}