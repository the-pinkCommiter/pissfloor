.set noat
.set noreorder // don't insert nops after branches

#include "macros.inc"

#include <PR/R4300.h>


.section .text, "ax"

glabel __osSetCompare
    mtc0  $a0, C0_COMPARE
    jr    $ra
     nop
