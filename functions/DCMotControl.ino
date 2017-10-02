// this program demonstrates the communication from the control laptop
// to the arduino and the commands to steer two DC motors
// 
// you need an USB cable to program the arduino

// pin number of on-board LED
int ledPin = 13;

// Pulse Width Modulation (PWM) pins
int PWM1 = 3;
int PWM2 = 5;
int PWM3 = 6;
int PWM4 = 11;


void setup() {
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
        analogWrite(PWM1,0);
        analogWrite(PWM2,0);
        analogWrite(PWM3,0);
        analogWrite(PWM4,0);
      break;
      // in other cases switch to all combinations of forward and reverse
      // 255 means full speed
      case '8':
        analogWrite(PWM1,255);
        analogWrite(PWM2,0);
        analogWrite(PWM3,255);
        analogWrite(PWM4,0);
      break;
    
      case '2':
        analogWrite(PWM1,0);
        analogWrite(PWM2,255);
        analogWrite(PWM3,0);
        analogWrite(PWM4,255);
      break;
      
      case '4':
        analogWrite(PWM1,255);
        analogWrite(PWM2,0);
        analogWrite(PWM3,0);
        analogWrite(PWM4,255);
      break;
      
      case '6':
        analogWrite(PWM1,0);
        analogWrite(PWM2,255);
        analogWrite(PWM3,255);
        analogWrite(PWM4,0);
      break;
      
    
      
     }
  }
}
