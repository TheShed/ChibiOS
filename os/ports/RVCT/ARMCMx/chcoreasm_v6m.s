/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
 * Imports the Cortex-Mx parameters header and performs the same calculations
 * done in chcore.h.
 */
#include "cmparams.h"

#define CORTEX_PRIORITY_MASK(n) ((n) << (8 - CORTEX_PRIORITY_BITS))

EXTCTX_SIZE     EQU     32
CONTEXT_OFFSET  EQU     12
SCB_ICSR        EQU     0xE000ED04

                PRESERVE8
                THUMB
                AREA    |.text|, CODE, READONLY

                IMPORT  chThdExit
                IMPORT  chSchIsRescRequiredExI
                IMPORT  chSchDoRescheduleI

/*
 * Performs a context switch between two threads.
 */
                EXPORT _port_switch
_port_switch    PROC
                push    {r4, r5, r6, r7, lr}
                mov     r4, r8
                mov     r5, r9
                mov     r6, r10
                mov     r7, r11
                push    {r4, r5, r6, r7}
                mov     r3, sp
                str     r3, [r1, #CONTEXT_OFFSET]
                ldr     r3, [r0, #CONTEXT_OFFSET]
                mov     sp, r3
                pop     {r4, r5, r6, r7}
                mov     r8, r4
                mov     r9, r5
                mov     r10, r6
                mov     r11, r7
                pop     {r4, r5, r6, r7, pc}
                ENDP

/*
 * Start a thread by invoking its work function.
 * If the work function returns @p chThdExit() is automatically invoked.
 */
                EXPORT  _port_thread_start
_port_thread_start PROC
                cpsie   i
                mov     r0, r5
                blx     r4
                bl      chThdExit
                ENDP

/*
 * NMI vector.
 * The NMI vector is used for exception mode re-entering after a context
 * switch.
 */
                EXPORT  NMIVector
NMIVector       PROC
                mrs     r3, PSP
                adds    r3, r3, #32
                msr     PSP, r3
                cpsie   i
                bx      lr
                ENDP

/*
 * Post-IRQ switch code.
 * Exception handlers return here for context switching.
 */
                EXPORT  _port_switch_from_isr
_port_switch_from_isr PROC
                bl      chSchDoRescheduleI
                movs    r3, #128
                lsls    r3, r3, #24
                ldr     r2, =SCB_ICSR
                str     r3, [r2, #0]
_waitnmi        b       _waitnmi
                ENDP

/*
 * Reschedule verification and setup after an IRQ.
 */
                EXPORT  _port_irq_epilogue
_port_irq_epilogue PROC
                push    {r3, lr}
                adds    r0, r0, #15
                beq     stillnested
                cpsid   i
                bl      chSchIsRescRequiredExI
                cmp     r0, #0
                bne     doresch
                cpsie   i
stillnested
                pop     {r3, pc}
doresch
                mrs     r3, PSP
                subs    r3, r3, #32
                msr     PSP, r3
                ldr     r2, =_port_switch_from_isr
                str     r2, [r3, #24]
                movs    r2, #128
                lsls     r2, r2, #17
                str     r2, [r3, #28]
                pop     {r3, pc}
                ENDP

                END
