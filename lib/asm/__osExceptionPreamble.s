.set noat      // allow manual use of $at
.set noreorder // don't insert nops after branches

#include "macros.inc"

#include <PR/R4300.h>
#include <PR/rcp.h>
#include <PR/ique.h>

.section .text, "ax"

#ifdef AVOID_UB
.set __osThreadTail, __osThreadTail_fix
#endif

glabel __osExceptionPreamble
    la    $k0, __osException
    jr    $k0
     nop

glabel __osException
    la    $k0, __osThreadSave
    sd    $at, 0x20($k0)
    mfc0  $k1, $12
    sw    $k1, 0x118($k0)
    li    $at, -4
    and   $k1, $k1, $at
    mtc0  $k1, $12
    sd    $t0, 0x58($k0)
    sd    $t1, 0x60($k0)
    sd    $t2, 0x68($k0)
    sw    $zero, 0x18($k0)
    mfc0  $t0, $13
    andi  $t1, $t0, 0x7c
    li    $t2, 0
    bne   $t1, $t2, .L80326750
     nop
    and   $t1, $k1, $t0
    andi  $t2, $t1, 0x4000
    beqz  $t2, .L80326734
     nop
    li    $t1, 1
    lui   $at, %hi(D_80334934)
    b     .L80326794
     sw    $t1, %lo(D_80334934)($at)
.L80326734:
    andi  $t2, $t1, 0x2000
    beqz  $t2, .L80326750
     nop
    li    $t1, 1
    lui   $at, %hi(D_80334938)
    b     .L80326794
     sw    $t1, %lo(D_80334938)($at)
.L80326750:
    lui   $at, %hi(D_80334934)
    sw    $zero, %lo(D_80334934)($at)
    lui   $at, %hi(D_80334938)
    move  $t0, $k0
    sw    $zero, %lo(D_80334938)($at)
    lui   $k0, %hi(__osThreadTail + 0x10)
    lw    $k0, %lo(__osThreadTail + 0x10)($k0)
    ld    $t1, 0x20($t0)
    sd    $t1, 0x20($k0)
    ld    $t1, 0x118($t0)
    sd    $t1, 0x118($k0)
    ld    $t1, 0x58($t0)
    sd    $t1, 0x58($k0)
    ld    $t1, 0x60($t0)
    sd    $t1, 0x60($k0)
    ld    $t1, 0x68($t0)
    sd    $t1, 0x68($k0)
.L80326794:
    mflo  $t0
    sd    $t0, 0x108($k0)
    mfhi  $t0
    sd    $v0, 0x28($k0)
    sd    $v1, 0x30($k0)
    sd    $a0, 0x38($k0)
    sd    $a1, 0x40($k0)
    sd    $a2, 0x48($k0)
    sd    $a3, 0x50($k0)
    sd    $t3, 0x70($k0)
    sd    $t4, 0x78($k0)
    sd    $t5, 0x80($k0)
    sd    $t6, 0x88($k0)
    sd    $t7, 0x90($k0)
    sd    $s0, 0x98($k0)
    sd    $s1, 0xa0($k0)
    sd    $s2, 0xa8($k0)
    sd    $s3, 0xb0($k0)
    sd    $s4, 0xb8($k0)
    sd    $s5, 0xc0($k0)
    sd    $s6, 0xc8($k0)
    sd    $s7, 0xd0($k0)
    sd    $t8, 0xd8($k0)
    sd    $t9, 0xe0($k0)
    sd    $gp, 0xe8($k0)
    sd    $sp, 0xf0($k0)
    sd    $fp, 0xf8($k0)
    sd    $ra, 0x100($k0)
    sd    $t0, 0x110($k0)
    mfc0  $t0, C0_EPC
    sw    $t0, 0x11c($k0)
    lw    $t0, 0x18($k0)
    beqz  $t0, .L80326868
     nop
    cfc1  $t0, $31
    nop
    sw    $t0, 0x12c($k0)
    sdc1  $f0, 0x130($k0)
    sdc1  $f2, 0x138($k0)
    sdc1  $f4, 0x140($k0)
    sdc1  $f6, 0x148($k0)
    sdc1  $f8, 0x150($k0)
    sdc1  $f10, 0x158($k0)
    sdc1  $f12, 0x160($k0)
    sdc1  $f14, 0x168($k0)
    sdc1  $f16, 0x170($k0)
    sdc1  $f18, 0x178($k0)
    sdc1  $f20, 0x180($k0)
    sdc1  $f22, 0x188($k0)
    sdc1  $f24, 0x190($k0)
    sdc1  $f26, 0x198($k0)
    sdc1  $f28, 0x1a0($k0)
    sdc1  $f30, 0x1a8($k0)
.L80326868:
    mfc0  $t0, C0_CAUSE
    sw    $t0, 0x120($k0)
    lui   $t1, %hi(PHYS_TO_K1(MI_INTR_MASK_REG))
    lw    $t1, %lo(PHYS_TO_K1(MI_INTR_MASK_REG))($t1)
    sw    $t1, 0x128($k0)
    li    $t1, 2
    sh    $t1, 0x10($k0)
    lui   $t1, %hi(D_80334934)
    lw    $t1, %lo(D_80334934)($t1)
    beqz  $t1, .L803268B4
     nop
    lui   $t2, %hi(D_C0000008)
    sw    $zero, %lo(D_C0000008)($t2)
    lui   $a0, %hi(D_C0000000)
    addiu $t2, %lo(D_C0000008)
    jal   kdebugserver
     lw    $a0, %lo(D_C0000000)($a0)
    b     .L80326E08
     nop
.L803268B4:
    lui   $t1, %hi(D_80334938)
    lw    $t1, %lo(D_80334938)($t1)
    beqz  $t1, .L80326900
     nop
    lui   $t2, %hi(D_C000000C)
    sw    $zero, %lo(D_C000000C)($t2)
    lui   $t1, %hi(__osRdbSendMessage)
    lw    $t1, %lo(__osRdbSendMessage)($t1)
    addiu $t2, %lo(D_C000000C)
    beqz  $t1, .L803268E8
     nop
    jal   send_mesg
     li    $a0, 120
.L803268E8:
    lui   $t1, %hi(__osRdbWriteOK)
    lw    $t1, %lo(__osRdbWriteOK)($t1)
    lui   $at, %hi(__osRdbWriteOK)
    addi  $t1, $t1, 1
    b     .L80326E08
     sw    $t1, %lo(__osRdbWriteOK)($at)
.L80326900:
    andi  $t1, $t0, CAUSE_EXCMASK
    li    $t2, EXC_BREAK
    beq   $t1, $t2, handle_break
     nop
    li    $t2, EXC_CPU
    beq   $t1, $t2, handle_CpU
     nop
    li    $t2, EXC_INT
    bne   $t1, $t2, panic
     nop
    and   $s0, $k1, $t0
next_interrupt:
    andi  $t1, $s0, SR_IMASK
    srl   $t2, $t1, 0xc
    bnez  $t2, .L80326944
     nop
    srl   $t2, $t1, 8
    addi  $t2, $t2, 0x10
.L80326944:
    // TODO: Get rid of noat
.set at
    lbu   $t2, __osIntOffTable($t2)
    lw    $t2, __osIntTable($t2)
.set noat
    jr    $t2
     nop
glabel counter
    mfc0  $t1, C0_COMPARE
    mtc0  $t1, C0_COMPARE
    jal   send_mesg
     li    $a0, 24
    lui   $at, (~CAUSE_IP8 >> 16) & 0xFFFF
    ori   $at, (~CAUSE_IP8 & 0xFFFF)
    b     next_interrupt
     and   $s0, $s0, $at

glabel cart
    li    $t2, 4
    lui   $at, %hi(__osHwIntTable)
    addu  $at, $at, $t2
    lw    $t2, %lo(__osHwIntTable)($at)
    beqz  $t2, .L803269A4
     nop
    jalr  $t2
     nop
.L803269A4:
    jal   send_mesg
     li    $a0, 16
    li    $at, -2049
    b     next_interrupt
     and   $s0, $s0, $at
glabel rcp
    lui   $s1, %hi(PHYS_TO_K1(MI_INTR_REG))
    lw    $s1, %lo(PHYS_TO_K1(MI_INTR_REG))($s1)
    andi  $s1, $s1, 0x3f
    andi  $t1, $s1, MI_INTR_SP
    beqz  $t1, vi
     nop
    lui   $t4, %hi(PHYS_TO_K1(SP_STATUS_REG))
    lw    $t4, %lo(PHYS_TO_K1(SP_STATUS_REG))($t4)
    li    $t1, SP_CLR_INTR
    lui   $at, %hi(PHYS_TO_K1(SP_STATUS_REG))
    andi  $t4, $t4, 0x300
    andi  $s1, $s1, 0x3e
    beqz  $t4, sp_other_break
     sw    $t1, %lo(PHYS_TO_K1(SP_STATUS_REG))($at)
    jal   send_mesg
     li    $a0, 32
    beqz  $s1, no_more_rcp_ints
     nop
    b     vi
     nop
sp_other_break:
    jal   send_mesg
     li    $a0, 88
    beqz  $s1, no_more_rcp_ints
     nop
vi:
    andi  $t1, $s1, 8
    beqz  $t1, ai
     lui   $at, %hi(PHYS_TO_K1(VI_CURRENT_REG))
    andi  $s1, $s1, 0x37
    sw    $zero, %lo(PHYS_TO_K1(VI_CURRENT_REG))($at)
    jal   send_mesg
     li    $a0, 56
    beqz  $s1, no_more_rcp_ints
     nop
ai:
    andi  $t1, $s1, 4
    beqz  $t1, si
     nop
    li    $t1, 1
    lui   $at, %hi(PHYS_TO_K1(AI_STATUS_REG))
    andi  $s1, $s1, 0x3b
    sw    $t1, %lo(PHYS_TO_K1(AI_STATUS_REG))($at)
    jal   send_mesg
     li    $a0, 48
    beqz  $s1, no_more_rcp_ints
     nop
si:
    andi  $t1, $s1, 2
    beqz  $t1, pi
     lui   $at, %hi(PHYS_TO_K1(SI_STATUS_REG))
    andi  $s1, $s1, 0x3d
    sw    $zero, %lo(PHYS_TO_K1(SI_STATUS_REG))($at)
    jal   send_mesg
     li    $a0, 40
    beqz  $s1, no_more_rcp_ints
     nop
pi:
    andi  $t1, $s1, 0x10
    beqz  $t1, dp
     nop
    li    $t1, 2
    lui   $at, %hi(PHYS_TO_K1(PI_STATUS_REG))
    andi  $s1, $s1, 0x2f
    sw    $t1, %lo(PHYS_TO_K1(PI_STATUS_REG))($at)
    jal   send_mesg
     li    $a0, 64
.L803076C8:
    beqz  $s1, no_more_rcp_ints
     nop
dp:
    andi  $t1, $s1, 0x20
    beqz  $t1, no_more_rcp_ints
     nop
    li    $t1, MI_CLR_DP_INTR
    lui   $at, %hi(PHYS_TO_K1(MI_MODE_REG))
    andi  $s1, $s1, 0x1f
    sw    $t1, %lo(PHYS_TO_K1(MI_MODE_REG))($at)
    jal   send_mesg
     li    $a0, 72
no_more_rcp_ints:
    li    $at, -1025
    b     next_interrupt
     and   $s0, $s0, $at
glabel prenmi
    lw    $k1, 0x118($k0)
    li    $at, -4097
    lui   $t1, %hi(__osShutdown)
    and   $k1, $k1, $at
    sw    $k1, 0x118($k0)
    addiu $t1, %lo(__osShutdown)
    lw    $t2, ($t1)
    beqz  $t2, firstnmi
     li    $at, -4097
    b     redispatch
     and   $s0, $s0, $at
firstnmi:
    li    $t2, 1
    sw    $t2, ($t1)
    jal   send_mesg
     li    $a0, 112
    lui   $t2, %hi(__osThreadTail + 0x8)
    lw    $t2, %lo(__osThreadTail + 0x8)($t2)
    li    $at, -4097
    and   $s0, $s0, $at
    lw    $k1, 0x118($t2)
    and   $k1, $k1, $at
    b     redispatch
     sw    $k1, 0x118($t2)
glabel sw2
    li    $at, -513
    and   $t0, $t0, $at
    mtc0  $t0, $13
    jal   send_mesg
     li    $a0, 8
    li    $at, -513
    b     next_interrupt
     and   $s0, $s0, $at
glabel sw1
    li    $at, -257
    and   $t0, $t0, $at
    mtc0  $t0, $13
    jal   send_mesg
     li    $a0, 0
    li    $at, -257
    b     next_interrupt
     and   $s0, $s0, $at
handle_break:
    li    $t1, 1
    sh    $t1, 0x12($k0)
    jal   send_mesg
     li    $a0, 80
    b     redispatch
     nop

glabel redispatch
    lui   $t2, %hi(__osThreadTail + 0x8)
    lw    $t2, %lo(__osThreadTail + 0x8)($t2)
    lw    $t1, 4($k0)
    lw    $t3, 4($t2)
    slt   $at, $t1, $t3
    beqz  $at, enqueue_running
     nop
    lui   $a0, %hi(__osThreadTail + 0x8)
    move  $a1, $k0
    jal   __osEnqueueThread
     addiu $a0, %lo(__osThreadTail + 0x8)
    j     __osDispatchThread
     nop

enqueue_running:
    la    $t1, __osThreadTail + 0x8
    lw    $t2, ($t1)
    sw    $t2, ($k0)
    j     __osDispatchThread
     sw    $k0, ($t1)

glabel panic
    lui   $at, %hi(__osThreadTail + 0x14)
    sw    $k0, %lo(__osThreadTail + 0x14)($at)
    li    $t1, 1
    sh    $t1, 0x10($k0)
    li    $t1, 2
    sh    $t1, 0x12($k0)
    mfc0  $t2, $8
    sw    $t2, 0x124($k0)
    jal   send_mesg
     li    $a0, 96
    j     __osDispatchThread
     nop

glabel send_mesg
    la    $t2, __osEventStateTab
    addu  $t2, $t2, $a0
    lw    $t1, ($t2)
    move  $s2, $ra
    beqz  $t1, .L80326CC4
     nop
    lw    $t3, 8($t1)
    lw    $t4, 0x10($t1)
    slt   $at, $t3, $t4
    beqz  $at, .L80326CC4
     nop
    lw    $t5, 0xc($t1)
    addu  $t5, $t5, $t3
    div   $zero, $t5, $t4
    bnez  $t4, .L80326C60
     nop
    break 7
.L80326C60:
    li    $at, -1
    bne   $t4, $at, .L80326C78
     lui   $at, 0x8000
    bne   $t5, $at, .L80326C78
     nop
    break 6
.L80326C78:
    lw    $t4, 0x14($t1)
    mfhi  $t5
    sll   $t5, $t5, 2
    addu  $t4, $t4, $t5
    lw    $t5, 4($t2)
    addiu $t2, $t3, 1
    sw    $t5, ($t4)
    sw    $t2, 8($t1)
    lw    $t2, ($t1)
    lw    $t3, ($t2)
    beqz  $t3, .L80326CC4
     nop
    jal   __osPopThread
     move  $a0, $t1
    move  $t2, $v0
    lui   $a0, %hi(__osThreadTail + 0x8)
    move  $a1, $t2
    jal   __osEnqueueThread
     addiu $a0, %lo(__osThreadTail + 0x8)
.L80326CC4:
    jr    $s2
     nop
handle_CpU: // coprocessor error
    lui   $at, 0x3000
    and   $t1, $t0, $at
    srl   $t1, $t1, 0x1c
    li    $t2, 1
    bne   $t1, $t2, panic
     nop
    lw    $k1, 0x118($k0)
    lui   $at, 0x2000
    li    $t1, 1
    or    $k1, $k1, $at
    sw    $t1, 0x18($k0)
    b     enqueue_running
     sw    $k1, 0x118($k0)


glabel __osEnqueueAndYield
    lui   $a1, %hi(__osThreadTail + 0x10)
    lw    $a1, %lo(__osThreadTail + 0x10)($a1)
    mfc0  $t0, $12
    lw    $k1, 0x18($a1)
    ori   $t0, $t0, 2
    sw    $t0, 0x118($a1)
    sd    $s0, 0x98($a1)
    sd    $s1, 0xa0($a1)
    sd    $s2, 0xa8($a1)
    sd    $s3, 0xb0($a1)
    sd    $s4, 0xb8($a1)
    sd    $s5, 0xc0($a1)
    sd    $s6, 0xc8($a1)
    sd    $s7, 0xd0($a1)
    sd    $gp, 0xe8($a1)
    sd    $sp, 0xf0($a1)
    sd    $fp, 0xf8($a1)
    sd    $ra, 0x100($a1)
    beqz  $k1, .L80326D70
     sw    $ra, 0x11c($a1)
    cfc1  $k1, $31
    sdc1  $f20, 0x180($a1)
    sdc1  $f22, 0x188($a1)
    sdc1  $f24, 0x190($a1)
    sdc1  $f26, 0x198($a1)
    sdc1  $f28, 0x1a0($a1)
    sdc1  $f30, 0x1a8($a1)
    sw    $k1, 0x12c($a1)

.L80326D70:
    lui   $k1, %hi(PHYS_TO_K1(MI_INTR_MASK_REG))
    lw    $k1, %lo(PHYS_TO_K1(MI_INTR_MASK_REG))($k1)
    beqz  $a0, .L80326D88
     sw    $k1, 0x128($a1)
    jal   __osEnqueueThread
     nop
.L80326D88:
    j     __osDispatchThread
     nop

// enqueue and pop look like compiled functions?  but there's no easy way to extract them
glabel __osEnqueueThread
    lw    $t8, ($a0)
    lw    $t7, 4($a1)
    move  $t9, $a0
    lw    $t6, 4($t8)
    slt   $at, $t6, $t7
    bnez  $at, .L80326DC4
     nop
.L80326DAC:
    move  $t9, $t8
    lw    $t8, ($t8)
    lw    $t6, 4($t8)
    slt   $at, $t6, $t7
    beqz  $at, .L80326DAC
     nop
.L80326DC4:
    lw    $t8, ($t9)
    sw    $t8, ($a1)
    sw    $a1, ($t9)
    jr    $ra
     sw    $a0, 8($a1)

glabel __osPopThread
    lw    $v0, ($a0)
    lw    $t9, ($v0)
    jr    $ra
     sw    $t9, ($a0)

glabel __osDispatchThread
    lui   $a0, %hi(__osThreadTail + 0x8)
    jal   __osPopThread
     addiu $a0, %lo(__osThreadTail + 0x8)
    lui   $at, %hi(__osThreadTail + 0x10)
    sw    $v0, %lo(__osThreadTail + 0x10)($at)
    li    $t0, 4
    sh    $t0, 0x10($v0)
    move  $k0, $v0
.L80326E08:
    ld    $k1, 0x108($k0)
    ld    $at, 0x20($k0)
    ld    $v0, 0x28($k0)
    mtlo  $k1
    ld    $k1, 0x110($k0)
    ld    $v1, 0x30($k0)
    ld    $a0, 0x38($k0)
    ld    $a1, 0x40($k0)
    ld    $a2, 0x48($k0)
    ld    $a3, 0x50($k0)
    ld    $t0, 0x58($k0)
    ld    $t1, 0x60($k0)
    ld    $t2, 0x68($k0)
    ld    $t3, 0x70($k0)
    ld    $t4, 0x78($k0)
    ld    $t5, 0x80($k0)
    ld    $t6, 0x88($k0)
    ld    $t7, 0x90($k0)
    ld    $s0, 0x98($k0)
    ld    $s1, 0xa0($k0)
    ld    $s2, 0xa8($k0)
    ld    $s3, 0xb0($k0)
    ld    $s4, 0xb8($k0)
    ld    $s5, 0xc0($k0)
    ld    $s6, 0xc8($k0)
    ld    $s7, 0xd0($k0)
    ld    $t8, 0xd8($k0)
    ld    $t9, 0xe0($k0)
    ld    $gp, 0xe8($k0)
    mthi  $k1
    ld    $sp, 0xf0($k0)
    ld    $fp, 0xf8($k0)
    ld    $ra, 0x100($k0)
    lw    $k1, 0x11c($k0)
    mtc0  $k1, $14
    lw    $k1, 0x118($k0)
    mtc0  $k1, $12
    lw    $k1, 0x18($k0)
    beqz  $k1, .L80326EF0
     nop
    lw    $k1, 0x12c($k0)
    ctc1  $k1, $31
    ldc1  $f0, 0x130($k0)
    ldc1  $f2, 0x138($k0)
    ldc1  $f4, 0x140($k0)
    ldc1  $f6, 0x148($k0)
    ldc1  $f8, 0x150($k0)
    ldc1  $f10, 0x158($k0)
    ldc1  $f12, 0x160($k0)
    ldc1  $f14, 0x168($k0)
    ldc1  $f16, 0x170($k0)
    ldc1  $f18, 0x178($k0)
    ldc1  $f20, 0x180($k0)
    ldc1  $f22, 0x188($k0)
    ldc1  $f24, 0x190($k0)
    ldc1  $f26, 0x198($k0)
    ldc1  $f28, 0x1a0($k0)
    ldc1  $f30, 0x1a8($k0)
.L80326EF0:
    lw    $k1, 0x128($k0)
    sll   $k1, $k1, 1
    la    $k0, __osRcpImTable
    addu  $k1, $k1, $k0
    lhu   $k1, ($k1)
    // TODO: is this an la?
    lui   $k0, %hi(PHYS_TO_K1(MI_INTR_MASK_REG))
    addiu $k0, %lo(PHYS_TO_K1(MI_INTR_MASK_REG))
    sw    $k1, ($k0)
    nop
    nop
    nop
    nop
    eret
glabel __osCleanupThread
    jal   osDestroyThread
     move  $a0, $zero

.section .data

glabel __osHwIntTable
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0

glabel D_80334934
    .word 0

glabel D_80334938
    .word 0
    .word 0

.section .rodata

glabel __osIntOffTable
    .byte 0x00,0x14,0x18,0x18,0x1C,0x1C,0x1C,0x1C,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x00,0x04,0x08,0x08,0x0C,0x0C,0x0C,0x0C,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10

glabel __osIntTable
    .word redispatch
    .word sw1
    .word sw2
    .word rcp
    .word cart
    .word prenmi
    .word panic
    .word panic
    .word counter
    .word 0
    .word 0
    .word 0
