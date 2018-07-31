#include "NewhavenOLED.h"

/* Newhaven OLED display Library
   Supports OLED displays using US2066 controller chip.

   Adapted from software provided by Newhaven Display International
   and updates made by Pasquale D'Antini.
   https://newhavendisplay.com/app_notes.html
   https://www.newhavendisplay.com/NHD_forum/index.php?topic=914.0

   Further updated by Andy4495 to package into Arduino library format
   and allow simpler selection of Arduino control pin numbers.
*/
/*
 This library implements a software SPI ("bit-bang") communication with
 Newhaven OLED displays in the 0216CW and 0220CW. It will also likely
 work with other Newhaven displays which use the US2066 controller chip.

 The serial (SPI) interface on the display uses a minimum of pins
 conpared to parallel control methods.
 At a minimum, pins need to be dedicated for MOSI and SCK.
 If multiple displays are using the same MOSI and SCK pins, then a chip
 select pin also needs to be defined.
 A resent pin may optionally be defined.

 NOTES:
 - If there are hardware SPI devices being used, then the hardware
   SPI lines should NOT be used with this library.
 - The begin() and clear() functions use blocking delay() calls.
*/

/* Constructor:
      lines:   # of rows in display
      cols:    # of columns in display
      mosiPin: Pin # of the data out signal (OLED data in)
      sckPin:  Pin # of the serial clock signal
      csPin:   Pin # of the OLED chip select signal
               If no chip select pin is used (hardwired LOW), then use NO_PIN
      rstPin:  Pin # of the OLED RESET signal
               If no reset signal is used (hardwired HIGH), then use NO_PIN
*/
NewhavenOLED::NewhavenOLED(byte lines, byte cols, byte mosiPin, byte sckPin, byte csPin, byte rstPin)
{
  Lines = lines;
  Cols = cols;
  MOSI_pin = mosiPin;
  SCK_pin = sckPin;
  CS_pin = csPin;
  RST_pin = rstPin;

  if (Lines == 2 || Lines == 4)
  Row_bit = 0x08;                    // Display mode: 2/4 lines
  else
  Row_bit = 0x00;                    // Display mode: 1/3 lines
}

/* command(c)
   Send a command byte to the OLED
*/
void NewhavenOLED::command(byte c)
// Start Byte: 5 high bits, followed by:
// - R/W control bit: 0 = write, 1 = read
// - Data/Control bit: 0 = control, 1 = data
// - End bit: 0
// So in the case of a command, the sequence is 5 high bits followed by 3 low bits
{
  if (CS_pin != NO_PIN) {
    digitalWrite(CS_pin, LOW);
  }

  byte i = 0;                   // Bit index

  for (i = 0; i < 5; i++)
  {
    digitalWrite(MOSI_pin, HIGH);
    clock_cycle();
  }
  for (i = 0; i < 3; i++)
  {
    digitalWrite(MOSI_pin, LOW);
    clock_cycle();
  }

  send_byte(c);                      // Transmits the byte

  if (CS_pin != NO_PIN) {
    digitalWrite(CS_pin, HIGH);
  }

}

/* data(c)
   Send a data byte to the OLED
*/
void NewhavenOLED::data(byte d)
// Start Byte: 5 high bits, followed by:
// - R/W control bit: 0 = write, 1 = read
// - Data/Control bit: 0 = control, 1 = data
// - End bit: 0
// So in the case of a command, the sequence is 5 high bits followed by low, high, low bits
{
  if (CS_pin != NO_PIN) {
    digitalWrite(CS_pin, LOW);
  }

  byte i = 0;                        // Bit index

  for (i = 0; i < 5; i++)
  {
    digitalWrite(MOSI_pin, HIGH);
    clock_cycle();
  }
  digitalWrite(MOSI_pin, LOW);
  clock_cycle();
  digitalWrite(MOSI_pin, HIGH);
  clock_cycle();
  digitalWrite(MOSI_pin, LOW);
  clock_cycle();

  send_byte(d);                      // Transmits the byte

  if (CS_pin != NO_PIN) {
    digitalWrite(CS_pin, HIGH);
  }
}

/* clear()
   Clear the OLED display: move cursosr to top left, and clear display RAM
   Note that this function has a 2 ms blocking delay
*/
void NewhavenOLED::clear()
{
  command(0x01);        // Clear display
  delay(2);             // After a clear display, a minimum pause of 1-2 ms is required
}

/* begin()
   Intialize the control signals and display hardware.
   Note that this function has 252 ms of blocking delays
*/
void NewhavenOLED::begin()
{
  if (RST_pin != NO_PIN) {
    pinMode(RST_pin, OUTPUT);
    digitalWrite(RST_pin, HIGH);
  }
  if (CS_pin != NO_PIN) {
    pinMode(CS_pin, OUTPUT);
    digitalWrite(CS_pin, HIGH);
  }
  pinMode(MOSI_pin, OUTPUT);
  digitalWrite(MOSI_pin, LOW);
  pinMode(SCK_pin, OUTPUT);
  digitalWrite(SCK_pin, HIGH);

  command(0x22 | Row_bit); // Function set: extended command set (RE=1), lines #
  command(0x71);        // Function selection A:
  data(0x5C);           //  enable internal Vdd regulator at 5V I/O mode (def. value) (0x00 for disable, 2.8V I/O)
  command(0x20 | Row_bit); // Function set: fundamental command set (RE=0) (exit from extended command set), lines #
  command(0x08);        // Display ON/OFF control: display off, cursor off, blink off (default values)
  command(0x22 | Row_bit); // Function set: extended command set (RE=1), lines #
  command(0x79);        // OLED characterization: OLED command set enabled (SD=1)
  command(0xD5);        // Set display clock divide ratio/oscillator frequency:
  command(0x70);        //  divide ratio=1, frequency=7 (default values)
  command(0x78);        // OLED characterization: OLED command set disabled (SD=0) (exit from OLED command set)

  if (Lines > 2)
  command(0x09);  // Extended function set (RE=1): 5-dot font, B/W inverting disabled (def. val.), 3/4 lines
  else
  command(0x08);  // Extended function set (RE=1): 5-dot font, B/W inverting disabled (def. val.), 1/2 lines

  command(0x06);        // Entry Mode set - COM/SEG direction: COM0->COM31, SEG99->SEG0 (BDC=1, BDS=0)
  command(0x72);        // Function selection B:
  data(0x0A);           //  ROM/CGRAM selection: ROM C, CGROM=250, CGRAM=6 (ROM=10, OPR=10)
  command(0x79);        // OLED characterization: OLED command set enabled (SD=1)
  command(0xDA);        // Set SEG pins hardware configuration:
  command(0x10);        //  alternative odd/even SEG pin, disable SEG left/right remap (default values)
  command(0xDC);        // Function selection C:
  command(0x00);        //  internal VSL, GPIO input disable
  command(0x81);        // Set contrast control:
  command(0x7F);        //  contrast=127 (default value)
  command(0xD9);        // Set phase length:
  command(0xF1);        //  phase2=15, phase1=1 (default: 0x78)
  command(0xDB);        // Set VCOMH deselect level:
  command(0x40);        //  VCOMH deselect level=1 x Vcc (default: 0x20=0,77 x Vcc)
  command(0x78);        // OLED characterization: OLED command set disabled (SD=0) (exit from OLED command set)
  command(0x20 | Row_bit); // Function set: fundamental command set (RE=0) (exit from extended command set), lines #
  command(0x01);        // Clear display
  delay(2);             // After a clear display, a minimum pause of 1-2 ms is required
  command(0x80);        // Set DDRAM address 0x00 in address counter (cursor home) (default value)
  command(0x0C);        // Display ON/OFF control: display ON, cursor off, blink off
  delay(250);           // Waits 250 ms for stabilization purpose after display on
}

/* end()
   Turn off the OLED and set the control pins to default values for the OLED
*/
void NewhavenOLED::end()
{
  command(0x20 | Row_bit); // RE=0 (exit from extended command set), lines #
  command(0x08);        // display off, cursor off, blink off (default values)

  // Put control pins in default state
  pinMode(MOSI_pin, OUTPUT);
  digitalWrite(MOSI_pin, LOW);
  pinMode(SCK_pin, OUTPUT);
  digitalWrite(SCK_pin, HIGH);
  if (CS_pin != NO_PIN) {
    pinMode(CS_pin, OUTPUT);
    digitalWrite(CS_pin, HIGH);
  }
  if (RST_pin != NO_PIN) {
    pinMode(RST_pin, OUTPUT);
    digitalWrite(RST_pin, HIGH);
  }

}

/*-------------- PRIVATE METHODS -------------------------------------------*/

// Private function to toggle SCK_pin
void NewhavenOLED::clock_cycle()
{
  // This function uses a hard-coded delay to ensure proper timing of
  // the SCLK. Per the US2066 driver chip datasheet, the clock period
  // needs to be a minimum of 1 us. While the digitalWrite() function
  // tends to be "slow" (at least with respect to direct port manipulation),
  // some chips with faster clock speeds may still change the pin state
  // faster than 1 us when using digitalWrite().
  // Note that the Arduino and Energia documentation state that the function
  // is only accurate for values of  3us and greater; hence, the delay below
  // uses 3 us, even though a 1 us delay would be sufficient.
  digitalWrite(SCK_pin, LOW);        // Sets LOW the SCLK line of the display
  delayMicroseconds(3);              // Waits >1 us (required for timing purpose)
  digitalWrite(SCK_pin, HIGH);       // Sets HIGH the SCLK line of the display
  delayMicroseconds(3);              // Waits >1 us (required for timing purpose)
}

// Private function to package the byte and transmit to OLED
void NewhavenOLED::send_byte(byte tx_b)
// A byte is split into upper and lower nybbles, with 4 low bits filling the transfer
// In other words: low data nybble, 4 low bits, high data nybble, 4 low bits
{
  byte i;

  // send the lower nybble
  digitalWrite(MOSI_pin, tx_b & 0x01);
  clock_cycle();
  digitalWrite(MOSI_pin, tx_b & 0x02);
  clock_cycle();
  digitalWrite(MOSI_pin, tx_b & 0x04);
  clock_cycle();
  digitalWrite(MOSI_pin, tx_b & 0x08);
  clock_cycle();

  for (i = 0; i < 4; i++)  // 4 low data filler bits
  {
    digitalWrite(MOSI_pin, LOW);
    clock_cycle();
  }

  // send the upper nybble
  digitalWrite(MOSI_pin, tx_b & 0x10);
  clock_cycle();
  digitalWrite(MOSI_pin, tx_b & 0x20);
  clock_cycle();
  digitalWrite(MOSI_pin, tx_b & 0x40);
  clock_cycle();
  digitalWrite(MOSI_pin, tx_b & 0x80);
  clock_cycle();

  for (i = 0; i < 4; i++) // 4 low data filler bits
  {
    digitalWrite(MOSI_pin, LOW);
    clock_cycle();
  }
}
