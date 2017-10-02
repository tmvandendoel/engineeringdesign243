// If button connected to pin 12 is pressed, send a response to controlling laptop
// the switche closes a contact between pin12 and gnd.

int buttonPin = 12;    // the number of the pushbutton pin

int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin

unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 100;    // the debounce time; increase if the output flickers

void setup() {
  
  pinMode(buttonPin, INPUT_PULLUP);
  
  Serial1.begin(115200);  // ethernet or WiFi
   
  // clear the input buffer
  while (Serial1.available()){
     Serial1.read();
  }
}

void loop() {

  int reading = digitalRead(buttonPin);
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      if ((buttonState == LOW) ){
        Serial1.print('C');
        delay(1);
        Serial1.print('O');
        delay(1);
        Serial1.print('N');
        delay(1);
        Serial1.print('T');
        delay(1);
        Serial1.print('A');
        delay(1);
        Serial1.print('C');
        delay(1);
        Serial1.print('T');
        delay(1);
        Serial1.println();
      }
    }
  }

  lastButtonState = reading;
  
}
