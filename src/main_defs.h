/* -------------------------------------------------------------------------- */
/* Tooth Eater Firmware: v1.1                                                 */
/*                                                                            */
/* Copyright © 2026 Alex Kiaos                                                */
/*                                                                            */
/* Processor: atTiny85                                                        */
/* Licence:   GPLv3                                                           */
/* -------------------------------------------------------------------------- */

#ifndef MAIN_DEFS_H
#define MAIN_DEFS_H

typedef volatile unsigned short vu16;
typedef volatile unsigned char  vu8;

/* -------------------------------------------------------------------------- */
/* Global Definitions                                                         */
/* -------------------------------------------------------------------------- */
/*                                                                            */

/* Crank pulses are direct pass-through from VR to ECU.                       */
/* Cam pulses are used from the VR to initially SYNC, then are ignored by the */
/* VR. The Tooth eater will insert its own cam pulse digitally on every 24th  */
/* crank pulse. The width of the inserted cam pulse is 1 crank pulse period.  */
#define CAM_PULSE_FASTER_STARTUP

/* Crank ISR polarity.                                                        */
#undef INVERT_CRANK_INPUT

/* Cam ISR polarity.                                                          */
/* Depending on the output of the VR chip that feeds into this tooth eater,   */
/* you may want to fire the cam ISR either on the falling or rising edge.     */
/*   Eg. The NCV1124 VR chip inverts its TTL output. So we can fire the ISR   */
/* on the falling edge and correctly align to the 'first paired'.             */
#define INVERT_CAM_INPUT

/* Define to activate ISR input side pull-ups. Undefine to deactivate (Hi-Z). */
/* Default is deactivated.                                                    */
/* Note: The output TTL side of the VR drives these inputs.                   */
#undef ACTIVATE_INTERNAL_PULLUPS

/* Default is non-inverted output.                                            */
#undef INVERT_CAM_OUTPUT

/* Number of crank pulses per engine cycle.                                   */
/* The crank trigger wheel has 12T, so double this.                           */
#define CKP_PER_ENGINE_CYCLE   24

/* The number of crank pulses between the fast start SYNC point and the       */
/* first paired tooth. This is for testing alignment only & not needed in     */
/* fast start mode. This is more of a test feature and is used when           */
/* CAM_PULSE_FASTER_STARTUP is undefined.                                     */
#define DELAY_CRK_ALIGN_COUNT  8

/* Tacho output                                                               */
#define TACHO_CYCLES           (CKP_PER_ENGINE_CYCLE / 4)
#define TACHO_HALF_PERIOD      (TACHO_CYCLES / 2)


/* -------------------------------------------------------------------------- */
/* Custom Flag bits (single byte only)                                        */
/* -------------------------------------------------------------------------- */
#define BIT_0                  (0x01)
#define BIT_1                  (0x02)
#define BIT_2                  (0x04)
#define BIT_3                  (0x08)
#define BIT_4                  (0x10)
#define BIT_5                  (0x20)
#define BIT_6                  (0x40)
#define BIT_7                  (0x80)

#define BIT_SPARE_0            (BIT_0)
#define BIT_SPARE_1            (BIT_1)
#define BIT_SPARE_2            (BIT_2)
#define BIT_SPARE_3            (BIT_3)
#define BIT_SPARE_4            (BIT_4)
#define BIT_SPARE_5            (BIT_5)
#define BIT_SPARE_6            (BIT_6)
#define BIT_HARD_LOCKUP        (BIT_7)


/* -------------------------------------------------------------------------- */
/* Testing/Instrumentation - Debug memory locations                           */
/* -------------------------------------------------------------------------- */

/* Used i.c.w testing. Stores a vu16 in addresses 0x100 & 0x101               */
#define FLAGS                 (*(vu8 *)0x0008000A0)


/* -------------------------------------------------------------------------- */
/* Pin Definitions                                                            */
/* -------------------------------------------------------------------------- */

/* Input pins                                                                 */
#define CAM_TRIG_IN            PB2
#define CRANK_TRIG_IN          PB1

/* Output pins                                                                */
#define CAM_OUT_ENABLE         PB0
#define TACHO_OUT              PB4

#define TEST_LINE_OUT          PB3


/* -------------------------------------------------------------------------- */
/* Macros - Outputs                                                           */
/* -------------------------------------------------------------------------- */
#define TACHO_OFF              PORTB &= ~(1 << TACHO_OUT);
#define TACHO_ON               PORTB |= (1 << TACHO_OUT);
#define TACHO_TOGGLE           PORTB ^= (1 << TACHO_OUT);

#ifdef INVERT_CAM_OUTPUT
#define CAM_OUTPUT_OFF         PORTB |= (1 << CAM_OUT_ENABLE);
#define CAM_OUTPUT_ON          PORTB &= ~(1 << CAM_OUT_ENABLE);
#else
#define CAM_OUTPUT_OFF         PORTB &= ~(1 << CAM_OUT_ENABLE);
#define CAM_OUTPUT_ON          PORTB |= (1 << CAM_OUT_ENABLE);
#endif

#define TEST_LINE_ON           PORTB |= (1 << TEST_LINE_OUT);
#define TEST_LINE_OFF          PORTB &= ~(1 << TEST_LINE_OUT);
#define TEST_LINE_TOGGLE       PORTB ^= (1 << TEST_LINE_OUT);


/* -------------------------------------------------------------------------- */
/* Macros - Watchdog                                                          */
/* -------------------------------------------------------------------------- */

/* Disable the WDG                                                            */
#define DISABLE_WDG \
    MCUSR = 0; \
    WDTCR |= (1 << WDCE) | (1 << WDE); \
    WDTCR = 0;

/* Enable the WDG                                                             */
#define ENABLE_WDG \
    WDTCR |= (1 << WDCE) | (1 << WDE);

/* Pat the WDG                                                                */
#define WDG_PAT() \
    __asm__ __volatile__ ("wdr")


/* -------------------------------------------------------------------------- */
/* Hard micro-controller lock up - Fault Reset                                */
/* Call this to invoke a micro reset                                          */
/* -------------------------------------------------------------------------- */
#define HARD_LOCKUP_FAULT \
    CAM_OUTPUT_OFF \
    TACHO_OFF \
    TEST_LINE_ON \
    FLAGS |= BIT_HARD_LOCKUP;

#endif

