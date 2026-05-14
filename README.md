<table border="1">
<tr>
<td align="center" valign="center">
<img 
    style="display: block; 
           margin-left: auto;
           margin-right: auto;
           width: 80%;"
    src="./images/TE_3.png#center">
</img>
</td>
</tr>
</table>

<br /><br />

# Tooth Eater Module

Various early 2000's Honda CBR models use a 3-spoke cam trigger wheel. Some examples are the:

<ul>
  <li>Honda Blackbird CBR1100XX</li>
  <li>Honda ST1300</li>
  <li>Honda CBR600</li>
  <li>Honda CBR954</li>
  <li>Honda CBR1000RR</li>
  <li>Honda Aquatrax jet ski</li>
</ul> 
Other models may also use this pattern as well.

<br />
<table border="1">
<tr>
<td align="center" valign="center">
<img 
    style="display: block; 
           margin-left: auto;
           margin-right: auto;
           width: 50%;"
    src="./images/BB_ExhCam.png#center">
</img>
<p style="text-align: center;">The tooth shown in red is the reference tooth that is allowed through to the stand alone ECU. The other teeth are digitally deleted (eaten) by the tooth eater. This reference tooth also happens to align with TDC cylinder #1 & #4.</p>
</table>
</td>
</tr>
<br />


While this 3-spoke pattern is designed to work with the OEM Keihin ECU, it will not work with many stand alone ECU's due to its peculiar tooth arrangement. Many stand alone ECU systems available on the market today such as Megasquirt, Speeduino, RusEFI and others will cater for a single tooth pulse from the camshaft without fuss since this is a very standard configuration. In the past, this issue was usually resolved by physically removing two teeth from the original cam trigger wheel (I.e. by grinding or hack-saw), leaving one tooth behind. This allows the stand alone unit but also renders the Keihin unit inoperative since it expects the 3 teeth. If the original Keihin unit was to work again then a new cam trigger wheel would need to be re-installed onto the camshaft. This project aims to keep the bike intact allowing one to retain both the existing Honda wiring harness as well as the original trigger system. This way, there is no need to cut into the Honda wiring harness nor physically grind the two teeth off the camshaft trigger wheel. Therefore making it unnecessary to open the top end of the motor and also allowing for the original Keihin unit to be reinstalled easily if needed.

You can watch the tooth eater in action working together with the microRusEfi stand alone ECU by clicking on the image below.
[![Watch the video](./images/thumb.jpg)](https://vimeo.com/reviews/9667708b-47c5-4177-8223-fa850c1e370e/videos/1182912498)
<br />

<p>
This 'Tooth Eater' project allows you to run the bike without having to get into the top end and remove the teeth off the exhaust camshaft trigger wheel. Instead, this small PCB and its associated firmware digitally removes 'eats' the two teeth, leaving one behind to be processed by your stand alone unit. With this unit your stand alone ECU will only see a single cam pulse (home position) instead of three pulses every cam revolution. This should allow the Blackbird and other Honda models that use the same cam tooth pattern to be run with many of the available ECU systems on the market.
<br />
This Tooth Eater module performs the following tasks:<br />
</p>

<table border="1">

<tr> 
<td width="5%">
<strong>(1)</strong>
</td>
<td width="95%">
Converts the CBR's 3-spoke cam pattern into a single digital pulse making it compatible with other standalone ecu systems on the market.
</td>
</tr>

<tr>
<td width="5%">
<strong>(2)</strong>
</td>
<td width="95%">
Houses an on-board VR conditioner. This unit is a direct plug in module as used on Speeduino boards and it plugs straight into the tooth eater board (as seen in the image below). The Blackbird and other CBR's models require the VR module since they use inductive pickups on both the crank and the cam. The VR module converts the direct signal from the inductive pickups into a 0-5v TTL signal making it compatible with the stand alone ECU. This VR unit is readily available for purchase on the internet as part of the speeduino project.
</td>
</tr>

<tr>
<td width="5%">
<strong>(3)</strong>
</td>
<td width="95%">
On engine startup the tooth eater must 'SYNC' to the reference tooth on the cam trigger wheel (shown in red in the above image). It is critical that this tooth is always selected by the firmware on engine startup since the ECU will be also synced to it. The Tooth Eater will inhibit all output until engine SYNC has occurred. Once SYNC has occurred, the tooth eater will 'release' both the CRANK and CAM signals to the stand alone ECU for further processing & subsequent engine startup. This makes for a clean transition ensuring that the ECU receives crisp signals.
</td>
</tr>

<tr>
<td width="5%">
<strong>(4)</strong>
</td>
<td width="95%">
Attempt to keep cranking times as short as possible via its algorithm.
</td>
</tr>

</table>

<br />

<table border="1">
<tr>
<td align="center" valign="center">
<img 
    style="display: block; 
           margin-left: auto;
           margin-right: auto;
           width: 100%;"
    src="./images/TE-2.jpg#center">
</img>
<p style="text-align: center;">Tooth Eater board with VR module inserted atop.</p>
</table>

<br />

# General Overview
<p>
The tooth eater appears to deal with 'teeth' or pulses, however in reality it is only the rising edge of that pulse that is relevant. The ECU also deals only with edges and when it receives them it internally fires an interrupt service routine (ISR) that updates the crank and/or cam position in real time. All ECU's work with edges.
</p>
<p>
The early 2000's CBRs use a 12 tooth crank trigger wheel. The teeth are exactly spaced as the hours on a clock. In one crank revolution the ECU will see 12 pulses (360 deg). Two crank revolutions constitutes one engine cycle in which case 24 crank pulses will have been received by the ECU (720 deg). In one engine cycle exactly one cam pulse will be produced by the tooth eater.
</p>
<p>
The cam trigger wheel is as shown below. The tooth highlighted in red is the SYNC tooth as mentioned. I also refer to this tooth as the 'First Paired', since it is adjacent to its neighbour tooth that I refer to the 'Second Paired'. The remaining opposite tooth (on its own) I refer to as the 'Isolated' tooth. Once SYNCED, the firmware inhibits the 'Second Paired' and 'Isolated' tooth and only allows the 'First Paired' to pass through. It does this continually as the engine is running, however in most cases the ECU is only interested in the initial startup then manages its own cam sync internally. Once SYNCED, the crank signal is allowed to 'pass-through' to the ECU via the VR conditioner. When the engine is up and running, the crank signal is no longer processed by the tooth eater. It is briefly used at engine startup only in conjunction with the cam signal to gain SYNC as fast as possible for short cranking times. In the worst case scenario, SYNC should be attained in no more than 2 camshaft revolutions or 4 crank revolutions. Both cam and crank signals are used to gain initial sync.
</p>

<table border="1">
<tr>
<td align="center" valign="center">
<img 
    style="display: block; 
           margin-left: auto;
           margin-right: auto;
           width: 50%;"
    src="./images/SyncedCamTooth.png#center">
</img>
<p style="text-align: center;">The 'First Paired' tooth as shown by the red arrow is the SYNC tooth. The other two teeth are removed by the digital processing.</p>
</table>

<br />
When the engine is cranked (approximately 300 RPM), the raw camshaft signal as produced by the inductive pickup is shown below. This signal is not directly processable by the ECU as it first needs to be processed by the VR. The 3 spoke pattern can can be seen in the image below with their relative tooth spacings.

<table border="1">
<tr>
<td align="center" valign="center">
<img 
    style="display: block; 
           margin-left: auto;
           margin-right: auto;
           width: 80%;"
    src="./images/CAM-RawSensorOutput.png#center">
</img>
<p style="text-align: center;">Three levels of zoom shown of the raw cam signal directly from the inductive pickup.</p>
</table>


<br />
Likewise, the raw crank signal appears as follows. The amplitude fluctuations of the signal below also illustrate the engine spinning. The troughs highlight either one of two pistons in cylinder pairs 1&4 or 2&3 coming up on the compression stroke. This slows the crank down producing a less energetic signal. Conversely, when the pistons are in mid-stroke (90 crank degrees later) the crank spins faster since there is no compression to work against. This results in an inductive signal of greater strength and is shown by the peaks in the signal. Interestingly, with a little math and observing the time-based used on the scope, the cranking speed can be computed at 300 RPM's.

<table border="1">
<tr>
<td align="center" valign="center">
<img 
    style="display: block; 
           margin-left: auto;
           margin-right: auto;
           width: 80%;"
    src="./images/CRANK-RawSensorOutput.png#center">
</img>
<p style="text-align: center;">Raw crank signal directly from the inductive pickup.</p>
</table>

<p>
These raw signals are fed into the VR conditioner plug-in module. The TTL output is shown in the image below. Note the three inverted cam pulses shown in the upper half image. This TTL signal is fed into the tooth eater for processing. The lower half image shows the crank signal.
</p>

<table border="1">
<tr>
<td align="center" valign="center">
<img 
    style="display: block; 
           margin-left: auto;
           margin-right: auto;
           width: 100%;"
    src="./images/VR_TTL_Output.png#center">
</img>
<p style="text-align: center;">Crank and cam signals as processed by the VR plug-in module.</p>
</table>

<br />

Finally, the output produced by the tooth eater module. Both the cam and crank signals are now in an acceptable format for use with a stand alone ECU.
<table border="1">
<tr>
<td align="center" valign="center">
<img 
    style="display: block; 
           margin-left: auto;
           margin-right: auto;
           width: 100%;"
    src="./images/TE_PostProcessing.png#center">
</img>

<p style="text-align: center;">Tooth Eater processed output. This is fed into the downstream ECU.<br />I.e. 24 crank pulses and 1 cam pulse. This makes for a very standard configuration that most ECU's will support.</p>
</tr>
</table>

<br />

<p>
The following settings are RusEFI specific. Once in Tuner Studio, we configure the ECU for 12T crank (zero missing teeth) and 1T cam. For Speeduino, 'Dual Wheel' would need to be selected and configured in a similar manner.
</p>

<table border="1">
<tr>
<td align="center" valign="center">
<img 
    style="display: block; 
           margin-left: auto;
           margin-right: auto;
           width: 100%;"
    src="./images/RusEfiTriggerSettings.png#center">
</img>
<p style="text-align: center;">In my case I used RusEFI. The trigger configuration shown was used with the cam and crank signals.</p>
</table>

<br>
The output we expect from the high speed logger interface in Tuner Studio is shown.
<table border="1">
<tr>
<td align="center" valign="center">
<img 
    style="display: block; 
           margin-left: auto;
           margin-right: auto;
           width: 100%;"
    src="./images/TraceLogged.png#center">
</img>
<p style="text-align: center;">This is the desired output we want to see. I.e. One cam pulse per two crank revolutions (24 crank pulses).</p>
</table>

<br /><br />
# Principles of Operation
<p>
The tooth eater module depends on incoming crank and cam pulses to function with the cam pulse being of primary importance. The orientation of the camshaft immediately prior to starting the engine will be in a random position. Once cranking begins the tooth eater must attempt to gather all positional information as quickly and efficiently as possible in order to locate the reference cam tooth and send it on as a SYNC signal to the stand alone ECU. This is in the interests of quick engine starting.<br />
</p>
<p>
The way it achieves this is via two interrupt service routines (ISR's) coupled with the use of a simple state machine. Both the crank and cam each have their own ISR and fire their respective ISR's as each pulse arrives. When the engine is started, the first action that takes place is at the arrival of the first crank pulse which starts the state machine. The arrival of the first cam pulse that follows begins the 'search' process which involves searching for a pattern of 12 consecutive crank pulses without interruption by a cam pulse. Once this pattern is found then the following cam pulse becomes the SYNC pulse (I.e. The 'First Paired' as mentioned above).<br />
</p>

<table border="1">
<tr>
<td align="center" valign="center">
<img 
    style="display: block; 
           margin-left: auto;
           margin-right: auto;
           width: 100%;"
    src="./images/StateMachine.png#center">
</img>
<p style="text-align: center;">Tooth Eater state machine. Both crank and cam signals are used inconjunction for state transitions.</p>
</table>


<p>
To facilitate this development, a simulated environment was developed under Simulide. The simulator was used to generate a digital representation of the crank and cam signals. In addition, the simulator was also able to directly execute the atTiny85 firmware.  The image shown below illustrates the following:
</p>

<table border="1">
<tr>
<td align="center" valign="center">
<img 
    style="display: block; 
           margin-left: auto;
           margin-right: auto;
           width: 100%;"
    src="./images/SimulatedSignals_1.png#center">
</img>

<p style="text-align: center;">Simulated signals (first two rows on CH1 & 2). Output processed signals from tooth eater (CH3, 4 & 5)</p>
</table>

<br />
Continuing, the following image highlights various markers in the signals.
<table border="1">
<tr>
<td align="center" valign="center">
<img 
    style="display: block; 
           margin-left: auto;
           margin-right: auto;
           width: 100%;"
    src="./images/SimulatedSignals_2.png#center">
</img>
</table>


<ol>
  <li>CH1 - The stream of crank pulses produced in the simulator. The highlighted section in orange displays the 'group of 12' consecutive pulses in sequence without a cam pulse interruption. The firmware receives this crank pulse stream as an input to its crank ISR.</li>
  <li>CH2 - The stream of simulated cam pulses in the simulator. This is the 3 spoke pattern with the pulses in their relative proportional spacing. The highlighted red pulses are the SYNC pulses (once in SYNC) and is also the 'First Paired' tooth. The purple highlighted pulse is the cam pulse that immediately succeeds the 'group of 12' crank pulses. This becomes a marker pulse to prepare the following cam pulse to become the SYNC pulse. The firmware receives this cam pulse stream as an input to its cam ISR.</li>
  <li>CH3 - Simply an output test line used to illuminate a LED for diagnostics.</li>
  <li>CH4 - The cam pulse output as a positive edge which is the cam SYNC pulse. This is highlighted in green and aligned to the 'First Paired' tooth. This cam pulse is produced by the tooth eater firmware and is what the downstream ECU 'sees' as a single cam pulse every engine cycle.</li>
  <li>CH5 - The crank 'enable' line. Once the state machine is SYNCED the crank pulses are allowed to simply 'pass-through' to the stand alone ECU. The firmware simply acts as a switch to allow the crank pulses to flow through once SYNCED.</li>
</ol> 

<p>
Once SYNCED, tooth eater acts as a simple counter and simply deletes the unwanted teeth in very simplistic terms. There is no run-time checking for skipped teeth. This is very important to note since if a tooth skip was to occur the ECU would SYNC at the wrong time as the engine is running and would misfire the engine. It is therefore <i><b>very important to configure the ECU</b></i> such that the cam SYNC is only required at engine startup so that it does not depend on the tooth eater once the engine has been started. Most ECU systems should be capable of managing the cam SYNC internally once the engine has been started. I can speak for RusEFI, having started the bike up and physically disconnected the cam wire from the tooth eater to RusEFU and it did not affect the running of the engine. This is due to RusEFI managing its own cam SYNC internally once the engine has been started.<br />
</p>

<p>
Furthering, once SYNCED the crank ISR is disabled to unburden the processor with unnecessary workload with its high pulse rate.<br />
</p>

<p>
There is an important caveat to the above operation. Let say the operator attempts to start the engine by holding the start button down (hence cranking it), however decides to release the start button before the engine got to fire up. If a subsequent engine restart were made, then the state machine logic would fail and yield false results since it would be in mid-flight from the first startup attempt. I.e. The state machine will be in an unknown state, unable to sync correctly. To solve this, we make use of the watch dog timer. If the scenario described above occurs, then the watch dog timer will reset the microcontroller and hence reset the state machine. This allows for a clean restart. The watch dog timer is invoked at the first crank pulse event with a time-out of 250mSecs. The watch dog is patted on every cam pulse there-after. So if the operator releases the start button after cranking (but not starting), the absence of cam pulse events will reboot the micro and restart the whole process cleanly. A time-out of 250mSecs is long enough to keep the watch dog satisfied with cam pulses at cranking speeds of 300 RPM, so there is no issue of the micro rebooting during cranking or normal engine running.<br />
</p>

<br /><br />
# Project

<table border="2">

<tr>
<td>
<p>
<u>The Tooth Eater project is comprised of 3 parts</u>:
<ol>
  <li>Firmware (source code)</li>
  <li>Simulation</li>
  <li>Hardware - Electronic schematics and PCB design</li>
</ol>

<br />
The firmware (1) and simulation (2) are found here. The hardware & PCB component (3) are found in the THT or SMD hardware repositories. Both hardware types use the same version of the firmware.
</p>
</td>
</tr>

<tr>

<td align="center" valign="center">
<img 
    style="display: block; 
           margin-left: auto;
           margin-right: auto;
           width: 50%;"
    src="./images/.directoryStructure.png#center">
</img>
<p>
Directory structure of the Tooth Eater project
</p>
</td>
</tr>

</table>

<br />

## 1. Firmware
<p>
The tooth eater firmware was written in C and developed for the ATMEL atTiny85 microcontroller using avr-gcc version 5.4.0 under linux. This small micro controller is more than adequate for the task, using less than 7% of its flash for program code storage and less than 1% of its ram. Upon startup, its internal clocking is set to 8Mhz along with peripheral setup. On power-up, it will simply wait for crank and cam pulse events to arrive to motion the state machine, finally starting the engine.
</p>

<p>
The firmware is licenced under GPLv3 and can be found under the 'src'. Official releases are found as .hex files under the 'releases' directory.
</p>

<br />


## 2. Simulation
<p>
The SimulIDE application (v1.1.0) was used to simulate both the crank and cam patterns as well as the atTiny85 firmware. This is a great tool in verifying correct operation before moving onto the hardware. The simulation file can be found under the 'simulation' directory. When this file is loaded into simulide, the firmware must also be loaded into the atTiny85 within the simulator.
</p>

<table border="1">
<tr>
<td align="center" valign="center">
<img 
    style="display: block; 
           margin-left: auto;
           margin-right: auto;
           width: 100%;"
    src="./images/Simulation.png#center">
</img>
<p style="text-align: left;">Tooth Eater in a simulated environment (SimulIDE). 
The section in red on the left is crank and cam signal generator and is driven by the clock as seen at the very left. The section on the right is the Tooth Eater taking in the signals from the generator and processing them. The simulated visual outputs are show in the scope and on the leds.<br /><br />
<u>Note</u>: In order for the simulator to work, the firmware 'hex' file must first be loaded into the virtual atTiny85 within the simulator by right clicking the processor and selecting 'Load Firmware', then selecting the .hex file. The firmware can be found under the 'Releases' directory. To start the simulation, click on the red power button at the top of the screen.<br />
<br />
If you have started the simulation correctly, your screen should look something like this.
<img 
    style="display: block; 
           margin-left: auto;
           margin-right: auto;
           width: 100%;"
    src="./images/SimulationAnimation.gif#center">
</img>
</p>
</tr>
</table>

<br />
## 3. Hardware - Electronic schematics and PCB design
<p>
Please see either
<strong><a href="https://github.com/BlackbirdStandalone/ToothEater-HW-THT" target="_blank">ToothEater-HW-THT</a></strong>
or
<strong><a href="https://github.com/BlackbirdStandalone/ToothEater-HW-SMD" target="_blank">ToothEater-HW-SMD</a></strong>

</p>
