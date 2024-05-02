# NIOS-RobotArm
A 3DoF Robot programmed and controlled using a DE1-SoC's embedded NIOS II processor

Aryan Ghosh | David Xu

[![Robot Demo](https://youtu.be/5mW0npLePgE.jpg)](https://youtu.be/5mW0npLePgE)



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
![image](https://github.com/Aryan-G4/NIOS-RobotArm/assets/119129454/cb1c69f7-cc6c-4223-818c-1b2c3625922a)

The mechanical design of this system has been through several iterations, all developed in Solidworks. This was done because of Solidowork's Assembly environment, where it is extremely easy to create new parts within an assembly related to others. 
The stepper motor that we chose for the project has an extremely high quality and precise .STEP file, and we used this to create an extremely low tolerance gap between the motors and the body.
All three motors were securely fastened using M3 screws measured at the correct length to engage as many threads of the screw as possible for the most secure connection.
![image](https://github.com/Aryan-G4/NIOS-RobotArm/assets/119129454/d5a2016d-7ab8-4708-9017-0cc0c67ca4a7)

Transferring the output from the stepper motor shaft to a joint linkage was challenging. This was done by attaching an 8mm flange shaft coupler to the arm linkage. This was all held in place by four No.4 bolts and nuts. There was an issue with the nuts coming loose, and in the next revision, locknuts will be used.

Upon completing the structural and dynamic analysis of the robot and evaluating the torque produced by each motor, the length of both arm linkages was established. This was after several iterations.

Due to the need for transportation between the workshop and the lab, the robot could not be bolted down to a table to be held in place. To save costs and negate any moment produced by the linkages and end effectors, I poured concrete into the base of the robot, making it so heavy that any movement or forces applied to or by the robot would not cause it to fall over. 

![image](https://github.com/Aryan-G4/NIOS-RobotArm/assets/119129454/76c4a5af-19eb-4e4e-9d2f-38868df38b38)

## Embedded Software Design
The software takes the hardware provided to it (GPIO pins, Switches, and LEDs), and interprets the I/O to drive the motors. The software is designed to process only one instruction at a time. It works by polling all the switches waiting for any one to be switched. Once it detects that a switch has been pressed. 

## Electrical Design

## Applications

## Iterations
Our Mk1 iteration was extremely durable, but both arms were far too heavy. Linkages were unnecesarily large, and the 3d printed infill density was far too high. This caused the elbow motor (located above the base) to miss steps because it 
![image](https://github.com/Aryan-G4/NIOS-RobotArm/assets/119129454/5b8bf70b-2a84-4dc7-8fc0-2c187afb9e42)
