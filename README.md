# PCF8574-Stepper-Motor-Control
A library that can be used with a PCF8574 chip to control a stepper motor with either a two wire input PCB for controlling stepper motors, or four wires 

Requires the PCF8574 library made by xreef https://github.com/xreef/PCF8574_library

This library cannot control the speed of the motor as it does not calculate the delay between each step so in the Arduino program 
you will need to find out the delay between each step yourself. For information how to find the step delay see here:
https://arduino.stackexchange.com/questions/46468/how-to-calculate-step-delay


Note: As of version 1.0 completed 8/04/20, this library has not yet been tested on stepper motors and only works in theory
