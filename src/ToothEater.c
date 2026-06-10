/* -------------------------------------------------------------------------- */
/* Tooth Eater Firmware: v1.1                                                 */
/*                                                                            */
/* Copyright © 2026 Alex Kiaos                                                */
/*                                                                            */
/* Processor: atTiny85                                                        */
/* Licence:   GPLv3                                                           */
/* -------------------------------------------------------------------------- */

#include <avr/interrupt.h>

#include "main_defs.h"
#include "BB_Cam_To_SinglePulse.h"

/* Crank ISR                                                                  */
/*                                                                            */
/* This interrupt service routine fires on every positive crank edge during   */
/* engine startup. Once the engine has been started, the interrupt line is    */
/* disabled by the state machine once it is in the SYNC state. This is to     */
/* prevent unnecessarily overloading the processor with crank interrupts      */
/* during engine running.                                                     */
ISR (PCINT0_vect)
{
    if (PINB & (1 << CRANK_TRIG_IN))
    {
        doCrankPulse();
        TEST_LINE_ON
    }
    else
    {
        TEST_LINE_OFF
    }
}

/* Cam ISR                                                                    */
/*                                                                            */
/* This interrupt service routine fires on every falling cam edge. This is    */
/* due to the inverted cam pulse from the NCV1124 VR conditioner chip. The    */
/* state machine is driven by 'doCamPulse()' and the watchdog is always       */
/* patted once the first cam pulse has passed.                                */
/*   If a HARD LOCKUP is invoked, then this ISR is intentionally jammed to    */
/* invoke the WDG and reset the microcontroller.                              */
ISR (INT0_vect)
{
    if (FLAGS & BIT_HARD_LOCKUP)
    {
        while(1) {}
    }

    doCamPulse();

    /* Pat the watch dog, but only if past the 'UNINITIALISED' state, since   */
    /* the engine is now cranking or running                                  */
    if (cam.state > UNINITIALISED)
    {
        WDG_PAT();
    }
}

/* Configure cam ISR pin                                                      */
/* Enable the external interrupt line INT0                                    */
static void externalInterrupt(void)
{
    GIMSK |= (1 << INT0);

#ifdef INVERT_CAM_INPUT
    /* Fire cam ISR on falling edge                                           */
    MCUCR &= ~(1 << ISC00);
#else
    /* Fire cam ISR on rising edge                                            */
    MCUCR |= (1 << ISC00);
#endif

    MCUCR |= (1 << ISC01);
}

/* Configure crank ISR pin                                                    */
/* Listen for a bit polarity change on PB1, invoking 'PCINT0_vect' if so      */
static void pinChangeInterrupt(void)
{
    GIMSK |= (1 << PCIE);
    PCMSK |= (1 << PCINT1);
}

/* The AVR clock default is 1Mhz. Enforce 8Mhz operation                      */
static void setupClockRate8Mhz(void)
{
    CLKPR = 0x80;
    CLKPR = 0x00;
}

/* Setup watchdog timeout to 0.250 secs (250 mSecs).                          */
/* Start out with the watch dog disabled.                                     */
static void setupWatchDog(void)
{
    DISABLE_WDG
    WDTCR = (1 << WDP2);
}

/* Pin setup:                                                                 */
/*                                                                            */
/* INPUTS:  PB2: CAM (input ISR trigger)                                      */
/*          PB1: CRANK (input ISR trigger)                                    */
/*                                                                            */
/* OUTPUTS: PB0: CAM (processed output to ECU)                                */
/*          PB4: CRANK enable pass-through (output to ECU)                    */
/*          PB3: Test line - diagnosis                                        */
/*                                                                            */
static void setupPins(void)
{
    /* Cam input port on PB2 (ISR). Hi-Z (deactivated internal pull-up)       */
    DDRB &= ~(1 << CAM_TRIG_IN);
    PORTB &= ~(1 << CAM_TRIG_IN);

    /* Crank input port on PB1 (ISR). Hi-Z (deactivate internal pull-up)      */
    DDRB &= ~(1 << CRANK_TRIG_IN);
    PORTB &= ~(1 << CRANK_TRIG_IN);

    /* Setup output ports                                                     */
    DDRB |= (1 << CAM_OUT_ENABLE);
    DDRB |= (1 << CRANK_OUT_ENABLE);
    DDRB |= (1 << TEST_LINE_OUT);

    /* On start-up, ensure both CAM and CRANK output enable lines are OFF     */
    CAM_OUTPUT_OFF
    CRANK_OUTPUT_OFF
    TEST_LINE_OFF
}

/* The entire system is interrupt driven. main() is only used to setup the    */
/* micro on startup. Then loop continuously unless a hard lock up is invoked  */
int main(void)
{
    initCamSinglePulse();

    cli();

    setupClockRate8Mhz();
    setupWatchDog();
    externalInterrupt();
    pinChangeInterrupt();
    setupPins();

    sei();

    while (!(FLAGS & BIT_HARD_LOCKUP))
    {
    }
}


