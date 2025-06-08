#include "libultra_internal.h"
#include "PR/R4300.h"
#include "piint.h"
#include "PR/rcp.h"
#include "PR/ique.h"
#include <macros.h>

typedef struct {
    u32 inst1;
    u32 inst2;
    u32 inst3;
    u32 inst4;
} __osExceptionVector;
extern __osExceptionVector __osExceptionPreamble;

u32 __osFinalrom; // maybe initialized?
u64 osClockRate = OS_CLOCK_RATE;

u32 __osShutdown = 0; // used in __osException

void osInitialize(void) {
    u32 pifdata;
    u32 clock = 0;

    UNUSED u32 ptr;

    __osFinalrom = TRUE;
    __osSetSR(__osGetSR() | SR_CU1);
    __osSetFpcCsr(FPCSR_FS | FPCSR_EV);
    while (__osSiRawReadIo(PIF_RAM_END - 3, &pifdata)) {
        ;
    }
    while (__osSiRawWriteIo(PIF_RAM_END - 3, pifdata | 8)) {
        ;
    }
    *(__osExceptionVector *) UT_VEC = __osExceptionPreamble;
    *(__osExceptionVector *) XUT_VEC = __osExceptionPreamble;
    *(__osExceptionVector *) ECC_VEC = __osExceptionPreamble;
    *(__osExceptionVector *) E_VEC = __osExceptionPreamble;
    osWritebackDCache((void *) UT_VEC, E_VEC - UT_VEC + sizeof(__osExceptionVector));
    osInvalICache((void *) UT_VEC, E_VEC - UT_VEC + sizeof(__osExceptionVector));
    osMapTLBRdb();
    osPiRawReadIo(4, &clock);
    clock &= ~0xf;
    if (clock) {
        osClockRate = clock;
    }
    osClockRate = osClockRate * 3 / 4;
    if (osResetType == RESET_TYPE_COLD_RESET) {
        bzero(osAppNmiBuffer, sizeof(osAppNmiBuffer));
    }
}
