/* Library designed for Newhaven OLEDs based on US2066 controller.
   This includes NHD-0216CW-Ax3, NHD-0220CW-Ax3.
   Implemented using "bit-bang" SPI -- do not user hardware SPI pins
   if other devices are using hardware SPI.

   Pin Configuration:
   OLED                ARDUINO
   ----                -------
   Pin 1 (Vss)         GND
   Pin 2 (VDD)         5V
   Pin 3 (REGVDD)      5V
   Pins 4 to 6         GND
   Pin 7 (SCLK)        Pin number defined with "sckPin" in Newhaven_OLED constructor
   Pin 8 (SID)         Pin number defined with "mosiPin" in Newhaven_OLED constructor
   Pin 9 (SOD)         No connection
   Pins 10 to 14       GND
   Pin 15 (/CS)        Pin number defined with "csPin" in Newhaven_OLED constructor
                        This pin can alternatively  be tied to GND if this is the only display
                        controlled with sckPin and mosiPin. If csPin is tied
                        to GND, then define it as NO_PIN in the constructor.
   Pin 16 (/RES)       Pin number defined with "rstPin" in Newhaven_OLED constructor
                        This pin can alternatively be tied to 5V, in which case
                        it should be defined as NO_PIN in the constructor.
                        If it is associated with an actual pin, then the software
                        should set it to HIGH unless the display is being reset.
    Pins 17-19 (BS0-2)  GND
    Pin 20 (Vss)        GND
*/
#ifndef NEWHAVEN_OLED
#define NEWHAVEN_OLED

#include <Arduino.h>

#define NO_PIN 255

class NewhavenOLED {
public:
  NewhavenOLED(byte lines, byte cols, byte mosiPin, byte sckPin, byte csPin, byte rstPin);
  void command(byte c);
  void data(byte d);
  void clear();
  void begin();
  void end();
private:
  byte Lines, Cols;
  byte MOSI_pin, SCK_pin, CS_pin, RST_pin;
  byte Row_bit;
  void clock_cycle();
  void send_byte(byte c);
};
#endif
