// this program demonstrates the communication from the control laptop
// to the arduino
// 
// you need an USB cable to program the arduino
// the LED is present on the Arduino board

//int ledPin = 13; //declare variable ledPin. 13 is the address of the port
//to which the LED is connected.

// setup() is only run once
void setup() {
  // enable pin 13 as an output
  pinMode(ledPin,OUTPUT);
  
  Serial1.begin(115200); // ethernet cable or WiFi
  
  // read the input buffer until it is empty
  // any character in the buffer is ignored
  while (Serial1.available()){
     Serial1.read();
  }
     
     
  // switch off the LED
  digitalWrite(ledPin,LOW);
  
}

// the commands in loop() are repeated forever
void loop() {
  // check if a character is present
  if(Serial1.available() > 0)
  {
    // if so read it
    char Command = Serial1.read();
    switch(Command)
    {
      // if it is a '1' switch the LED on
      case '1':
        digitalWrite(ledPin, HIGH);
      break;
      // if it is a '0' switch the LED off
      case '0':
        digitalWrite(ledPin, LOW);
      break;
      // other characters are ignored     
      
     }
  }
}
