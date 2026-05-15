--------
TO BUILD
--------
Ensure the avr gnu toolchain is installed and simply 'make' from this 'src' directory.

It will produce 'ToothEater.hex' which can then either be loaded into the simulator or atTiny85 chip.


-----------------
OFFICIAL RELEASES 
-----------------
These releases have been pre-tested to work on the motorcycle against v1.x of the hardware.

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
v1.1 - Corrected alignment to 'First paired'

If using RusEFI, the 'Trigger Advance Angle' is +190 Deg for #1/#4 TDC alignment.
I.e. Setup -> Trigger in the Tuner Studio menu.

Authenticity .hex file check:
md5sum ./releases/ToothEater_v1.1.hex 
b87d97d2e7928088f92aec5555e7095f  ./releases/ToothEater_v1.1.hex
-------------------------------------------------------------------------------
v1.0 - Initial version

Attains sync in a worst case scenario of 2 cam rotations (4 crank rotations).

If using RusEFI, the 'Trigger Advance Angle' is +130 Deg for #1/#4 TDC alignment.
I.e. Setup -> Trigger in the Tuner Studio menu.

Authenticity .hex file check:
md5sum ./releases/ToothEater_v1.0.hex 
39cb44cfd7c7d3e81f36deadc50aef02  ./releases/ToothEater_v1.0.hex
-------------------------------------------------------------------------------


-----------
OTHER NOTES
-----------
1. I have noticed an anomoly with the NCV 1124 chip or perhaps its using it in conjunction with the hardware.
   It seems to have a quirk that I havn't had time to investigate yet. It starts and runs the bike just fine,
   however I have noticed that the 3 tooth pattern is inverted when cranking, then becomes non-inverted when
   the engine is running. This can be observed on the oscilloscope when probing the insertable VR board on
   pin 3 from the top. I.e. The Cam TTL signal as conditioned by the VR, but not yet processed by the tooth
   eater.
     What is important is the repeatablity in starting the bike correctly on the first paired tooth. Once the
   initial sync has been made, then the ECU will maintain its own internal CAM sync.
     I have not tried the MAX9926 VR chip yet.

