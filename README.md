# PCF8574-Stepper-Motor-Control
A library that can be used with a PCF8574 chip to control a stepper motor with either a two wire input PCB for controlling stepper motors, or four wires 

Requires the PCF8574 library made by xreef https://github.com/xreef/PCF8574_library

When the constructor is run, if there are not 4 pins supplied, the library will default to two wire control mode.This library can control the speed of the motor. Motors do not step simultaneously 

Notes: 
As of version 1.2 completed 14/04/20, this library has not yet been tested on stepper motors and only works in theory
To get the library to work place the PCF8574_Stepper folder in the Arduino libraries folder
