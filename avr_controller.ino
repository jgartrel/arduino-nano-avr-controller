#define pwrSignal   3    // pushbutton connected to digital pin 3 (Nano pin 6)
#define muteSignal  2    // pushbutton connected to digital pin 2 (Nano pin 5)
#define ledPin     13    // LED connected to digital pin 13 (Nano pin 16)
#define BITtime   562    //length of the carrier bit in microseconds

enum AvrStates {ON, OFF};
AvrStates avrState = OFF;  //Assume the receiver is off on startup
AvrStates desiredState = OFF;
long lastMillis = 0;
long loops = 0;

/*
// #define IRLEDpin    2    // IR LED HIGH=LED ON
//put your own code here - 4 bytes (ADDR1 | ADDR2 | COMMAND1 | COMMAND2)
unsigned long IRcode=0b11000001110001111100000000111111;

void IRcarrier(unsigned int IRtimemicroseconds) {
}
*/

AvrStates recommendState()  {

  switch (digitalRead(pwrSignal)) {
    case HIGH: {
      switch (digitalRead(muteSignal)) {
        case HIGH: {
          // Powered ON and MUTED: turn receiver off
          return OFF;
          break;
        }
        case LOW: {
          // Powered ON and NOT muted: turn receiver on
          return ON;
          break;
        }
      }
      break;
    }
    case LOW:  {
      // Powered OFF: turn receiver off
      return OFF;
      break;
    }
  }
  return OFF;  // Should never get here

}


bool verifyRecommendation(AvrStates targetState,int msecs) {
  Serial.print("Verify State Change: ");
  Serial.println(targetState);

  int step = 100;    // Check every X ms until we reach total msecs
  for (int i=0; i <= msecs; i += step) {
    if (recommendState() != targetState) return false;
    delay(step);
  }
  return true;

}


void avr_on() {
  Serial.println("AVR: On");
  digitalWrite(ledPin, HIGH);
}


void avr_off() {
  Serial.println("AVR: Off");
  digitalWrite(ledPin, LOW);
}


void avrMachine() {

  desiredState = recommendState();

  switch (avrState) {
    case OFF: {
      if (desiredState != avrState) {
        if (verifyRecommendation(desiredState, 2000)) {
          avr_on(); avrState = ON;
        }
      }
      break;
    }
    case ON: {
      if (desiredState != avrState) {
        if (verifyRecommendation(desiredState, 2000)) {
          avr_off(); avrState = OFF;
        }
      }
      break;
    }
  }

}


void setup() {
  Serial.begin(9600);
  Serial.println("Setup Begin");
  pinMode(ledPin, OUTPUT);
  pinMode(pwrSignal, INPUT);
  pinMode(muteSignal, INPUT);
  digitalWrite(ledPin, LOW);
  for (int i=0; i < 3; i++) {
    digitalWrite(ledPin, HIGH);
    delay(1000);
    digitalWrite(ledPin, LOW);
    delay(1000);
  }
  Serial.println("Setup End");
}


void loop() {
  long currentMillis = millis();
  loops++;

  avrMachine();
  delay(100);

  if(currentMillis - lastMillis > 1000){
    Serial.print("Loops last second:");
    Serial.println(loops);
    lastMillis = currentMillis;
    loops = 0;
  }
}
