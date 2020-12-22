HardwareSerial FPGC(1);
// Adjusted HardwareSerial.cpp to use pin 22 for RX, and pin 19 for TX

void setup() {
  // put your setup code here, to run once:
  FPGC.begin(115200);
  Serial.begin(115200);
}

int incomingByte = 0;

// Simple echo test to see if incoming string is correct
void loop() {
  if (FPGC.available() > 0) 
  {
    incomingByte = FPGC.read();
    for (int i = 0; i < 8; i++)
    {
      if (incomingByte < pow(2, i))
         FPGC.print(B0);
    }
    FPGC.print((char)incomingByte, BIN);
  }
}
