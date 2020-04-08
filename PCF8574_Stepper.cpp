/*
 * Created by ProRedCat (Reilly Oldham) 
 * V 1.0 created 8/04/20
 * NOTE: HAS NOT BEEN TESTED ON STEPPER MOTORS YET
 */

#include "PCF8574_Stepper.h"
#include "PCF8574.h"

StepperMotor::StepperMotor(PCF8574 &stepperExpander, int pin1, int pin2, int pin3 = NULL, int pin4 = NULL){
  //Set the pins to the supplied pins
  this->pin1 = pin1;
  this->pin2 = pin2;
  this->pin3 = pin3;
  this->pin4 = pin4;

  //Configure Stepper States to be either in 4 pin or 2 pin mode (2 pin requiring only index 0, 1 for storing the states of the stepper motor)
  if(!(pin3 == NULL || pin4 == NULL)){
    this->stepperStates[0] = true; 
    this->stepperStates[1] = false; 
    this->stepperStates[2] = false; 
    this->stepperStates[3] = true; 
  }
  else{
    this->stepperStates[0] = false; 
    this->stepperStates[1] = false; 
    this->stepperStates[2] = NULL; 
    this->stepperStates[3] = NULL; 
  }

  //Make stepperExpander the object of the PCF8574 chip 
  this->stepperExpander = stepperExpander;

  //Run init to initialize the pins of stepperExpander
  init();
}

void StepperMotor::init(){
  //Configure the pin modes of the stepperExpander to be either four wire control or two wire control 
  if(!(pin3 == NULL || pin4 == NULL)){
    stepperExpander.pinMode(pin1, OUTPUT);
    stepperExpander.pinMode(pin2, OUTPUT);
    stepperExpander.pinMode(pin3, OUTPUT);
    stepperExpander.pinMode(pin4, OUTPUT);
  }
  else{
    stepperExpander.pinMode(pin1, OUTPUT);
    stepperExpander.pinMode(pin2, OUTPUT);
  }
}

void StepperMotor::stepForward(){
  //Check if in two wire control mode, if so run two wire step code
  if(pin3 == NULL || pin4 == NULL){ 
    if(stepperStates[0] == stepperStates[1]){ //If the states of the wires are the same invert the second state and set the first wire to that state
      stepperStates[1] = !stepperStates[1];
      stepperExpander.digitalWrite(pin1, stepperStates[1]);
    }
    else{ //Else the states are not the same invert the first state and set the second wire to that state
      stepperStates[0] = !stepperStates[0];
      stepperExpander.digitalWrite(pin2, stepperStates[0]);
    }
  }
  //Else running in four wire mode, so run in four wire step code
  else{
    if(stepperStates[0] == stepperStates[2]){ //If the states of wire 1 and 3 are the same invert wire 1 and set wire 2 as the inverted value of wire 1 and update both outputs to the PCF8574 chip
      stepperStates[0] = !stepperStates[0];
      stepperStates[1] = !stepperStates[0];
      stepperExpander.digitalWrite(pin1, stepperStates[0]);
      stepperExpander.digitalWrite(pin2, stepperStates[1]);
    } 
    else{ //Else the states of wire 1 and 3 are not the same invert wire 3 and set wire 4 as the inverted value of wire 3 and update both outputs to the PCF8574 chip
      stepperStates[2] = !stepperStates[2];
      stepperStates[3] = !stepperStates[3];
      stepperExpander.digitalWrite(pin3, stepperStates[2]);
      stepperExpander.digitalWrite(pin4, stepperStates[3]);
    }
  }
}

void StepperMotor::stepBackward(){
  if(pin3 == NULL || pin4 == NULL){
    if(stepperStates[0] == stepperStates[1]){ //Same as stepForwards except the pin2 is now related to the second state and pin1 is related to first state
      stepperStates[1] = !stepperStates[1];
      stepperExpander.digitalWrite(pin2, stepperStates[1]);
    }
    else{
      stepperStates[0] = !stepperStates[0];
      stepperExpander.digitalWrite(pin1, stepperStates[0]);
    }
  }
  else{ //Same as stepForwards except flipped the contents of the if and else statements 
    if(stepperStates[0] == stepperStates[2]){
      stepperStates[2] = !stepperStates[2];
      stepperStates[3] = !stepperStates[3];
      stepperExpander.digitalWrite(pin4, stepperStates[2]);
      stepperExpander.digitalWrite(pin3, stepperStates[3]);
      
    } 
    else{
      stepperStates[0] = !stepperStates[0];
      stepperStates[1] = !stepperStates[0];
      stepperExpander.digitalWrite(pin2, stepperStates[0]);
      stepperExpander.digitalWrite(pin1, stepperStates[1]);
    }
  }
}


