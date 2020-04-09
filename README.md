# PCF8574-Stepper-Motor-Control
A library that can be used with a PCF8574 chip to control a stepper motor with either a two wire input PCB for controlling stepper motors, or four wires 

Requires the PCF8574 library made by xreef https://github.com/xreef/PCF8574_library

When the constructor is run, if there are not 4 pins supplied, the library will default to two wire control mode
This library can control the speed of the motor in a range between 1 and 100 RPM (softcap of the library as to not cause missed steps, this can be changed in the library, to change this go to the .h file and change maxRPM)

Note: As of version 1.1 completed 10/04/20, this library has not yet been tested on stepper motors and only works in theory
