# NIOS-RobotArm
A 3DoF Robot programmed and controlled using a DE1-SoC's embedded NIOS II processor
Aryan Ghosh | David Xu

## Introduction 
The first of its kind, the NIOS-Robot arm is a robot completely designed in-house.

It boasts a fully 3D-printed body, NEMA17 stepper motors, a breadboarded electrical system, and a low-level embedded software system.

## Key Features
* Free Movement in XYZ space
* Detachable and modular End-Effector
* Zero Backlash 5:1 Geared stepper motors
* Joint Rotation Limits in software
* Switch-controlled tactile User interface
* Solid Concrete Base
  
## Mechanical Design
The mechanical design of this system has been through several iterations, all developed in Solidworks. This was done because of Solidowork's Assembly environment, where it is extremely easy to create new parts within an assembly related to others. 
The stepper motor that we chose for the project has an extremely high quality and precise .STEP file, and we used this to create an extremely low tolerance gap between the motors and the body.
All three motors were securely fastened using M3 screws measured at the correct length to engage as many threads of the screw as possible for the most secure connection.
![image](https://github.com/Aryan-G4/NIOS-RobotArm/assets/119129454/d5a2016d-7ab8-4708-9017-0cc0c67ca4a7)

Transferring the output from the stepper motor shaft to a joint linkage was a challenge. This was done by attaching an 8mm flange shaft coupler to the arm linkage. This was all held in place by four No.4 bolts and nuts. There was an issue with the nuts coming loose, and in the next revision, locknuts will be used.

Upon completing the structural and dynamic analysis of the robot and evaluating the torque being produced by each motor, the length of both arm linkages were established. This was after several iterations.



## Embedded Software Design
The software takes the hardware provided to it (GPIO pins, Switches, and LEDs), and interprets the I/O to drive the motors. The software is designed to process only one instruction at a time. That means 

## Electrical Design

## Applications

## Iterations
