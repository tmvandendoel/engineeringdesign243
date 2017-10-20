#include <Servo.h>

/* Servo mappings:
 *  9, 10, 11 -> front, left, and right wheels
 *  5, 6      -> camera X and Y
 *  7, 8      -> picking up
 *  12        -> lights
 */
Servo wheel0;
Servo wheel1;
Servo wheel2;
Servo cameraX;
Servo cameraY;
Servo pickup0;
Servo pickup1;
int lightsPin = 13;

// Values at which the continuous servos are (almost) stationary
float deadPoint0 = 88.0; // right
float deadPoint1 = 99.5; // left
float deadPoint2 = 83.8; // back

// Angles at which the pickup servos are up resp. down
float pickup0up   = 0;
float pickup0down = 135;
float pickup1up   = 0;
float pickup1down = 135;

void setup() {
  // Attach the servo objects to their respective ports
  wheel0.attach(9);
  wheel1.attach(10);
  wheel2.attach(11);
  cameraX.attach(5);
  cameraY.attach(6);
  pickup0.attach(7);
  pickup1.attach(8);
  pinMode(lightsPin, OUTPUT);

  Serial1.begin(115200); //for Ethernet or Wifi -- and clear input buffer
  // clear the input buffer
  while (Serial1.available()) {
     Serial1.read();  
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
  // Serial1.print('h');
  char c;
  do {
    c = Serial1.read();
  } while (c != ' ');
  // Make sure the entire data-packet is in the buffer
  while (Serial1.available() < 7) {
    delay(10);
  }
  // Read wheel data
  c = Serial1.read();
  float wheelSpeed0 = char_to_float(c);
  c = Serial1.read();
  float wheelSpeed1 = char_to_float(c);
  c = Serial1.read();
  float wheelSpeed2 = char_to_float(c);

  // Read camera angles
  c = Serial1.read();
  float camX = char_to_float(c);
  c = Serial1.read();
  float camY = char_to_float(c);

  // Read pickup mechanism state
  c = Serial1.read();
  // float pu = char_to_float(c);
  bool pu_up = (c != '0');
  c = Serial1.read();
  bool lights_on = (c != '0');

  //The Python program will return a value between -1 and 1, the arduino needs values between 0 and 180 and converts it here
  wheelSpeed0  = wheelSpeed0 * 90 + deadPoint0;
  wheelSpeed1  = wheelSpeed1 * 90 + deadPoint1;
  wheelSpeed2  = wheelSpeed2 * 90 + deadPoint2;
  camX         = camX * 90 + 90;
  camY         = camY * 90 + 90;
  // float pickupAngle0 = pickup0down + pu * (pickup0up - pickup0down);
  // float pickupAngle1 = pickup1down + pu * (pickup1up - pickup1down);
  float pickupAngle0 = pu_up ? pickup0up : pickup0down;
  float pickupAngle1 = pu_up ? pickup1up : pickup1down;

  wheel0.write(wheelSpeed0);
  wheel1.write(wheelSpeed1);
  wheel2.write(wheelSpeed2);
  cameraX.write(camX);
  cameraY.write(camY);
  pickup0.write(pickupAngle0);
  pickup1.write(pickupAngle1);
  digitalWrite(lightsPin, lights_on ? HIGH : LOW);

  
}


