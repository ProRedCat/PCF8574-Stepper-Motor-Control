/*
 * Created by ProRedCat (Reilly Oldham) 
 * V 1.2 created 14/04/20
 * NOTE: HAS NOT BEEN TESTED ON STEPPER MOTORS YET
 */

#ifndef PCF8574_Stepper_H
#define PCF8574_Stepper_H

#include <Arduino.h>
#include <PCF8574.h>

class StepperMotor{
  private:
    int stepsPerRev;
  
    int pin1;
    int pin2;
    int pin3;
    int pin4;

    bool twoWire;
    
    bool stepperStates[4]; //Make an array with 4 indexes to store the states of the output to the stepper motor

    const int maxRPM = 100;

    unsigned long stepDelay;
    unsigned long lastStepTime;

    bool direction;

    void stepForward();
    void stepBackward();

    PCF8574 &stepperExpander;

  public:
    //Constructor that takes in PCF8574 object, and the pins that the stepper motor is going to use
    StepperMotor(PCF8574 &stepperExpander, int stepsPerRev, int pin1, int pin2, int pin3 = NULL, int pin4 = NULL);
    
    void setSpeed(float speed);
    void step(int steps);
};

#endif
