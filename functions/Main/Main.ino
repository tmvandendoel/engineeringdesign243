#include <Servo.h>
#include <ArduinoJson.h>
//set wheel speeds as globals to ues them in other functions
int wheelLeft = 0;
int wheelRight = 0;
int wheelBack = 0;
Servo ServoWheelLeft;
Servo ServoWheelRight;
Servo ServoWheelBack;

//This file contains the neccesary functions for movement etc...
//FILE NOT READY TO COMPILE!


void movement(int direction, int speed) {
  
  
    /* int direction: integer between -100 and 100 indicating the
     * direction to be taken. 0 means straight forward.
 
     * int speed: integer between 0 and 100 indicating how fast the ROD
     * should be moving. 100 represents the fasted speed possible in the
     * GIVEN direction! Due to wheel position, the fastest speed might
     * differ in different directions.

     * NOTE: this function uses trigonometric calculations. These might
     * slow down the reaction speed of the Arduino or consume too much
     * memory. In both cases a lookuptable will be created.
     */
        
    int wheelLeft = 0;
    int wheelRight = 0;
    int wheelBack = 0;
    // at the end, these vars contain the power sent to each motor
    float pi = 3.14;
    float cons = pi / 6.0;
    float dir = direction / 100 * pi;
    // convert to radians
        
    wheelLeft = (int) 100*sin(dir - cons);
    wheelRight = (int) 100*sin(dir + cons);
    wheelBack = (int) 100*sin(dir);
    //calculate relative powers
    

    int largest = wheelLeft;
    if(wheelRight > largest)
    {
      largest = wheelRight;
    }
    if(wheelBack > largest)
    {
      largest = wheelBack;
    }
    //calculate largest value of the three wheels

    wheelRight = (int) wheelRight / largest * 100;
    wheelLeft = (int) wheelLeft / largest * 100;
    wheelBack = (int) wheelBack / largest * 100;
    //make the largest power become 100, scale the others appropriately
}

void setup() {
  ServoWheelLeft.attach(9);
  ServoWheelRight.attach(10);
  ServoWheelBack.attach(11);

  Serial1.begin(115200); //for Ethernet or Wifi -- and clear input buffer
  // clear the input buffer
  while (Serial1.available()) {
     Serial1.read();  
  }
}

void loop() {
  //Use of JSON, because two variables are sent via the Serial. This part reads the JSON data and parses it to two variables.
  const size_t bufferSize = JSON_OBJECT_SIZE(2) + 20;
  DynamicJsonBuffer jsonBuffer(bufferSize);
  const char* json = Serial1.read();
  JsonObject& root = jsonBuffer.parseObject(json);
  int direction = root["direction"];
  int speed = root["speed"];
  
  movement(direction, speed); //parsed data is passed to movement calulation function, this function will store data in following globals 

  int ServoWheelLeftSpeed =   90 + 0.9 * wheelLeft; //servo speeds 90 is still, 180 max, 0 max the otherway (maybe a map function is better)
  int ServoWheelRightSpeed =  90 + 0.9 * wheelRight;
  int ServoWheelBackSpeed =   90 + 0.9 * wheelBack;
  
  ServoWheelLeft.write(ServoWheelLeftSpeed);
  ServoWheelRight.write(ServoWheelRightSpeed);
  ServoWheelBack.write(ServoWheelBackSpeed);
  
}

