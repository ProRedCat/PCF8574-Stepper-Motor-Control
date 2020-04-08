#include "PCF8574_Stepper.h"
#include "PCF8574.h"

PCF8574 stepperExpander(0x20); //Create the PCF8574 object at the hex address the PCF8574 chip is at 
StepperMotor mainStepper(stepperExpander, 0, 1, 2, 3); //When making the new object you can either use P0, P1, P2, ect.. or use numbers

void setup() {
  stepperExpander.begin();
}

void loop() {
  mainStepper.stepForward(); //Makes the stepper motor do one step forwards
  mainStepper.stepBackward(); //Makes the stepper motor do one step backwards

  //NOTE - This library does not account for how many steps per revolution of a stepper motor, or the delay that should be added between each step
}
