#include "libultra_internal.h"
#include "osContInternal.h"
#include "PR/ique.h"
#include <macros.h>
#include "controller.h"

ALIGNED8 OSPifRam __osContPifRam;

extern u8 __osContLastCmd;
extern u8 __osMaxControllers;

void __osPackReadData(void);

s32 osContStartReadData(OSMesgQueue *mesg) {
    s32 ret = 0;
    s32 i;
    __osSiGetAccess();
    if (__osContLastCmd != CONT_CMD_READ_BUTTON) {
        __osPackReadData();
        ret = __osSiRawStartDma(OS_WRITE, __osContPifRam.ramarray);
        osRecvMesg(mesg, NULL, OS_MESG_BLOCK);
    }
    for (i = 0; i < ARRAY_COUNT(__osContPifRam.ramarray) + 1; i++) {
        __osContPifRam.ramarray[i] = 0xff;
    }
    __osContPifRam.pifstatus = 0;

    ret = __osSiRawStartDma(OS_READ, __osContPifRam.ramarray);
    __osContLastCmd = CONT_CMD_READ_BUTTON;
    __osSiRelAccess();
    return ret;
}

void osContGetReadData(OSContPad *pad) {
    u8 *cmdBufPtr;
    OSContPackedRead response;
    s32 i;
    cmdBufPtr = (u8 *) __osContPifRam.ramarray;
    for (i = 0; i < __osMaxControllers; i++, cmdBufPtr += sizeof(OSContPackedRead), pad++) {
        response = *(OSContPackedRead *) cmdBufPtr;
        pad->errnum = (response.rxLen & 0xc0) >> 4;
        if (pad->errnum == 0) {
            pad->button = response.button;
            pad->stick_x = response.rawStickX;
            pad->stick_y = response.rawStickY;
        }
    }
}

void __osPackReadData() {
    u8 *cmdBufPtr;
    OSContPackedRead request;
    s32 i;
    cmdBufPtr = (u8 *) __osContPifRam.ramarray;

    for (i = 0; i < ARRAY_COUNT(__osContPifRam.ramarray) + 1; i++) {
        __osContPifRam.ramarray[i] = 0;
    }

    __osContPifRam.pifstatus = 1;
    request.padOrEnd = 255;
    request.txLen = 1;
    request.rxLen = 4;
    request.command = 1;
    request.button = 65535;
    request.rawStickX = -1;
    request.rawStickY = -1;
    for (i = 0; i < __osMaxControllers; i++) {
        *(OSContPackedRead *) cmdBufPtr = request;
        cmdBufPtr += sizeof(OSContPackedRead);
    }
    *cmdBufPtr = 254;
}
