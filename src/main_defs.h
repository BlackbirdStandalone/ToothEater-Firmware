/* -------------------------------------------------------------------------- */
/* Tooth Eater Firmware: v1.2                                                 */
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

/* Strategy selection                                                         */
#define CAM_PULSES_ON_STARTUP_ONLY
#define CAM_PULSES_ALLOWED     50

/* Crank ISR polarity.                                                        */
#undef INVERT_CRANK_INPUT

/* Cam ISR polarity.                                                          */
/* Depending on the output of the VR chip that feeds into this tooth eater,   */
/* you may want to fire the cam ISR either on the falling or rising edge.     */
/*   Eg. The NCV1124 VR chip inverts its TTL output. So we can fire the ISR   */
/* on the falling edge.                                                       */
#define INVERT_CAM_INPUT

/* Define to activate ISR input side pull-ups. Undefine to deactivate (Hi-Z). */
/* Default is Activated.                                                      */
/* Note: The output TTL side of the VR drives these inputs.                   */
#define ACTIVATE_INTERNAL_PULLUPS

/* Default is non-inverted output.                                            */
#undef INVERT_CAM_OUTPUT

/* Number of crank pulses per engine cycle.                                   */
/* The crank trigger wheel has 12T, so double this.                           */
#define CKP_PER_ENGINE_CYCLE   24

/* Tacho output                                                               */
/* The tacho is enabled by default. If its disabled, the tacho pin serves as  */
/* a test pin to measure the CPU utilisation load.                            */
#define TACHO_ENABLE

#define TACHO_CYCLES           (CKP_PER_ENGINE_CYCLE / 4)
#define TACHO_HALF_PERIOD      (TACHO_CYCLES / 2)


/* -------------------------------------------------------------------------- */
/* Custom Flag bits (16-bit)                                                  */
/* -------------------------------------------------------------------------- */
#define BIT_0                  (0x0001)
#define BIT_1                  (0x0002)
#define BIT_2                  (0x0004)
#define BIT_3                  (0x0008)
#define BIT_4                  (0x0010)
#define BIT_5                  (0x0020)
#define BIT_6                  (0x0040)
#define BIT_7                  (0x0080)
#define BIT_8                  (0x0100)
#define BIT_9                  (0x0200)
#define BIT_10                 (0x0400)
#define BIT_11                 (0x0800)
#define BIT_12                 (0x1000)
#define BIT_13                 (0x2000)
#define BIT_14                 (0x4000)
#define BIT_15                 (0x8000)

#define BIT_SPARE_0            (BIT_0)
#define BIT_SPARE_1            (BIT_1)
#define BIT_SPARE_2            (BIT_2)
#define BIT_SPARE_3            (BIT_3)
#define BIT_SPARE_4            (BIT_4)
#define BIT_SPARE_5            (BIT_5)
#define BIT_SPARE_6            (BIT_6)
#define BIT_SPARE_7            (BIT_7)
#define BIT_SPARE_8            (BIT_8)
#define BIT_SPARE_9            (BIT_9)
#define BIT_SPARE_10           (BIT_10)
#define BIT_SPARE_11           (BIT_11)
#define BIT_SPARE_12           (BIT_12)
#define BIT_SPARE_13           (BIT_13)
#define BIT_SPARE_14           (BIT_14)
#define BIT_HARD_LOCKUP        (BIT_15)


/* -------------------------------------------------------------------------- */
/* Testing/Instrumentation - Debug memory locations                           */
/* -------------------------------------------------------------------------- */

/* Used i.c.w testing. Stores a vu16 in addresses 0x1A0 & 0x1A1               */
#define FLAGS                  (*(vu16 *)0x01A0)


/* -------------------------------------------------------------------------- */
/* Pin Definitions                                                            */
/* -------------------------------------------------------------------------- */

/* Input pins                                                                 */
#define CAM_TRIG_IN            PB2
#define CRANK_TRIG_IN          PB1

/* Output pins                                                                */
#define CRANK_OUT_ENABLE       PB4
#define CAM_OUT_ENABLE         PB0
#define TACHO_OUT              PB3


/* -------------------------------------------------------------------------- */
/* Macros - Outputs                                                           */
/* -------------------------------------------------------------------------- */
#define CRANK_OUTPUT_ON        PORTB |= (1 << CRANK_OUT_ENABLE);
#define CRANK_OUTPUT_OFF       PORTB &= ~(1 << CRANK_OUT_ENABLE);

#ifdef INVERT_CAM_OUTPUT
#define CAM_OUTPUT_OFF         PORTB |= (1 << CAM_OUT_ENABLE);
#define CAM_OUTPUT_ON          PORTB &= ~(1 << CAM_OUT_ENABLE);
#else
#define CAM_OUTPUT_OFF         PORTB &= ~(1 << CAM_OUT_ENABLE);
#define CAM_OUTPUT_ON          PORTB |= (1 << CAM_OUT_ENABLE);
#endif

#define TACHO_OFF              PORTB &= ~(1 << TACHO_OUT);
#define TACHO_ON               PORTB |= (1 << TACHO_OUT);
#define TACHO_TOGGLE           PORTB ^= (1 << TACHO_OUT);

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
    TACHO_OFF \
    FLAGS |= BIT_HARD_LOCKUP;

#endif

