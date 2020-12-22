#define INPUTSIZE 188

volatile boolean a = false;
volatile boolean b = false;
volatile boolean l = false;
volatile boolean r = false;
volatile boolean u = false;
volatile boolean d = false;
volatile boolean start = false;
volatile boolean sel = false;

volatile byte startcounter = 4;

volatile uint8_t buttonState = 0xff;

volatile byte rbits = 0;

volatile byte startDelay = 0;

volatile boolean rdata[INPUTSIZE] = { 0,0,0,0,0,0,0,0,0,0,
                                      0,0,0,0,0,0,0,0,0,0,
                                      0,0,0,0,0,0,0,0,0,0,
                                      0,0,0,0,0,0,0,0,0,0,
                                      0,0,0,0,0,0,0,0,0,0,
                                      0,0,0,0,0,0,0,0,0,0,
                                      0,0,0,0,0,0,0,0,0,0,
                                      0,0,0,0,0,0,0,0,0,0,
                                      0,0,0,0,0,0,0,0,0,0,
                                      0,0,0,0,0,0,0,0,0,0,
                                      0,0,0,0,0,0,0,0,0,0,
                                      0,0,0,0,0,0,0,0,0,0,
                                      0,0,0,0,0,0,0,0,0,0,
                                      0,0,0,0,0,0,0,0,0,0,
                                      0,0,0,0,0,0,0,0,0,0,
                                      0,0,0,0,0,0,0,0,0,0,
                                      0,0,0,0,0,0,0,0,0,0,
                                      0,0,0,0,0,0,0,0,0,0,
                                      0,0,0,0,0,0,0,0     };

//volatile boolean waitForFrame = true;        //when waiting for next frame

volatile int frameCounter = 0;

//volatile int skips = 0;



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

volatile int pollCount = 0;

int incomingByte = 0;

int boardString[24];
int byteIndex = 0;

void IRAM_ATTR latch_isr() {


    //rising
    latchHigh = 1;
    if (shiftButtons[0] == 1)
      GPIO.out_w1tc = ((uint32_t)1 << 27);
    else
      GPIO.out_w1ts = ((uint32_t)1 << 27);
  
    shiftIdx = 1;

    pollCount += 1;
  
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

void setup()
{

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

void byteBoardToBits()
{
  int bcount = INPUTSIZE-8;
  for (int i = 1; i < 24; i++)
  {
    for (int n = 0; n <8; n++)
    {
      int b = bitRead(boardString[i], n);
      bcount -= 1;
      if (bcount >= 0)
        rdata[bcount] = b;
    }
  }

  rdata[INPUTSIZE-8] = bitRead(boardString[0], 7);
  rdata[INPUTSIZE-7] = bitRead(boardString[0], 6);
  rdata[INPUTSIZE-6] = bitRead(boardString[0], 5);
  rdata[INPUTSIZE-5] = bitRead(boardString[0], 4);
  rdata[INPUTSIZE-4] = bitRead(boardString[0], 3);
  rdata[INPUTSIZE-3] = bitRead(boardString[0], 2);
  rdata[INPUTSIZE-2] = bitRead(boardString[0], 1);
  rdata[INPUTSIZE-1] = bitRead(boardString[0], 0);
}

void loop()
{
  
  if (FPGC.available() > 0) 
  {
    
    incomingByte = FPGC.read();
    boardString[byteIndex] = incomingByte;
    byteIndex += 1;
    if (byteIndex >= 24)
    {
      byteIndex = 0;
      byteBoardToBits();
    }
  }

  
  if (pollCount >= 2)
  {
    pollCount = 0;
    actOnFrame();
  }
}

void actOnFrame()
{
  frameCounter++;
  play();
}



void updateButtons()
{
  shiftButtons[0] = b;

  shiftButtons[2] = sel;
  shiftButtons[3] = start;
  
  shiftButtons[4] = u;
  shiftButtons[5] = d;
  shiftButtons[6] = l;
  shiftButtons[7] = r;
  shiftButtons[8] = a;
}
