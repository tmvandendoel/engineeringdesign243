#include <Servo.h>
#include <ArduinoJson.h>
//set wheel speeds as globals to ues them in other functions
int wheelLeft = 0;
int wheelRight = 0;
int wheelBack = 0;
Servo ServoWheel0;
Servo ServoWheel1;
Servo ServoWheel2;

float deadPoint0 = 88.0; // Heel
float deadPoint1 = 83.8; // Niks
float deadPoint2 = 98.0; // Half

void setup() {
  ServoWheel0.attach(9);
  ServoWheel1.attach(10);
  ServoWheel2.attach(11);

  Serial.begin(9600); //for Ethernet or Wifi -- and clear input buffer
  // clear the input buffer
  while (Serial.available()) {
     Serial.read();  
  }
}

float char_to_float(char c) {
  if ('A' <= c && c <= 'Z')
    return (c - 'A' + 1) / 26.0;
  if ('a' <= c && c <= 'z')
    return (c - 'a' + 1) / -26.0;
  return 0.0;
}

void loop() {
  char c;
  do {
    c = Serial.read();
  } while (c != ' ');
  while (Serial.available() < 3) delay(10);
  c = Serial.read();
  float wheel0 = char_to_float(c);
  c = Serial.read();
  float wheel1 = char_to_float(c);
  c = Serial.read();
  float wheel2 = char_to_float(c);
  //The Python program will return a value between -1 and 1, the arduino needs values between 0 and 180 and converts it here
  float wheelSpeed0 = wheel0 * 90 + deadPoint0;
  float wheelSpeed1 = wheel1 * 90 + deadPoint1;
  float wheelSpeed2 = wheel2 * 90 + deadPoint2;

  Serial.print(wheelSpeed0);
  Serial.print("|");
  Serial.print(wheelSpeed1);
  Serial.print("|");
  Serial.println(wheelSpeed2);
  
  ServoWheel0.write(wheelSpeed0);
  ServoWheel1.write(wheelSpeed1);
  ServoWheel2.write(wheelSpeed2);
}


