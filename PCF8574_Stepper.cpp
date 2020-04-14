/*
 * Created by ProRedCat (Reilly Oldham) 
 * V 1.2 created 14/04/20
 * NOTE: HAS NOT BEEN TESTED ON STEPPER MOTORS YET
 */

#include "PCF8574_Stepper.h"
#include "PCF8574.h"

/*
 * Constructor, takes in PCF8574 object, and pins 1-4, if pin 3 or 4 are not supplied when constructor is run, stepper will default to two wire control
 */
StepperMotor::StepperMotor(PCF8574 &stepperExpander,int stepsPerRev, int pin1, int pin2, int pin3 = NULL, int pin4 = NULL){
  this->stepsPerRev = stepsPerRev;
  
  //Set the pins to the supplied pins
  this->pin1 = pin1;
  this->pin2 = pin2;
  this->pin3 = pin3;
  this->pin4 = pin4;

  this->twoWire = (pin3 == NULL || pin4 == NULL);

  //Configure Stepper States to be either in 4 pin or 2 pin mode (2 pin requiring only index 0, 1 for storing the states of the stepper motor)
  if(this->twoWire){
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
  if(this->twoWire){
    this->stepperExpander.pinMode(this->pin1, OUTPUT);
    this->stepperExpander.pinMode(this->pin2, OUTPUT); 
  }
  else{
    this->stepperExpander.pinMode(this->pin1, OUTPUT);
    this->stepperExpander.pinMode(this->pin2, OUTPUT);
    this->stepperExpander.pinMode(this->pin3, OUTPUT);
    this->stepperExpander.pinMode(this->pin4, OUTPUT);
  }

  setSpeed(50); //Set default speed to 50 RPM
  this->direction = 1; //Set default direction to forwards
  this->lastStepTime = 0; 
}

/*
 * step takes in number of steps (positive for forward, negative for backward) and will step until it's reached the step count
 */
void StepperMotor::step(int steps){
  while(steps > 0){ //While there are still steps to do keep looping
    this->direction = steps > 0 ? 1 : 0; //Find out if going backwards or forwards
    unsigned long currentTime = micros();
    if(currentTime - this->lastStepTime >= this->stepDelay){ //Check if time between now and the last step is greater than or equal to the step delay, if so change last step time and step in the direction
      this->lastStepTime = currentTime;
      if(this->direction == 1){
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
 * setSpeed sets the speed of the motor, take an input of 0 - 100 (RPM), has a softcap of 100RPM as to not cause issue with missing steps
 */
void StepperMotor::setSpeed(float speed){
  speed = speed > maxRPM || speed <= 0 ? 100 : speed; //Cap max speed at 100 RPM, set default if it exceeds 100RPM or below 1RPM to 100RPM (CHANGE THIS IF YOU WANT BUT I FOUND ANY MORE THAN 100RPM CAUSES PROBLEMS)
  this->stepDelay = 60L * 1000L * 1000L / this->stepsPerRev / speed; //Find the step delay at a set RPM (Line taken from default Arduino stepper library) 
}

/*
 * Steps the motor forward one step
 */
void StepperMotor::stepForward(){
  //Check if in two wire control mode, if so run two wire step code
  if(this->twoWire){ 
    if(this->stepperStates[0] == this->stepperStates[1]){ //If the states of the wires are the same invert the second state and set the first wire to that state
      this->stepperStates[1] = !this->stepperStates[1];
      this->stepperExpander.digitalWrite(this->pin2, this->stepperStates[1]);
    }
    else{ //Else the states are not the same invert the first state and set the second wire to that state
      this->stepperStates[0] = !this->stepperStates[0];
      this->stepperExpander.digitalWrite(this->pin1, this->stepperStates[0]);
    }
  }
  //Else running in four wire mode, so run in four wire step code
  else{
    if(this->stepperStates[0] == this->stepperStates[2]){ //If the states of wire 1 and 3 are the same invert wire 1 and set wire 2 as the inverted value of wire 1 and update both outputs to the PCF8574 chip
      this->stepperStates[0] = !this->stepperStates[0];
      this->stepperStates[1] = !this->stepperStates[0];
      this->stepperExpander.digitalWrite(this->pin1, this->stepperStates[0]);
      this->stepperExpander.digitalWrite(this->pin2, this->stepperStates[1]);
    } 
    else{ //Else the states of wire 1 and 3 are not the same invert wire 3 and set wire 4 as the inverted value of wire 3 and update both outputs to the PCF8574 chip
      this->stepperStates[2] = !this->stepperStates[2];
      this->stepperStates[3] = !this->stepperStates[3];
      this->stepperExpander.digitalWrite(this->pin3, this->stepperStates[2]);
      this->stepperExpander.digitalWrite(this->pin4, this->stepperStates[3]);
    }
  }
}

/*
 * Steps the motor backwards one step
 */
void StepperMotor::stepBackward(){
  if(this->twoWire){
    if(this->stepperStates[0] == this->stepperStates[1]){ //Same as stepForwards except flipped the contents of the if else statements
      this->stepperStates[0] = !this->stepperStates[0];
      this->stepperExpander.digitalWrite(this->pin1, this->stepperStates[0]);
    }
    else{
      this->stepperStates[1] = !this->stepperStates[1];
      this->stepperExpander.digitalWrite(this->pin2, this->stepperStates[1]);
    }
  }
  else{ //Same as stepForwards except flipped the contents of the if and else statements 
    if(this->stepperStates[0] == this->stepperStates[2]){
      this->stepperStates[2] = !this->stepperStates[2];
      this->stepperStates[3] = !this->stepperStates[3];
      this->stepperExpander.digitalWrite(this->pin4, this->stepperStates[2]);
      this->stepperExpander.digitalWrite(this->pin3, this->stepperStates[3]);
      
    } 
    else{
      this->stepperStates[0] = !this->stepperStates[0];
      this->stepperStates[1] = !this->stepperStates[0];
      this->stepperExpander.digitalWrite(this->pin2, this->stepperStates[0]);
      this->stepperExpander.digitalWrite(this->pin1, this->stepperStates[1]);
    }
  }
}


