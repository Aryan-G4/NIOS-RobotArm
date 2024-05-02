# NIOS-RobotArm
A 3DoF Robot Manipulator programmed and controlled using a DE1-SoC's embedded NIOS II processor. Fully designed in-house.


Funded by the University of Toronto


Sponsored by StepperOnline


Aryan Ghosh | David Xu

[Project Demo:![image](https://github.com/Aryan-G4/NIOS-RobotArm/assets/119129454/49d6488a-0b0f-4029-a8d5-771ca01d3090)](https://youtu.be/5mW0npLePgE)

## Introduction 
The first of its kind, the NIOS-Robot arm is a robot completely designed in-house and controlled by an FPGA.
It boasts a fully 3D-printed body, NEMA17 stepper motors, a breadboarded electrical system, and a low-level embedded software system.
This project was created to gain experience designing dynamic mechanical systems, conduct dyanamic and force analysis and configure and drive motors. 

![image](https://github.com/Aryan-G4/NIOS-RobotArm/assets/119129454/1d9d747d-6692-479e-93fc-b320513d60f2)

Today, robot manipulators are used worldwide to weld, assemble, transport, package, pick, and place. Seeing the importance of these concepts, we decided to create a simplified, scaled-down, cost-effective version of these industrial robots.

![image](https://github.com/Aryan-G4/NIOS-RobotArm/assets/119129454/758fc85a-4b77-4ba9-9466-50986205331f)
![image](https://github.com/Aryan-G4/NIOS-RobotArm/assets/119129454/172b1e4d-2cfb-433b-9574-2de3ec7b1dac)

*These graphics are for the previous Mk1*

## Key Features
* Free Movement in XYZ space
* Detachable and modular End-Effector
* Zero Backlash 5:1 Geared stepper motors
* Joint Rotation Limits in software
* Switch-controlled tactile User interface
* Solid Concrete Base
  
## Mechanical Design
![image](https://github.com/Aryan-G4/NIOS-RobotArm/assets/119129454/cb1c69f7-cc6c-4223-818c-1b2c3625922a) 
![image](https://github.com/Aryan-G4/NIOS-RobotArm/assets/119129454/d46c8dcf-b428-48b1-98eb-87508341bd13)


The mechanical design of this system has been through several iterations, all developed in Solidworks. This was done because of Solidowork's Assembly environment, where it is extremely easy to create new parts within an assembly related to others. 


The stepper motor that we chose for the project has an extremely high quality and precise .STEP file, and we used this to create an extremely low tolerance gap between the motors and the body.
All three motors were securely fastened using M3 screws measured at the correct length to engage as many threads of the screw as possible for the most secure connection.
![image](https://github.com/Aryan-G4/NIOS-RobotArm/assets/119129454/d5a2016d-7ab8-4708-9017-0cc0c67ca4a7)
![image](https://github.com/Aryan-G4/NIOS-RobotArm/assets/119129454/a72e495c-c841-4419-9049-a7e18d757c8d)

Transferring the output from the stepper motor shaft to a joint linkage was challenging. This was done by attaching an 8mm flange shaft coupler to the arm linkage. This was all held in place by four No.4 bolts and nuts. There was an issue with the nuts coming loose, and in the next revision, locknuts will be used.
![image](https://github.com/Aryan-G4/NIOS-RobotArm/assets/119129454/939e9292-a4dc-4bae-9e9c-7f8126d1206b)

Upon completing the structural and dynamic analysis of the robot and evaluating the torque produced by each motor, the length of both arm linkages was established. This was after several iterations.

Due to the need for transportation between the workshop and the lab, the robot could not be bolted down to a table to be held in place. To save costs and negate any moment produced by the linkages and end effectors, We poured concrete into the base of the robot, making it so heavy that any movement or forces applied to or by the robot would not cause it to fall over. 

![image](https://github.com/Aryan-G4/NIOS-RobotArm/assets/119129454/76c4a5af-19eb-4e4e-9d2f-38868df38b38)
![image](https://github.com/Aryan-G4/NIOS-RobotArm/assets/119129454/e02f9860-67d5-40bf-aeaa-acc3005afb9e)

## Embedded Software Design
The software takes the hardware provided to it (GPIO pins, Switches, and LEDs), and interprets the I/O to drive the motors. The software is designed to process only one instruction at a time. It works by polling all the switches waiting for any one to be switched. Once it detects that a switch has been flipped high, the program moves to a subroutine that detects which switch has been flipped, generates the appropriate GPIO pins to write to, and then generates a square wave to those pins. The program continues to generate this signal until the switch is flipped down. During this time, any other inputs and switch flips are completley ignored, allowing the user to focus on one joint at a time. 
![image](https://github.com/Aryan-G4/NIOS-RobotArm/assets/119129454/5932d479-3280-48f4-ac99-1b32bdf24901)

Joint limits were implemented by creating variables that counted how many steps each motor took and calculating how many degrees the motor moved by. Assuming there are no missed steps, the software can detect when a motor has rotated 180 degrees, and it can prevent the square wave to the step pin from being generated.

All code can be found in **main.c**
## Electrical Design
The electrical design handles two power rails:  a 24V power line and a 5V logic line. Each nema17 stepper motor required approximately 12-24V of power at 2.2A to produce its advertised torque. This was all controlled through the A4988 Stepper motors. The A4988 offers 2 important logic pins, step and direction. Every time the step pin goes high (with a logic of 5V), the stepper motor rotates by 0.36 degrees. To allow for continuous rotation, we needed to generate a **square wave** to send to the driver, causing the motor to step so frequently, creating the illusion of seamless rotation. The rotation direction would depend on the direction pin, which causes the motor to rotate clockwise when low (~0V) and counterclockwise when high (~5V). The A4988 drivers had to be tuned to the right resistance, as too much current can fry the IC and too little current will not give us enough motor torque. 

Shown below is the electrical system:
![image](https://github.com/Aryan-G4/NIOS-RobotArm/assets/119129454/7cc518b5-121c-435c-939e-dd50978b4ba2)

The GPIO pins from the FPGA are shown on the left. The wires are in groups of two (step and direction) and connect to the step/dir pins of the a4988 Stepper motor drivers shown on the breadboard.on the right there are 3 groups of 4 wires(red,blue, black, green) These groups are for each stepper motor as each motor requires 4 wires. Additionally on the right there is a grey and purple wire. These to wires are connected to the right side power raila nd provide 24V to the system.

All wires are colour coded as black and red are often known to be power/ground. 
## Iterations
Our Mk1 iteration was extremely durable, but both arms were far too heavy. Linkages were unnecessarily large, and the 3d printed infill density was far too high. This caused the elbow motor (located above the base) to miss steps because it 
![image](https://github.com/Aryan-G4/NIOS-RobotArm/assets/119129454/5b8bf70b-2a84-4dc7-8fc0-2c187afb9e42)
![image](https://github.com/Aryan-G4/NIOS-RobotArm/assets/119129454/7a787c1e-a779-474e-a0df-16c0ae538e2c)

*Old mk1*
Here is a comparison between the old and new first linkage:


![image](https://github.com/Aryan-G4/NIOS-RobotArm/assets/119129454/e35eeaf3-0f10-4251-b9f4-468050137dc8)

