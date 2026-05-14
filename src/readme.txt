TO BUILD
--------
Ensure the avr gnu toolchain is installed and simply 'make' from this 'src' directory.

It will produce ToothEater.hex which can then either be loaded into the simulator or atTiny85 chip.


PRE-RELEASED BUILDS
-------------------
See the hex files in the 'release' directory.


BURNING THE FIRMWARE ONTO THE ATMEL atTiny85
--------------------------------------------
In linux, this command can be used from the directory containing the hex file.

avrdude -c usbtiny -p t85 -U flash:w:ToothEater.hex

