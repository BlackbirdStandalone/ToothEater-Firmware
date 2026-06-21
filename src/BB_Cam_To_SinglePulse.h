/* -------------------------------------------------------------------------- */
/* Tooth Eater Firmware: v1.1                                                 */
/*                                                                            */
/* Copyright © 2026 Alex Kiaos                                                */
/*                                                                            */
/* Processor: atTiny85                                                        */
/* Licence:   GPLv3                                                           */
/* -------------------------------------------------------------------------- */

#ifndef BB_CAM_TO_SINGLE_PULSE_H
#define BB_CAM_TO_SINGLE_PULSE_H

#include "main_defs.h"

typedef enum
{
    UNINITIALISED = 0,
    CRANKING,
    WAIT_FOR_2,
    WAIT_FOR_12,
    SYNC_DELAY,
    SYNCED,
} CamEnum_t;

typedef struct
{
    vu8 state;
    vu8 nCrankModCount;
    vu8 nCamModCount;
    vu8 nTachoModCount;
    vu8 nIntervalCrankPulseCount;
    vu8 nDelayCrankAlignmentCount;
} Cam_Context_t;

extern Cam_Context_t cam;

extern void doCrankPulse(void);
extern void doCamPulse(void);
extern void initCamSinglePulse(void);

#endif

