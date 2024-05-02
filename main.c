  //CREATED BY DAVID XU AND ARYAN GHOSH FOR THE NIOS II ROBOT//
  
  #include <math.h>
  #include <stdbool.h>
  #include <stdio.h>
  //Defining I/O addresses for NIOSII GPIO
  #define ADDR_JP1PORT ((volatile char *) 0xFF200060)
  #define ADDR_JP2PORT ((volatile char *) 0xFF200070)
  //Onboard Hardware initialization
  #define LED ((volatile long *) 0xFF200000)
  #define PUSHBUTTONS ((volatile long *) 0xFF200050)
  #define SWITCHES ((volatile long *) 0xFF200040)
  // PS/2 port address
  #define PS2_ptr ((volatile int *) 0xFF200100)
  #define PUSHBUTTONS ((volatile long *) 0xFF200050)

  #define NUM_JOINTS 3
  //Motor set to microstepping
  //1.8 degrees /5 due to 5:1 planetary gear ratio and /4 due to quarter step a4988 driver setting
  double STEP_SIZE = 1.8/20;
  // forward kinematics
  typedef struct {
      double x;
      double y;
      double z;
  } Vector3D;

  typedef struct {
      double theta;
      double length;
      double upperlimit, lowerlimit;
  } Joint;

  typedef struct {
      Joint joints[NUM_JOINTS];
  } RoboticArm;

  void write_pixel(int x, int y, short colour) {
    volatile short *vga_addr=(volatile short*)(0x08000000 + (y<<10) + (x<<1));
    *vga_addr=colour;
  }

  /* use write_pixel to set entire screen to black (does not clear the character buffer) */
  void clear_screen() {
    int x, y;
    for (x = 0; x < 320; x++) {
      for (y = 0; y < 240; y++) {
      write_pixel(x,y,0);
    }
    }
  }

  void background() {
    int x, y;
    for (x = 0; x < 320; x++) {
      for (y = 0; y < 240; y++) {
        if(y < 179){
          write_pixel(x,y,image[y][x]);
        }else{
          write_pixel(x,y,0);
        }
    }
    }
  }

  /* write a single character to the character buffer at x,y
   * x in [0,79], y in [0,59]
   */
  void write_char(int x, int y, char c) {
    // VGA character buffer
    volatile char * character_buffer = (char *) (0x09000000 + (y<<7) + x);
    *character_buffer = c;
  }

bool isValidPosition(double theta1, double theta2, double theta3) {
    // Define the rotational limits for each motor
    const double motor1_min = -180.0;
    const double motor1_max = 180.0; // 360 degrees in radians

    const double motor2_min = 85;
    const double motor2_max = 95; // 200 degrees in radians

    const double motor3_min = -135.0;
    const double motor3_max = +135.0; // 270 degrees in radians

    // Check if the position is within the limits for each motor
    if (theta1 < motor1_min || theta1 > motor1_max)
        return false;
    if (theta2 < motor2_min || theta2 > motor2_max)
        return false;
    if (theta3 < motor3_min || theta3 > motor3_max)
        return false;

    // If all checks pass, the position is valid
    return true;
}

Vector3D cylindricalToCartesian(double theta, double radius, double height) {
    Vector3D cartesian;
    cartesian.x = radius * cos(theta);
    cartesian.y = radius * sin(theta);
    cartesian.z = height;
    return cartesian;
}


void motor(int sw, RoboticArm *arm){
  long count = 2500;
  int i = 0;
  int led = inttohex(sw);
  int index = (int)(sw/2);
  //long PBreleases;
  long Swval = *SWITCHES;

  while(Swval != 0){
    Swval = *SWITCHES;
    if(i == 0){
      if(sw % 2 == 1){
        led = led ^ inttohex(sw-1);  //xor for toggling led
        
        if((led & inttohex(sw-1)) != 0){
          arm->joints[index].theta += STEP_SIZE;
          if(arm->joints[index].theta >= arm->joints[index].upperlimit){ break;}
        }
      } else {
        led = led ^ inttohex(sw);  //xor for toggling led
        if(led != 0){
          arm->joints[index].theta -= STEP_SIZE;
          if(arm->joints[index].theta <= arm->joints[index].lowerlimit){ break;}
        }
      }
      i = count;
    }else{
       i--;
    }

    *ADDR_JP1PORT = led;
    *LED = led;

  }
  *LED = 0;
  *ADDR_JP1PORT = 0;
  return;
}


void moveMotor(Joint* vector, double targetAngle, int num) {
    // Determine the direction of rotation
    bool clockwise = (vector->theta > targetAngle);
    long count = 5000;
    int i = 0;
    int led = inttohex(num*2);
    // Move the motor until it reaches the target angle
    do {
    if(i == 0){
      if (clockwise){
        led = led ^ inttohex(num*2+1);  //xor for toggling led
        i = count;
        if((led & inttohex(num-1)) != 0){
          vector->theta += STEP_SIZE;
        }
      }else{
        led = led ^ inttohex(num*2);  //xor for toggling led
        i = count;
        if(led != 0){
          vector->theta -= STEP_SIZE;
        }
      }

    }else{
       i--;
    }

    *ADDR_JP1PORT = led;
    *LED = led;

  }while((int)vector->theta != (int)targetAngle && vector->theta > vector->lowerlimit && vector->theta < vector->upperlimit && *SWITCHES != 0);
  *LED = 0;
  *ADDR_JP1PORT = 0;
}


void moveArmToCoordinates(RoboticArm *arm, double theta, double radius, double height) {
  Vector3D targetPosition = cylindricalToCartesian(theta, radius, height);

  double L1 = arm->joints[0].length;
  double L2 = arm->joints[1].length;
  double L3 = arm->joints[2].length;

  double x = targetPosition.x;
  double y = targetPosition.y;
  double z = targetPosition.z;

  // Calculate theta1
  //arm->joints[0].theta = atan2(y, x);
  moveMotor(&arm->joints[0], atan2(y, x), 0);
  // Calculate theta2
  double D = (x * x + y * y + (z - L1) * (z - L1) - L2 * L2 - L3 * L3) / (2 * L2 * L3);
  //arm->joints[2].theta = atan2(-sqrt(1 - D * D), D);
  moveMotor(&arm->joints[2], atan2(-sqrt(1 - D * D), D), 2);

  double c1 = cos(arm->joints[0].theta);
  double s1 = sin(arm->joints[0].theta);
  double c3 = cos(arm->joints[2].theta);
  double s3 = sin(arm->joints[2].theta);

  double A = L2 + L3 * c3;
  double B = L3 * s3;
  double C = z - L1;
  double theta2_1 = atan2(C, sqrt(x * x + y * y)) - atan2(B, A);
  double theta2_2 = atan2(C, -sqrt(x * x + y * y)) - atan2(B, A);

  // Choose the solution closest to the previous angle to avoid joint flips
  if (fabs(theta2_1 - arm->joints[1].theta) < fabs(theta2_2 - arm->joints[1].theta))
    moveMotor(&arm->joints[1], theta2_1, 1);
  else
    moveMotor(&arm->joints[1], theta2_2, 1);
}

  int main () {
     //init interface directions
     *(ADDR_JP2PORT+4) = 0xffffffff; //set every JP2 bit direction to output
     *(ADDR_JP1PORT+4) = 0xffffffff; //set every JP1 bit dir to output

    clear_screen();
    background();


    int x = 0;
    int y = 50;
    char* hw;
    unsigned char byte1 = 0;
    unsigned char byte2 = 0;
    unsigned char byte3 = 0;


    long PBval;
    int checkswitch;
    int sw;
    int PS2_data, RVALID;
    bool turned = false;
    RoboticArm arm;

    // Initialize arm parameters
    arm.joints[0].length = 0;
    arm.joints[0].theta = 0;
    arm.joints[0].upperlimit = 180;
    arm.joints[0].lowerlimit = -180;

    arm.joints[1].length = 225;
    arm.joints[1].theta = 90;
    arm.joints[1].upperlimit = 95;
    arm.joints[1].lowerlimit = 85;

    arm.joints[2].length = 150;
    arm.joints[2].theta = 0;
    arm.joints[2].upperlimit = 135;
    arm.joints[2].lowerlimit = -135;

    while(1){
      int checkswitch = *(SWITCHES);
      if(checkswitch  == 0b1000000){
        moveArmToCoordinates(&arm, 0.0, 0.0, 300.0);
        moveArmToCoordinates(&arm, 0.0, 0.0, 375.0);
      }

      if (checkswitch != 0){
        if(!turned){
          sw = hextoint(checkswitch);
          motor(sw, &arm);
          turned = true;
        }else{
          sw = 0;
        }
      }else{
        turned = false;
      }

    }
     return 0;
  }

int motornumber(int sw){
  if(sw == 0 || sw ==1){
    return 1;
  }
  if (sw == 2  || sw ==3){
    return 2;
  }
  if (sw == 4 || sw ==5){
    return 3;
  }
}

int inttohex(int sw){
  if(sw == 0){
    return 0b1;
  }
  if(sw == 1){
    return 0b10;
  }
  if(sw == 2){
    return 0b100;
  }
  if(sw == 3){
    return 0b1000;
  }
  if(sw == 4){
    return 0b10000;
  }
  if(sw == 5){
    return 0b100000;
  }
}

int hextoint(int checkswitch){
  if(checkswitch == 0b1){
    return 0;
  }
  if(checkswitch == 0b10){
    return 1;
  }
  if(checkswitch == 0b100){
    return 2;
  }
  if (checkswitch == 0b1000){
    return 3;
  }
  if (checkswitch == 0b10000){
    return 4;
  }
  if (checkswitch == 0b100000){
    return 5;
  }
}
