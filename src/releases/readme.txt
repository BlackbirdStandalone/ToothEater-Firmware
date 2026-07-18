Pre-compiled .hex files are available in this directory for programming your ATtiny85 microcontroller.

Please read the 'readme.txt' in the 'src' directory.

If you wish to use either:
  'ToothEater_v1.2_ORIGINAL_CAM_PULSES_ON_STARTUP_ONLY.hex', or
  'ToothEater_v1.2_CPI_CAM_PULSES_ON_STARTUP_ONLY.hex', then
     please note that these have been compiled with a default of 5 initial cam pulses on startup only.
     If you need a different value that this, then you can change it to a value that suits you by changing
       '#define STARTUP_CAM_PULSES 5' in main_defs.h, then rebuilding.

These files are also loadable into the simulator if you wish to view how the signals operate.

