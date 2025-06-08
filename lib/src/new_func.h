#ifndef NEW_FUNC_H
#define NEW_FUNC_H

#include "libultra_internal.h"

extern u32 osDDActive;

void __osResetGlobalIntMask(u32 mask);
void osYieldThread(void);

#endif
