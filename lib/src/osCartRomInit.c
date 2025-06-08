#include "PR/os_internal.h"
#include "PR/R4300.h"
#include "PR/rcp.h"
#include "PR/os_pi.h"
#include "PR/os.h"
#include "libultra_internal.h"
#include "piint.h"

OSPiHandle __Dom1SpeedParam;

OSPiHandle *osCartRomInit(void) {
    u32 domain;
    u32 saveMask;

    domain = 0;

    if (__Dom1SpeedParam.baseAddress == PHYS_TO_K1(PI_DOM1_ADDR2)) {
        return &__Dom1SpeedParam;
    }

    __Dom1SpeedParam.type = DEVICE_TYPE_CART;
    __Dom1SpeedParam.baseAddress = PHYS_TO_K1(PI_DOM1_ADDR2);
    osPiRawReadIo(0, &domain);
    __Dom1SpeedParam.latency = domain & 0xff;
    __Dom1SpeedParam.pulse = (domain >> 8) & 0xff;
    __Dom1SpeedParam.pageSize = (domain >> 0x10) & 0xf;
    __Dom1SpeedParam.relDuration = (domain >> 0x14) & 0xf;
    __Dom1SpeedParam.domain = PI_DOMAIN1;
    //__Dom1SpeedParam.speed = 0;

    bzero(&__Dom1SpeedParam.transferInfo, sizeof(__OSTranxInfo));

    saveMask = __osDisableInt();
    __Dom1SpeedParam.next = __osPiTable;
    __osPiTable = &__Dom1SpeedParam;
    __osRestoreInt(saveMask);

    return &__Dom1SpeedParam;
}
