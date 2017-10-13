#include <Servo.h>

/* Servo mappings:
 *  9, 10, 11 -> front, left, and right wheels
 *  5, 6      -> camera X and Y
 *  7, 8      -> picking up
 */
Servo ServoWheel0;
Servo ServoWheel1;
Servo ServoWheel2;
Servo Camera0;
Servo Camera1;
Servo Pickup0;
Servo Pickup1;

// Values at which the continuous servos are (almost) stationary
float deadPoint0 = 88.0; // Heel
float deadPoint1 = 83.8; // Niks
float deadPoint2 = 98.0; // Half

// Angles at which the pickup servos are up resp. down
float pickup0up   = 0;
float pickup0down = 135;
float pickup1up   = 0;
float pickup1down = 135;

void setup() {
  // Attach the servo objects to their respective ports
  ServoWheel0.attach(9);
  ServoWheel1.attach(10);
  ServoWheel2.attach(11);
  Camera0.attach(5);
  Camera1.attach(6);
  Pickup0.attach(7);
  Pickup1.attach(8);

  Serial.begin(9600); //for Ethernet or Wifi -- and clear input buffer
  // clear the input buffer
  while (Serial.available()) {
     Serial.read();  
  }
}

// Method to 'decode' character
// 'A' to 'Z' for positive values
// 0 for zero
// 'a' to 'Z' for negative values
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
  // Make sure the entire data-packet is in the buffer
  while (Serial.available() < 6) delay(10);
  // Read wheel data
  c = Serial.read();
  float wheel0 = char_to_float(c);
  c = Serial.read();
  float wheel1 = char_to_float(c);
  c = Serial.read();
  float wheel2 = char_to_float(c);

  // Read camera angles
  c = Serial.read();
  float cam0 = char_to_float(c);
  c = Serial.read();
  float cam1 = char_to_float(c);

  // Read pickup mechanism state
  c = Serial.read();
  float pu = char_to_float(c);
  
  
  //The Python program will return a value between -1 and 1, the arduino needs values between 0 and 180 and converts it here
  float wheelSpeed0  = wheel0 * 90 + deadPoint0;
  float wheelSpeed1  = wheel1 * 90 + deadPoint1;
  float wheelSpeed2  = wheel2 * 90 + deadPoint2;
  float camAngle0    = cam0 * 90 + 90;
  float camAngle1    = cam1 * 90 + 90;
  float pickupAngle0 = pickup0down + pu * (pickup0up - pickup0down);
  float pickupAngle1 = pickup1down + pu * (pickup1up - pickup1down);
  

  ServoWheel0.write(wheelSpeed0);
  ServoWheel1.write(wheelSpeed1);
  ServoWheel2.write(wheelSpeed2);
  Camera0.write(camAngle0);
  Camera1.write(camAngle1);
  Pickup0.write(pickupAngle0);
  Pickup1.write(pickupAngle1);
}


