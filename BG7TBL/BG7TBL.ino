#define LED 12
#define MOSI 3
#define PLL_LOCK 19
#define CLK 4
#define CS 2

#include <SPI.h>
#include <inttypes.h>

void setup() {
  // put your setup code here, to run once:
  pinMode(LED, OUTPUT);
  pinMode(MOSI, OUTPUT);
  pinMode(CLK, OUTPUT);
  pinMode(CS, OUTPUT);
  pinMode(PLL_LOCK, INPUT);

  //Set everything high, especially the CS to let the ADF4350 reset its comms before starting talking.
  digitalWrite(CS, HIGH);
  digitalWrite(CLK, LOW);
  digitalWrite(MOSI, LOW);

  for (int i(0); i < 10; ++i) {
    delay(100);
    digitalWrite(LED, !digitalRead(LED));
  }

  //This is the setup sequence given by the original firmware suitable
  //for Anritsu spectrum analyzers. As this is about a HP system I
  //disabled it using the if (false) statement once I'd got it working.
  if (false) {
    //Reg 5
    //12345678901234567890123456789012
    //xxxxxxxxAAxxxxxxxxxxxxxxxxxxxccc
    //00000000010000000000000000000101
    //x = Unused
    //ccc = Register ID
    //AA = Lock detect pin operation 01 = Enabled
    shiftWord(0x00400005);
  
    //Reg 4
    //12345678901234567890123456789012
    //00000000100000101000010111111100
    //xxxxxxxxABBBCCCCCCCCDEFGHHIJJccc
    //A = Feedback select = fundamental
    //BBB = Divider select = 1/1
    //CCCCCCCC = Band select clock divider value = 40
    //D = VCO power down = VCO powered up
    //E = Mute till lock detect = Yes
    //F = Aux output select = Divided output
    //G = Aux output enable = Enabled
    //HH = Aux output power = +5dB
    //I = RF out = enabled
    //JJ = RF output power = +5dB
    shiftWord(0x008285FC);

    //Reg 3
    //12345678901234567890123456789012
    //00000000000000001000100101101011
    //xxxxxxxxxxxxxAxBBCCCCCCCCCCCCccc
    //A = Cycle slip reduction = Disabled
    //BB = Clock divider mode = Fast-lock enable
    //C^12 = 12-bit clock divider value = 301
    shiftWord(0x0000896B);

    //Reg 2
    //12345678901234567890123456789012
    //00000100000000010100111001000010
    //XAABBBCDEEEEEEEEEEFGGGGHIJKLMccc
    //AA = Low-noise mode = low noise mode
    //BBB = Muxout = DV_{DD}
    //C = Reference doubler = disabled
    //D = Ref. divide by 2 = disabled
    //E^10 = 10-bit R counter = 5
    //F = Double buffer = disabled
    //GGGG = Charge pump current setting = 2.5mA@5.1kOhm
    //H = LDF = Frac-N
    //I = LDP = 10ns
    //J = PD Polarity = positive
    //K = Power-down = disabled
    //L = CP 3-state = disabled
    //M = Counter reset = disabled
    shiftWord(0x04014E42);
  
    //Reg 1
    //12345678901234567890123456789012
    //00001000000000001011111010000001
    //xxxxABBBBBBBBBBBBCCCCCCCCCCCCccc
    //A = prescaler = 8/9
    //B = 12-bit phase value = 1
    //C = 12-bit modulus value = 2000
    shiftWord(0x0800BE81);
    //Reg 0
    //12345678901234567890123456789012
    //00000010011101100010101111000000
    //xAAAAAAAAAAAAAAAABBBBBBBBBBBBccc
    //A = 16-bit integer value = 1260
    //B = 12-bit fractional value = 1400
    shiftWord(0x02762BC0);
  }
  

  //This is the configuration required for the HP 8591A system. These
  //bytes were generated using the Analog Devices evaluation software (see
  //https://www.analog.com/en/design-center/evaluation-hardware-and-software/evaluation-boards-kits/EVAL-ADF4350.html)
  //using most of the settings listed above remained the same except
  //for the actual frequency.
  if (true) {
    shiftWord(0x00580005);
    shiftWord(0x009506FC);
    shiftWord(0x0000896B);
    shiftWord(0x04004E42);
    shiftWord(0x080080C9);
    shiftWord(0x00D40038);
  }
}

void shiftWord(int32_t w) {
  shiftCMD((w >> 24) & 0xFF, (w >> 16) & 0xFF, (w >> 8) & 0xFF, w & 0xFF);
}

void shiftCMD(byte A, byte B, byte C, byte D) {
  digitalWrite(CS, LOW);
  shiftOut(MOSI, CLK, MSBFIRST, A);
  shiftOut(MOSI, CLK, MSBFIRST, B);
  shiftOut(MOSI, CLK, MSBFIRST, C);
  shiftOut(MOSI, CLK, MSBFIRST, D);
  digitalWrite(CS, HIGH);
}

void loop() {
  delay(700);
  if (!digitalRead(PLL_LOCK))
    digitalWrite(LED, !digitalRead(LED));
  else
    digitalWrite(LED, HIGH);
}
