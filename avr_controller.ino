#define muteSignal 11    // mute input connected to digital pin 11
#define ledOut     13    // LED connected to digital pin 13
#define rmcOut      3    // IR LED HIGH=LED ON digital pin 3
#define bitTime   562    // length of the carrier bit in microseconds

// put your own codes here - 4 bytes (ADDR1 | ADDR2 | COMMAND1 | COMMAND2)
unsigned long irCmdOn      = 0b00000001000011100000001111111100; // ON
unsigned long irCmdDvd     = 0b00000001000011100000101111110100; // DVD
unsigned long irCmdVid2    = 0b00000001000011101101001100101100; // VID2
unsigned long irCmdAvr     = 0b00000001000011100000001111111100; // AVR
unsigned long irCmdVolUp   = 0b00000001000011101110001100011100; // Vol Up
unsigned long irCmdVolDown = 0b00000001000011100001001111101100; // Vol down
unsigned long irCmdOff     = 0b00000001000011101111100100000110; // OFF

// Establish initial variables and states
enum AvrStates {ON, OFF};      // Establish the valid states for the receiver
AvrStates avrState = OFF;      // Assume the receiver is off on startup
AvrStates desiredState = OFF;  // Assume the receiver should be  off on startup


void irCarrier(unsigned int irTimeMicroseconds) {
  digitalWrite(rmcOut, HIGH);
  delayMicroseconds(irTimeMicroseconds);
  digitalWrite(rmcOut, LOW);
}


void irSendCode(unsigned long code) {
  Serial.print("Sending IR Code:");
  Serial.println(code);
  // send the leading pulse
  irCarrier(9000);            // 9ms of carrier
  delayMicroseconds(4500);    // 4.5ms of silence

  // send the user defined 4 byte/32bit code
  for (int i=0; i<32; i++)            // send all 4 bytes or 32 bits
    {
    irCarrier(bitTime);               // turn on the carrier for one bit time
    if (code & 0x80000000)            // get the current bit by masking all but the MSB
      delayMicroseconds(3 * bitTime); // a HIGH is 3 bit time periods
    else
      delayMicroseconds(bitTime);     // a LOW is only 1 bit time period
     code<<=1;                        // shift to the next bit for this byte
    }
  irCarrier(bitTime);                 // send a single STOP bit.
  delayMicroseconds(4500);            // 4.5ms of silence
}


AvrStates recommendState()  {
  switch (digitalRead(muteSignal)) {
    case LOW: {
      // Powered ON and MUTED: turn receiver off
      digitalWrite(ledOut, LOW);
      return OFF;
      break;
    }
    case HIGH: {
      // Powered ON and NOT muted: turn receiver on
      digitalWrite(ledOut, HIGH);
      return ON;
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
  irSendCode(irCmdOn);
  delay(1000);  // Add delay to allow receiver to turn on fully
  irSendCode(irCmdDvd);
}


void avr_off() {
  Serial.println("AVR: Off");
  irSendCode(irCmdOff);
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
  pinMode(ledOut, OUTPUT);
  pinMode(rmcOut, OUTPUT);
  pinMode(muteSignal, INPUT);
  digitalWrite(ledOut, LOW);
  digitalWrite(rmcOut, LOW);
  Serial.println("Setup End");
}


void loop() {
  avrMachine();
  delay(100);
}
