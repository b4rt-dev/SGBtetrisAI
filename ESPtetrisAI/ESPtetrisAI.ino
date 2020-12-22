HardwareSerial FPGC(1);
// Adjusted HardwareSerial.cpp to use pin 22 for RX, and pin 19 for TX

/*
        Clock Cycle     Button Reported
        ===========     ===============
        1               B
        2               Y
        3               Select
        4               Start
        5               Up on joypad
        6               Down on joypad
        7               Left on joypad
        8               Right on joypad
        9               A
        10              X
        11              L
        12              R
        13              none (always high)
        14              none (always high)
        15              none (always high)
        16              none (always high)

        note: low == pressed
*/

volatile int shiftIdx = 0;
volatile int shiftButtons[17] = {0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0};
volatile int latchHigh = 0;

void IRAM_ATTR latch_isr() {


    //rising
    latchHigh = 1;
    if (shiftButtons[0] == 1)
      GPIO.out_w1tc = ((uint32_t)1 << 27);
    else
      GPIO.out_w1ts = ((uint32_t)1 << 27);
  
    shiftIdx = 1;

  
}

void IRAM_ATTR clk_isr() {



  if (shiftButtons[shiftIdx] == 1)
    GPIO.out_w1tc = ((uint32_t)1 << 27);
  else
    GPIO.out_w1ts = ((uint32_t)1 << 27);
    
    shiftIdx += 1;
    if (shiftIdx > 16)  
    {
      shiftIdx = 0;
      GPIO.out_w1tc = ((uint32_t)1 << 27); //low
    }

    
}

void setup() {
  // put your setup code here, to run once:
  FPGC.begin(115200);
  Serial.begin(115200);

  // 25: clk in
  // 26: latch in
  // 27: data out
  pinMode(27, OUTPUT);
  pinMode(25, INPUT);
  pinMode(26, INPUT);
  pinMode(39, INPUT);
  digitalWrite(27, LOW);
  attachInterrupt(25, clk_isr, RISING);
  attachInterrupt(26, latch_isr, RISING);
}

int incomingByte = 0;

int boardString[24];
int byteIndex = 0;

// Simple echo test to see if incoming string is correct
void loop() {
  
  delay(1000);
  /*
  if (FPGC.available() > 0) 
  {
    incomingByte = FPGC.read();
    boardString[byteIndex] = incomingByte;
    byteIndex += 1;
    if (byteIndex >= 24)
    {
      byteIndex = 0;
      for (int i = 0; i < 24; i++)
      {
        Serial.print(boardString[i], HEX);
      }
      Serial.println();
    }
    
  }*/
}
