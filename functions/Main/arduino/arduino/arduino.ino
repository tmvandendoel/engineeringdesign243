#include <Servo.h>
#include <ArduinoJson.h>
//set wheel speeds as globals to ues them in other functions
int wheelLeft = 0;
int wheelRight = 0;
int wheelBack = 0;
Servo ServoWheel0;
Servo ServoWheel1;
Servo ServoWheel2;

void setup() {
  ServoWheel0.attach(9);
  ServoWheel1.attach(10);
  ServoWheel2.attach(11);

  Serial1.begin(115200); //for Ethernet or Wifi -- and clear input buffer
  // clear the input buffer
  while (Serial1.available()) {
     Serial1.read();  
  }
}


void loop() {
  //Use of JSON, because two variables are sent via the Serial. This part reads the JSON data and parses it to variables.
  DynamicJsonBuffer jsonBuffer;
  const char* json = Serial1.read();
  JsonObject& root = jsonBuffer.parseObject(json);
  //pc returns value between -1 and 0 
  int wheel0 = root["Wheel0"];
  int wheel1 = root["Wheel1"];
  int wheel2 = root["Wheel2"];
  //The Python programm will return a value between -1 and 1, the arduino needs values between 0 and 180 and converts it here
  int wheelSpeed0 = (wheel0 + 1) + 90;
  int wheelSpeed1 = (wheel1 + 1) + 90;
  int wheelSpeed2 = (wheel2 + 1) + 90;

  
  ServoWheel0.write(wheel0);
  ServoWheel1.write(wheel1);
  ServoWheel2.write(wheel2);
}


