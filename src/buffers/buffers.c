#include <ultra64.h>

#include "buffers.h"

ALIGNED8 u8 gDecompressionHeap[0xD000];

ALIGNED16 u8 gAudioHeap[DOUBLE_SIZE_ON_64_BIT(0x31200)];

ALIGNED8 u8 gIdleThreadStack[IDLE_STACKSIZE];
ALIGNED8 u8 gThread3Stack[STACKSIZE];
ALIGNED8 u8 gThread4Stack[STACKSIZE];
ALIGNED8 u8 gThread5Stack[STACKSIZE];
// 0x400 bytes
ALIGNED8 u8 gGfxSPTaskStack[SP_DRAM_STACK_SIZE8];
// 0xc00 bytes for f3dex, 0x900 otherwise
ALIGNED8 u8 gGfxSPTaskYieldBuffer[OS_YIELD_DATA_SIZE];
// 0x200 bytes
ALIGNED8 struct SaveBuffer gSaveBuffer;
// 0x190a0 bytes
struct GfxPool gGfxPools[2];

// Yield buffer for audio, 0x400 bytes. Stubbed out post-JP since the audio
// task never yields.
ALIGNED8 u8 gAudioSPTaskYieldBuffer[OS_YIELD_AUDIO_SIZE];

// Probably Thread 2 stack space. Unreferenced, and stubbed out with f3dex to
// avoid an overflowing .buffers segment.
ALIGNED8 u8 gUnusedThread2Stack[UNUSED_STACKSIZE];
