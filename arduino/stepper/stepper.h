#ifndef _STEPPER_H_
#define _STEPPER_H_

class StepperMotor {
public:
    // Constructor that will set the inputs
    StepperMotor(int In1, int In2, int In3, int In4);

    // Function used to set the step duration in ms
    void setStepDuration(int duration);

    // Step a certain number of steps. + for one way and - for the other
    void step(int noOfSteps);

    // Step duration in ms
    int duration;
    // The input pin numbers
    int inputPins[4];
};

#endif
