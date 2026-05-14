/* -------------------------------------------------------------------------- */
/* Tooth Eater Firmware: v1.0                                                 */
/*                                                                            */
/* Copyright © 2026 Alex Kiaos                                                */
/*                                                                            */
/* Processor: atTiny85                                                        */
/* Licence:   GPLv3                                                           */
/* -------------------------------------------------------------------------- */

#ifndef MAIN_DEFS_H
#define MAIN_DEFS_H

#include "main_types.h"

/* -------------------------------------------------------------------------- */
/* Global Definitions                                                         */
/* -------------------------------------------------------------------------- */
#undef INVERT_CAM_OUTPUT


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
#define CRANK_OUT_ENABLE       PB4

#define TEST_LINE_OUT          PB3


/* -------------------------------------------------------------------------- */
/* Macros - Outputs                                                           */
/* -------------------------------------------------------------------------- */
#define CRANK_OUTPUT_OFF       PORTB &= ~(1 << CRANK_OUT_ENABLE);
#define CRANK_OUTPUT_ON        PORTB |= (1 << CRANK_OUT_ENABLE);

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
    CRANK_OUTPUT_OFF \
    TEST_LINE_ON \
    FLAGS |= BIT_HARD_LOCKUP;

#endif

