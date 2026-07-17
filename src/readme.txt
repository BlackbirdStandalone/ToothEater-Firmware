--------
TO BUILD
--------

There are 4 build options available depending on what you need to get your bike
running properly. Select one of the 4 stategies below by defining 'CDEFS' in the
makefile with one of the 4 definitions. I.e. 

  (1) CDEFS=-DORIGINAL (default)
  (2) CDEFS=-DORIGINAL_CAM_PULSES_ON_STARTUP_ONLY
  (3) CDEFS=-DCPI
  (4) CDEFS=-DCPI_CAM_PULSES_ON_STARTUP_ONLY

The 4 strategies were evolved during development i.c.w experimentation with rusefi
as the ECU. Depending one which ECU you decide to use, you may end up finding
an option that works best. After much experimentation I settled on option (4).
Option (1) is the default.

They are all described below.

#----------------------------------------------------------------------------
#                           STRATEGY SELECTION
#----------------------------------------------------------------------------
#
# Select the functionality you would like from the tooth eater by selecting
# a strategy.
#
# The build options are as follows:
# 
# (1) The functionality of the original v1.0 firmware. This build will align the
#     output cam tooth to the real 'second paired' tooth. There will also be a
#     continuous stream of single cam pulses as the engine is running. This
#     option is the true 'Tooth Eat' mode where two of the three cam teeth are
#     deleted as the engine in running. Another important note with this original
#     version is that both the crank and cam signals are 'released' at the same
#     time to the down-stream ECU once the cam signal has been 'synced' by the
#     tooth eater. This may or may not matter to your ECU.
#
#     To compile this functionality, ensure CDEFS is defined as such:
#       CDEFS=-DORIGINAL
#
#
# (2) This is the same as option (1), however will only produce cam signals
#     during the first 'x' number of cam rotations (where 'x' is defined by you)
#     when the engine is being cranked (upon start-up).
#       This option exists in the event that the ECU maintains its own cam
#     signal internally after the engine has been started. If further cam signals
#     were to be transmitted into the ECU by the tooth eater, the ECU would see the
#     extra cam pulses as conflicting with its own, causing de-sync due to pulse misalignment.
#     This may appear in the form of an engine that stutters during acceleration where
#     the stutter is caused by the de-sync/re-sync conflict between the tooth eater and
#     the ECU. By only transmitting a defined number of cam pulses on cranking and startup,
#     this allows the ECU to initially gain the cam SYNC information from the tooth eater,
#     but then manage its own cam sync internally and not conflict with the tootheater.
#       
#     To compile this functionality, ensure CDEFS is defined as such:
#       CDEFS=-DORIGINAL_CAM_PULSES_ON_STARTUP_ONLY
#
#
# (3) This option is known as 'Cam pulse insertion mode' (CPI). This is not strict 
#     tooth eating as option (1) implies, but rather a mode where the tooth eater
#     will insert its own synthesised cam tooth once every 24 crank pulses. This
#     mode effectively produces the same output as (1) but in a different way.
#     The cam is still used to gain initial sync, however the real cam pulse is
#     ignored at this point and replaced by a cam pulse that is 'inserted' on every
#     24'th crank pulse (I.e. crank pulse modulus 24 is 1 cam pulse).
#     This mode is aligned to the first crank pulse that succeeds the 'isolated'
#     tooth and finds cam sync a little faster than option (1). Another difference
#     with this strategy compared to (1) is that the tooth eater 'releases' a stream
#     of crank pulses to the ECU before it releases its first cam pulse. This is
#     somewhat of a realistic signal of what an ECU can expect when trying to sync
#     upon engine startup.
#
#     To compile this functionality, ensure CDEFS is defined as such:
#       CDEFS=-DCPI
#
#
# (4) This option is the same as (3) however only streams 'x' amount of output
#     cam pulses on startup as described in (2).
#
#     To compile this functionality, ensure CDEFS is defined as such:
#       CDEFS=-DCPI_CAM_PULSES_ON_STARTUP_ONLY
#
#
# Notes:
#  (a) If using either option (2) or (4), you can modify the number of initial
#      cam pulses transmitted by the tooth eater on engine start by modifying
#      the definition 'STARTUP_CAM_PULSES' found at the top of 'main_defs.h'.
#
#  (b) The ignition timing on your engine will differ between option (1) and (3).
#      So you will need to establish your ignition global offset angle for both
#      build types if you want to experiment with the 'original' and 'cpi' builds.
#
#  (c) On my motorcycle running rusefi, option (4) is what I use, but this might
#      not be the correct choice for you. Option (1) or (3) might be the better
#      choice to begin with depending on your ECU. If you can verify that your
#      ECU maintains its own cam sync once the engine has been started, then
#      option (2) or (4) might be worth sticking with to avoid cam pulse conflicts.
#        The above options were developed as a result of much experimentation
#      with rusefi configuration options. I.e. Working out what works and what doesn't.
#      I found that microRusEfi likes to manage its own cam sync internally once
#      initially synced by the tooth eater. Conflicts would arise if the tooth eater
#      continued to stream out its cam pulses to it. So option (4) worked best for me.
#
#      So, I use option (4) with microRusEfi with the trigger arrangement as follows:
#
#         Trigger Type: '12Crank/24Cam'
#         Crank Sensor: '45 - VR/Hall Crank'
#         Primary Edge: 'Rising'
#         Cam Mode (intake): 'Single Tooth'
#         Cam Mode (exhaust): 'Inactive'
#         Cam sensor bank #1 intake: '25 - Hall'
#         Intake cam edge select: 'Falling'
#
#         Trigger Advance angle (deg BTDC): '70'
#
#  (d) To date, I have only tested the Tooth eater against microRusEfi as an ECU.
#      I would like (one day) also have the Blackbird running on speeduino and also
#      obtain the necessary trigger configuration for it to be able to run the
#      motorcycle using the tooth eater. I will update this file once this information
#      comes to light in the future.
#----------------------------------------------------------------------------


Continuing ...

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
v1.2 - Adding 4 available build strategies depending on user requirements.
       This is to incorporate all use case operations as the firmware has
       evolved since v1.0. This has been tested against hardware THT v1.1.
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

