/* -------------------------------------------------------------------------- */
/* Tooth Eater Firmware: v1.1                                                 */
/*                                                                            */
/* Copyright © 2026 Alex Kiaos                                                */
/*                                                                            */
/* Processor: atTiny85                                                        */
/* Licence:   GPLv3                                                           */
/* -------------------------------------------------------------------------- */

#include <avr/io.h>
#include <avr/interrupt.h>

#include "main_defs.h"
#include "BB_Cam_To_SinglePulse.h"

Cam_Context_t cam;

/* A single crank pulse invokes this ISR.                                     */
/*                                                                            */
/* This routine counts crank pulses during cranking to establish SYNC prior   */
/* to startup. Once SYNCED, this routine is no longer required and the ISR    */
/* becomes disabled by the state machine to unburden the processor during     */
/* normal engine running.                                                     */
void doCrankPulse(void)
{
    /* If the engine is running exit early                                    */
    if (SYNCED == cam.state)
    {
        return;
    }

    /* The very first crank pulse has arrived. We are now cranking.           */
    if (UNINITIALISED == cam.state)
    {
        cam.state = CRANKING;
        return;
    }

    /* The engine is cranking. Count the crank pulses between cam intervals   */
    ++cam.nIntervalCrankPulseCount;
}

/* A single cam pulse invokes this ISR.                                       */
/*                                                                            */
/* This routine is called every time a cam pulse arrives where it executes    */
/* the state machine once. The general operation of the state machine is as   */
/* follows:                                                                   */
/*                                                                            */
/*   UNINITIALISED:                                                           */
/*     The state machine will remain in this state until the very first crank */
/*     pulse arrives from the press of the engine start button. When the      */
/*     first crank pulse arrives, the state transitions to the 'CRANKING'     */
/*     state. The UNINITIALISED state is mainly to start the whole process.   */
/*                                                                            */
/*   CRANKING:                                                                */
/*     Now that the engine is spinning the watch dog is started on the next   */
/*     cam pulse. The watch dog is patted on every incoming cam pulse. This   */
/*     is necessary because the operator may decided to release their finger  */
/*     off the engine start button before the engine starts. Without the      */
/*     watchdog, the state machine would be in mid flight and would need to   */
/*     be reset for the next engine restart. The use of the watch dog allows  */
/*     the state machine to be restarted in such a scenario by reseting the   */
/*     micro if the cam pulses cease.                                         */
/*                                                                            */
/*   WAIT_FOR_2:                                                              */
/*     Wait for two consecutive crank pulses to arrive between two cam pulses.*/
/*     This location in the waveform is between the 'first' and 'second'      */
/*     paired cam teeth. This is the previous state before 12 consecutive     */
/*     crank pulses arrive. This state is a preliminary state to 'release'    */
/*     the crank pulses to the down stream ECU prior to receiving its first   */
/*     cam pulse (home position).                                             */
/*                                                                            */
/*   WAIT_FOR_12:                                                             */
/*     The number of crank pulses are counted between each cam pulse interval.*/
/*     The state machine waits until it receives 12 consecutive crank pulses  */
/*     so it can determine which cam tooth just crossed the sensor. This      */
/*     current tooth crossing is the 'Isolated' tooth and precedes the SYNC   */
/*     tooth by one cam tooth. Therefore the next cam tooth crossing is the   */
/*     SYNC tooth ('First Paired').                                           */
/*                                                                            */
/*   SYNCED:                                                                  */
/*     In the SYNCED state, the crank pulses are allowed to pass through via  */
/*     the VR conditioner in TTL as they come from the sensor. I.e. 12 pulses */
/*     per crank revolution.  The cam pulses are only strobed through if they */
/*     are the SYNC pulse (First paired) and inhibited otherwise. So one of   */
/*     the cam pulses (SYNC pulse only) is allowed through and the remaining  */
/*     two pulses are deasserted.                                             */
void doCamPulse(void)
{
    switch(cam.state)
    {
    case UNINITIALISED:
        cam.state = CRANKING;
        break;

    case CRANKING:
        ENABLE_WDG
        cam.state = WAIT_FOR_2;
        break;

    case WAIT_FOR_2:
        if (2 == cam.nIntervalCrankPulseCount)
        {
            cam.state = WAIT_FOR_12;
            CRANK_OUTPUT_ON
        }
        break;

    case WAIT_FOR_12:
        if (12 == cam.nIntervalCrankPulseCount)
        {
            cam.state = SYNCED;

            /* Disable this cam ISR here. The crank ISR will take over now.   */
            GIMSK &= ~(1 << INT0);
        }
        break;

    case SYNCED:
        break;

    default:
        /* Something has gone wrong. Reset the state machine by resetting the */
        /* entire microcontroller - Force the watchdog to kick in             */
        cam.state = UNINITIALISED;
        HARD_LOCKUP_FAULT
        break;
    }

    /* Reset the crank interval counter between cam pulses                    */
    cam.nIntervalCrankPulseCount = 0;
}

void initCamSinglePulse(void)
{
    FLAGS = 0x00;

    cam.state = UNINITIALISED;
    cam.nCrankModCount = 0;
    cam.nCamModCount = 0;
    cam.nTachoModCount = 0;
    cam.nIntervalCrankPulseCount = 0;
}


