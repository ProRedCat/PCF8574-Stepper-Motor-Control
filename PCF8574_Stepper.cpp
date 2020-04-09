/*
 * Created by ProRedCat (Reilly Oldham) 
 * V 1.1 created 10/04/20
 * NOTE: HAS NOT BEEN TESTED ON STEPPER MOTORS YET
 */

#include "PCF8574_Stepper.h"
#include "PCF8574.h"

/*
 * Constructor, takes in PCF8574 object, and pins 1-4, if pin 3 or 4 are not supplied when constructor is run, stepper will default to two wire control
 */
StepperMotor::StepperMotor(PCF8574 &stepperExpander, int pin1, int pin2, int pin3 = NULL, int pin4 = NULL){
  //Set the pins to the supplied pins
  this->pin1 = pin1;
  this->pin2 = pin2;
  this->pin3 = pin3;
  this->pin4 = pin4;

  this->twoWire = (pin3 == NULL || pin4 == NULL) ? true : false;

  //Configure Stepper States to be either in 4 pin or 2 pin mode (2 pin requiring only index 0, 1 for storing the states of the stepper motor)
  if(twoWire){
    this->stepperStates[0] = false; 
    this->stepperStates[1] = false; 
    this->stepperStates[2] = NULL; 
    this->stepperStates[3] = NULL; 
  }
  else{
    this->stepperStates[0] = true; 
    this->stepperStates[1] = false; 
    this->stepperStates[2] = false; 
    this->stepperStates[3] = true; 
  }

  //Make stepperExpander the object of the PCF8574 chip 
  this->stepperExpander = stepperExpander;

  //Configure the pin modes of the stepperExpander to be either four wire control or two wire control 
  if(twoWire){
    stepperExpander.pinMode(pin1, OUTPUT);
    stepperExpander.pinMode(pin2, OUTPUT); 
  }
  else{
    stepperExpander.pinMode(pin1, OUTPUT);
    stepperExpander.pinMode(pin2, OUTPUT);
    stepperExpander.pinMode(pin3, OUTPUT);
    stepperExpander.pinMode(pin4, OUTPUT);
  }

  SSpeed(50); //Set default speed to 50 RPM
  this->sDirection = 1; //Set default direction to forwards
  this->lastStepTime = 0; 
}

/*
 * SStep takes in number of steps (positive for forward, negative for backward) and will step until it's reached the step count
 */
void StepperMotor::SStep(int steps){
  while(steps > 0){ //While there are still steps to do keep looping
    this->sDirection = steps > 0 ? 1 : 0; //Find out if going backwards or forwards
    unsigned long currentTime = micros();
    if(currentTime - this->lastStepTime >= stepDelay){ //Check if time between now and the last step is greater than or equal to the step delay, if so change last step time and step in the direction
      this->lastStepTime = currentTime;
      if(this->sDirection == 1){
        stepForward();
      }
      else{
        stepBackward();
      }
      steps--;
    }
  }
}

/*
 * SSpeed sets the speed of the motor, take an input of 0 - 100 (RPM), has a softcap of 100RPM as to not cause issue with missing steps
 */
void StepperMotor::SSpeed(int sSpeed){
  sSpeed = sSpeed > maxRPM || sSpeed <= 0 ? 100 : sSpeed; //Cap max speed at 100 RPM, set default if it exceeds 100RPM or below 1RPM to 100RPM (CHANGE THIS IF YOU WANT BUT I FOUND ANY MORE THAN 100RPM CAUSES PROBLEMS)
  this->stepDelay = round((((1/((sSpeed)/60))/360) * 1.8) * 1000000); //Find the step delay at a set RPM, convert to microseconds and round
}

/*
 * Steps the motor forward one step
 */
void StepperMotor::stepForward(){
  //Check if in two wire control mode, if so run two wire step code
  if(twoWire){ 
    if(stepperStates[0] == stepperStates[1]){ //If the states of the wires are the same invert the second state and set the first wire to that state
      stepperStates[1] = !stepperStates[1];
      stepperExpander.digitalWrite(pin2, stepperStates[1]);
    }
    else{ //Else the states are not the same invert the first state and set the second wire to that state
      stepperStates[0] = !stepperStates[0];
      stepperExpander.digitalWrite(pin1, stepperStates[0]);
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

/*
 * Steps the motor backwards one step
 */
void StepperMotor::stepBackward(){
  if(twoWire){
    if(stepperStates[0] == stepperStates[1]){ //Same as stepForwards except flipped the contents of the if else statements
      stepperStates[0] = !stepperStates[0];
      stepperExpander.digitalWrite(pin1, stepperStates[0]);
    }
    else{
      stepperStates[1] = !stepperStates[1];
      stepperExpander.digitalWrite(pin2, stepperStates[1]);
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


