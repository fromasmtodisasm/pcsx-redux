/***************************************************************************
 *   Copyright (C) 2019 PCSX-Redux authors                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.           *
 ***************************************************************************/

.include "common/hardware/hwregs.inc"

    .section .text, "ax", @progbits
    .align 2
    .global flushCache
    .type flushCache, @function

flushCache:
    mfc0  $t3, $12
    nop

    /* Around this location, the retail bios does a bal to the next instruction,
       in order to ensure this is run from the 0xbfc bios space, since the next
       few instructions are going to effectively unplug all other busses from
       the main CPU. Since flushCache can only be called from that space,
       thanks to the syscall, this should not be necessary here.
       
       A few other changes from the retail bios:
         - the cop0 status register is mutated BEFORE unplugging the busses.
         - the register k0 is left untouched.
     */

    li    $t1, 0x10000
    mtc0  $t1, $12
    nop
    nop

    /* The BIU_CONFIG register at 0xfffe0130 is a bit of a mystery at the moment.
       Further investigation is required here to properly understand what this
       code does exactly. Educated guesses are saying this is replacing the main
       ram with a direct access to the i-caches. There seems to be a way to remap
       the scratchpad into a d-cache also. But until the purpose of this register
       is better understood, this code will remain an almost exact copy of the
       retail code, as to not cause any problem. */

    li    $t0, 0x804
    sw    $t0, BIU_CONFIG

    move  $t0, $0
    li    $t2, 0x1000

cache_init_1:
    sw    $0, 0x00($t0)
    sw    $0, 0x10($t0)
    sw    $0, 0x20($t0)
    sw    $0, 0x30($t0)
    sw    $0, 0x40($t0)
    sw    $0, 0x50($t0)
    sw    $0, 0x60($t0)
    sw    $0, 0x70($t0)
    addi  $t0, 0x80
    bne   $t0, $t2, cache_init_1

    mtc0  $0, $12
    nop

    li    $t0, 0x800
    sw    $t0, BIU_CONFIG

    mtc0  $t1, $12
    nop
    nop

    move  $t0, $0
    li    $t2, 0x1000

cache_init_2:
    sw    $0, 0x00($t0)
    sw    $0, 0x04($t0)
    sw    $0, 0x08($t0)
    sw    $0, 0x0c($t0)
    sw    $0, 0x10($t0)
    sw    $0, 0x14($t0)
    sw    $0, 0x18($t0)
    sw    $0, 0x1c($t0)
    sw    $0, 0x20($t0)
    sw    $0, 0x24($t0)
    sw    $0, 0x28($t0)
    sw    $0, 0x2c($t0)
    sw    $0, 0x30($t0)
    sw    $0, 0x34($t0)
    sw    $0, 0x38($t0)
    sw    $0, 0x3c($t0)
    sw    $0, 0x40($t0)
    sw    $0, 0x44($t0)
    sw    $0, 0x48($t0)
    sw    $0, 0x4c($t0)
    sw    $0, 0x50($t0)
    sw    $0, 0x54($t0)
    sw    $0, 0x58($t0)
    sw    $0, 0x5c($t0)
    sw    $0, 0x60($t0)
    sw    $0, 0x64($t0)
    sw    $0, 0x68($t0)
    sw    $0, 0x6c($t0)
    sw    $0, 0x70($t0)
    sw    $0, 0x74($t0)
    sw    $0, 0x78($t0)
    sw    $0, 0x7c($t0)
    addi  $t0, 0x80
    bne   $t0, $t2, cache_init_2

    mtc0  $0, $12
    nop

    li    $t0, 0xa0000000
    lw    $t1, 0($t0)
    lw    $t1, 0($t0)
    lw    $t1, 0($t0)
    lw    $t1, 0($t0)
    lw    $t1, 0($t0)
    lw    $t1, 0($t0)
    lw    $t1, 0($t0)
    lw    $t1, 0($t0)
    nop

    li    $t0, 0x1e988
    sw    $t0, BIU_CONFIG

    mtc0  $t3, $12
    nop

    jr    $ra
