NewhavenOLED Library
====================

This library is designed to work with Newhaven OLEDs based on the US20166
controller chip. This includes NHD-0216CW-Ax3 and NHD-0220CW-Ax3. Others
may also work, but were not tested with this library.

This library should work with both Arduino and Energia MSP430 projects. 

Although the Newhaven data sheets don't directly mention support for 3.3V
operation (e.g., when using MSP430 controllers), I have had success with 
configurations using 3.3V logic and either 5V or 3.3V VDD supply voltage. 

The library implements a "bit-bang" SPI, and can therefore use any available
Arduino I/O pins to control the display. However, if your project is using
hardware SPI to control other devices, then do not use the hardware SPI pins.
For example, do not use pins 10-13 on an Arduino UNO (other Arduinos may use
other pins for hardware SPI).

Usage
-----

Use the constructor to set up the display size (rows and columns) and Arduino
pin numbers for the control pins. For example,

    NewhavenOLED oled(2, 16, 3, 4, 5, NO_PIN);

In the above example, the display has 2 rows, 16 columns, the SDI pin (MOSI)
is connected to Arduino pin 3, the SCLK pin (SCK) is connected to Arduino
pin 4, and the /CS pin (chip select) is connected to Arduino pin 5. The
/RES pin (reset) is hardwired to VDD (5V) and is not controlled by the
Arduino (so "NO_PIN" is used in the constructor).

After creating a NewhavenOLED object, call the "begin()" method before
using it. This initializes the OLED controller chip and makes it ready
to display information. Continuing the example from above:

    oled.begin();

You can then use the methods command(c) and data(d) to send commands and
data to the display. Refer to the example sketch in the examples folder,
along with the datasheets referenced below for further details.


Hardware Pin Configuration
--------------------------

     OLED                ARDUINO/MSP430
     ----                --------------
     Pin 1 (Vss)         GND
     Pin 2 (VDD)         5V (or 3.3V)
     Pin 3 (REGVDD)      5V (or 3.3V, same as pin 2)
     Pins 4 to 6         GND
     Pin 7 (SCLK)        Pin number defined with "sckPin" in NewhavenOLED constructor
     Pin 8 (SID)         Pin number defined with "mosiPin" in NewhavenOLED constructor
     Pin 9 (SOD)         No connection
     Pins 10 to 14       GND
     Pin 15 (/CS)        Pin number defined with "csPin" in NewhavenOLED constructor
                         This pin can alternatively  be tied to GND if this is the only display
                         controlled with sckPin and mosiPin. If csPin is tied
                         to GND, then define it as NO_PIN in the constructor.
     Pin 16 (/RES)       Pin number defined with "rstPin" in NewhavenOLED constructor
                         This pin can alternatively be tied to 5V, in which case
                         it should be defined as NO_PIN in the constructor.
                         If it is associated with an actual pin, then the software
                         should set it to HIGH unless the display is being reset.
     Pins 17-19 (BS0-2)  GND     
     Pin 20 (Vss)        GND     

Additional References
---------------------

+ NHD-0216CW [datasheet](https://www.newhavendisplay.com/specs/NHD-0216CW-AY3.pdf).
+ NHD-0220CW [datasheet](https://www.newhavendisplay.com/specs/NHD-0220CW-AB3.pdf).
+ US2066 controller chip [datasheet](http://www.newhavendisplay.com/app_notes/US2066.pdf).
+ Sample code from [Newhaven Display International](https://newhavendisplay.com/app_notes.html).
