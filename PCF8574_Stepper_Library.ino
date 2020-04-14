#include "PCF8574_Stepper.h"
#include "PCF8574.h"

PCF8574 stepperExpander(0x20); //Create the PCF8574 object at the hex address the PCF8574 chip is at 

StepperMotor mainStepper(stepperExpander, 200, 0, 1, 2, 3); //When making the new object you can either use P0, P1, P2, ect.. or use numbers
//StepperMotor mainStepper(stepperExpander, 200, 0, 1); //Config for two wire contol

void setup() {
  mainStepper.setSpeed(100); //Set speed to 100RPM (This is a softcap, library has limited to 100RPM to not cause problems with missing steps, change the library if you need to)
  stepperExpander.begin(); //Remember to .begin() the PCF8574 chip or else it will not output 
}

void loop() {
  mainStepper.step(100); //Step 100 forwards
  mainStepper.step(-100); //Step 100 backwards
}
