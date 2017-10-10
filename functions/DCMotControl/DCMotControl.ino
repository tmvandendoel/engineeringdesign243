#include <Servo.h>
// this program demonstrates the communication from the control laptop
// to the arduino and the commands to steer two DC motors
// 
// you need an USB cable to program the arduino

Servo Servo1;
Servo Servo2;
Servo Servo3;

// pin number of on-board LED
int ledPin = 13;

// Pulse Width Modulation (PWM) pins
int PWM1 = 3;
int PWM2 = 5;
int PWM3 = 6;
int PWM4 = 11;



void setup() {
  Servo1.attach(9);
  Servo2.attach(10);
  Servo3.attach(11);
  
  // all outputs to zero
  analogWrite(PWM1,0);
  analogWrite(PWM2,0);
  analogWrite(PWM3,0);
  analogWrite(PWM4,0);
 
  Serial1.begin(115200); //for Ethernet or Wifi
  
    
  // clear the input buffer
  while (Serial1.available())
     Serial1.read();  
  }

void loop() {
  if(Serial1.available() > 0)

  {
    char Command = Serial1.read();
    switch(Command)
    {
      
      // if 5 kill all outputs
      case '5':
        

      break;
      // in other cases switch to all combinations of forward and reverse
      // 255 means full speed
      case '8':
        Servo1.write(180);
        Servo2.write(180);
        Servo3.write(180);
      break;
    
      case '2':
        Servo1.write(90);
        Servo2.write(90);
        Servo3.write(90);
      
      break;
      
      case '4':
        

      break;
      
      case '6':
        

      break;
      
    
      
     }
  }
}
