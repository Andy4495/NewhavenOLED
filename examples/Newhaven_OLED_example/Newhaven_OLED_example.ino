/*
   Demo_NHD0420CW-Ax3_SPI.ino

   Tutorial sketch for use of character OLED slim display family by Newhaven with Arduino Uno.
   Models: NHD0420CW-Ax3, NHD0220CW-Ax3, NHD0216CW-Ax3. Controller: US2066
   Serial (SPI) interface.


   First shows the direct use of the command() and data() methods to display
   a text message and a sequence of angle bracket symbols.
   Next, makes use of the various write() methods to ASCII characters one at
   a time across the display, then individually updates a single character in
   the midddle of the first line. And then once again displays a text message.

   This sketch assumes the use of a 2x16 display; if different, modify the values of the two variables
   ROW_N and COLUMN_N.

   The circuit:
   OLED pin 1 (Vss)    to Arduino pin ground
   OLED pin 2 (VDD)    to Arduino pin 5V
   OLED pin 3 (REGVDD) to Arduino pin 5V
   OLED pin 4 to 6     to Vss ground
   OLED pin 7 (SCLK)   to Arduino pin D13 (SCK)
   OLED pin 8 (SID)    to Arduino pin D11 (MOSI)
   OLED pin 9 (SOD)    No connection
   OLED pin 10 to 14   to Vss ground
   OLED pin 15 (/CS)   to Vss ground  (or to Arduino pin D2, in case of use of more than one display)
   OLED pin 16 (/RES)  to Arduino pin Reset or VDD 5V (or to Arduino pin D3, to control reset by sw)
   OLED pin 17 (BS0)   to Vss ground
   OLED pin 18 (BS1)   to Vss ground
   OLED pin 19 (BS2)   to Vss ground
   OLED pin 20 (Vss)   to Vss ground

   Original example created by Newhaven Display International Inc.
   Modified and adapted to Arduino Uno 30 Mar 2015 by Pasquale D'Antini
   Modified 19 May 2015 by Pasquale D'Antini:
   https://www.newhavendisplay.com/NHD_forum/index.php?topic=914.0
   
   Further modifications to use "NewhavenOLED" library by Andy4495, Dec 2017, Jan 2019, July 2024.

   This example code is in the public domain.
*/

#include <NewhavenOLED.h>

const byte ROW_N = 2;                 // Number of display rows
const byte COLUMN_N = 16;             // Number of display columns

const byte CS = 2;                    // Arduino's pin assigned to the /CS line
const byte RES = NO_PIN;              // Arduino's pin assigned to the Reset line (optional, can be always high)
const byte SCLK = 13;                 // Arduino's pin assigned to the SCLK line
const byte SDIN = 11;                 // Arduino's pin assigned to the SID line

const byte TEXT[4][21] = {"1-Newhaven Example--",
                          "2-------Test--------",
                          "3-16/20-Characters--",
                          "4!@#$%^&*()_+{}[]<>?"
                         };         // Strings to be displayed

byte new_line[4] = {0x80, 0xA0, 0xC0, 0xE0};               // DDRAM address for each line of the display
NewhavenOLED oled(ROW_N, COLUMN_N, SDIN, SCLK, CS, RES);
// _______________________________________________________________________________________


void output(void)                    // SUBROUTINE: DISPLAYS THE FOUR STRINGS, THEN THE SAME IN REVERSE ORDER
{
  byte r = 0;                        // Row index
  byte c = 0;                        // Column index

  oled.command(0x01);                // Clears display (and cursor home)
  delay(2);                          // After a clear display, a minimum pause of 1-2 ms is required

  for (r = 0; r < ROW_N; r++)        // One row at a time,
  {
    oled.command(new_line[r]);       //  moves the cursor to the first column of that line
    for (c = 0; c < COLUMN_N; c++)   // One character at a time,
    {
      oled.data(TEXT[r][c]);         //  displays the corresponding string
    }
  }

  delay(2000);                       // Waits, only for visual effect purpose

  for (r = 0; r < ROW_N; r++)        // One row at a time,
  {
    oled.command(new_line[r]);       //  moves the cursor to the first column of that line
    for (c = 0; c < COLUMN_N; c++)   // One character at a time,
    {
      oled.data(TEXT[3 - r][c]);     //  displays the corresponding string (in reverse order)
    }
  }
}
// _________________________________________________________________________________________

void blocks(void)    // SUBROUTINE: FILLS THE ENTIRE DISPLAY WITH ALTERNATING ANGLE BRACKETS
{
  byte r = 0;                       // Row index
  byte c = 0;                       // Column index

  oled.clear();                     // Clear display (and cursor home)

  for (r = 0; r < ROW_N; r++)       // One row at a time,
  {
    oled.command(new_line[r]);      //  moves the cursor to the first column of that line
    for (c = 0; c < COLUMN_N; c++)  // One character at a time,
    {
      if (c % 2)
        oled.data(0xFC);            // Alternate the arrow direction
      else
        oled.data(0xFB);
      delay(25);                    // Waits, only for visual effect purpose
    }
  }
}
// _______________________________________________________________________________________

void oneAtATime() {
  char c = 'A';
  oled.clear();

  for (int i = 0; i < 2 * ROW_N * COLUMN_N; i++) {
    oled.write(c);
    delay(100);
    c++;
  }
}
// _______________________________________________________________________________________

void singleUpdate() {
  char c = '0';
  oled.clear();

  for (int i = 0; i < COLUMN_N; i++) {
    oled.write(COLUMN_N/2, 0, c++);
    delay(100);
  }

}
// _______________________________________________________________________________________

void allAtOnce() {
  char s[] = "The quick brown fox jumps over the lazy dog. Now is the time. One ring to rule them all.";

  oled.write(s);

}
// _______________________________________________________________________________________


void setup(void)
{
  oled.begin();                    // Setup control pins and initialize OLED controller

  if (ROW_N == 2)                  // Update DDRAM address for 2-line mode
    new_line[1] = 0xC0;            // 3- and 4-line use row addresses as defined above

}
// _______________________________________________________________________________________

void loop(void)
{
  output();
  delay(1000);                       // Waits, only for visual effect purpose
  blocks();
  delay(1000);                       // Waits, only for visual effect purpose
  oneAtATime();
  delay(1000);
  singleUpdate();
  delay(1000);
  allAtOnce();
  delay(1000);
}
