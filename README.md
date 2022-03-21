# NewhavenOLED Library

[![Arduino Compile Sketches](https://github.com/Andy4495/NewhavenOLED/actions/workflows/arduino-compile-sketches.yml/badge.svg)](https://github.com/Andy4495/NewhavenOLED/actions/workflows/arduino-compile-sketches.yml)
[![Check Markdown Links](https://github.com/Andy4495/NewhavenOLED/actions/workflows/CheckMarkdownLinks.yml/badge.svg)](https://github.com/Andy4495/NewhavenOLED/actions/workflows/CheckMarkdownLinks.yml)

This Arduino library is designed to work with Newhaven OLEDs based on the US2066 controller chip using SPI (serial interface) mode. This includes models NHD-0216CW-Ax3 and NHD-0220CW-Ax3. Other display models which use the same controller chip may also work, but were not tested with this library.

The library is based on example code provided by Newhaven Display International and [updates][2] made by Pasquale D'Antini. The original example code is apparently no longer available on Newhaven's web site, but a similar example is still [available][1] in Newhaven's updated Support Center.

This library further expands on these examples by packaging the code into [Arduino library format][7], providing cleaner selection of control pin numbers (SDI, SCK, CS, RES), improving the bit shift data transfer, and adding several `write()` methods to ease the use of the display.

Although the Newhaven data sheets do not directly mention support for 3.3V operation (e.g., when using MSP430 controllers), I have had success with configurations using 3.3V logic and either 5V or 3.3V VDD supply voltage.

This library implements a "bit-bang" SPI, and can therefore use any available I/O pins to control the display. However, if your project is using hardware SPI to control other devices, then do not use those hardware SPI pins. For example, do not use pins 10-13 on an Arduino UNO.

## Usage

*Refer to the example sketch in the `examples` folder.*

Use the constructor to set up the display size (rows and columns) and pin numbers for the control pins. For example,

    NewhavenOLED oled(2, 16, 3, 4, 5, NO_PIN);

In the above example, the display has 2 rows, 16 columns, the SDI pin (MOSI) is connected to Arduino pin 3, the SCLK pin (SCK) is connected to pin 4, and the /CS pin (chip select) is connected to  pin 5. The /RES pin (reset) is hardwired to VDD (5V), so "NO_PIN" is used in the constructor.

After creating a NewhavenOLED object, call the `begin()` method before using it. This initializes the OLED controller chip and makes it ready to display information:

    begin()

There are several methods for writing characters and controlling the display:

    clear()

Clears the display and sets the current cursor position to the top left (0,0).

    setCursor(col, row)

Sets the cursor to the position specified by (col,row). This is where the next character will be written when using `write(c)`.  
If the cursor position is outside the bounds of the display, then this method has no effect.

    write(c)

Writes the single character `c` at the current cursor position and advances the cursor to the next position (wrapping the end of line or to the top of display as needed).

    write(col, row, c)

Writes character `c` to position (col,row) on the display without changing any other characters on the display. Advances cursor to next position (wrapping at the end of a line if needed).  
If the cursor position is outside the bounds of the display, then this method has no effect.

    write(s)

Clear the display and writes the c-string pointed to by `s` to the display. `s` needs to be at least as large as the number of characters in the display. Any characters after the end of the display are ignored.

You can also use `command(c)` and `data(d)` to send commands and data to the display. Refer to the datasheets referenced below for further details.

## Hardware Pin Configuration for SPI (Serial Interface) Mode

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

## Additional References

+ NHD-0216CW [datasheet][3].
+ NHD-0220CW [datasheet][4].
+ US2066 controller chip [datasheet](http://www.newhavendisplay.com/app_notes/US2066.pdf).
+ Sample code from [Newhaven Display International][1]
+ Newhaven [US2066 code][5] on GitHub.
+ [Tutorial code][2] by Pasquale D'Antini, also availalbe on [GitHub][6].

## License

The software and other files in this repository are released under what is commonly called the [MIT License][100]. See the file [`LICENSE.txt`][101] in this repository.

<!-- markdown-link-check-disable-next-line -->
[1]: https://support.newhavendisplay.com/hc/en-us/articles/4413876825111-NHD-0216MW-0216CW-0220CW-0420CW-with-Arduino
[2]: https://www.newhavendisplay.com/NHD_forum/index.php?topic=914.0
[3]: https://www.newhavendisplay.com/specs/NHD-0216CW-AY3.pdf
[4]: https://www.newhavendisplay.com/specs/NHD-0220CW-AB3.pdf
[5]: https://github.com/NewhavenDisplay/NHD_US2066
[6]: https://github.com/oldmaker/hello-world
[7]: https://arduino.github.io/arduino-cli/0.21/library-specification/
[100]: https://choosealicense.com/licenses/mit/
[101]: ./LICENSE.txt
[200]: https://github.com/Andy4495/NewhavenOLED

[//]: # (The link for reference [1] above returns error 403, but is still loads the correct page. So have the Markdown Link Check ignore that link.)
[//]: # (Dead link from older version of README: original location of sample code https://newhavendisplay.com/app_notes.html)
