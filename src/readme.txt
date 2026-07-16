--------
TO BUILD
--------

There are 4 build options available depending on what you need.





Ensure the avr gnu toolchain is installed and simply 'make' from this 'src' directory.

It will produce 'ToothEater.hex' which can then either be loaded into the simulator or atTiny85 chip.


-----------------
OFFICIAL RELEASES 
-----------------
These releases have been pre-tested to work on the motorcycle against v1.1 of the hardware.

See the hex files in the 'release' directory. (Compiled with avr-gcc version 5.4.0)


--------------------------------------------
BURNING THE FIRMWARE ONTO THE ATMEL atTiny85
--------------------------------------------
In linux, this command can be used from the directory containing the hex file.

avrdude -c usbtiny -p t85 -U flash:w:<filename>

Eg.
  avrdude -c usbtiny -p t85 -U flash:w:ToothEater.hex

or
  avrdude -c usbtiny -p t85 -U flash:w:./releases/ToothEater_v1.1.hex 


When you burn the binary into the atTiny85 using the programmer with the following command,

  avrdude -c usbtiny -p t85 -U flash:w:./releases/ToothEater_v1.1.hex 

if successful, your output should look something like this:

  avrdude: AVR device initialized and ready to accept instructions
  
  Reading | ################################################## | 100% 0.01s
  
  avrdude: Device signature = 0x1e930b (probably t85)
  avrdude: NOTE: "flash" memory has been specified, an erase cycle will be performed
           To disable this feature, specify the -D option.
  avrdude: erasing chip
  avrdude: reading input file "./releases/ToothEater_v1.1.hex"
  avrdude: input file ./releases/ToothEater_v1.1.hex auto detected as Intel Hex
  avrdude: writing flash (524 bytes):
  
  Writing | ################################################## | 100% 0.85s
  
  avrdude: 524 bytes of flash written
  avrdude: verifying flash memory against ./releases/ToothEater_v1.1.hex:
  avrdude: load data flash data from input file ./releases/ToothEater_v1.1.hex:
  avrdude: input file ./releases/ToothEater_v1.1.hex auto detected as Intel Hex
  avrdude: input file ./releases/ToothEater_v1.1.hex contains 524 bytes
  avrdude: reading on-chip flash data:
  
  Reading | ################################################## | 100% 1.30s
  
  avrdude: verifying ...
  avrdude: 524 bytes of flash verified
  
  avrdude: safemode: Fuses OK (E:FF, H:DF, L:62)
  
  avrdude done.  Thank you.


---------------
VERSION HISTORY
---------------
-------------------------------------------------------------------------------
v1.1 - Improved engine startup times over v1.0 due to improved algorithm.
     - Added Tacho output feature.

Authenticity .hex file check:
md5sum ./releases/ToothEater_v1.1.hex 
1684e1e37aa2abb5f0f6c546ce50ff0b ./releases/ToothEater_v1.1.hex
-------------------------------------------------------------------------------
v1.0 - Initial version

Attains sync in a worst case scenario of 2 cam rotations (4 crank rotations).

Authenticity .hex file check:
md5sum ./releases/ToothEater_v1.0.hex 
39cb44cfd7c7d3e81f36deadc50aef02  ./releases/ToothEater_v1.0.hex
-------------------------------------------------------------------------------


-----------
OTHER NOTES
-----------
1. Only testing with the NCV1124 chip at the moment. I have not tried the MAX9926 VR chip yet.

