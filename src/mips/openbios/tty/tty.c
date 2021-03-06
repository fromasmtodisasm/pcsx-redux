/***************************************************************************
 *   Copyright (C) 2020 PCSX-Redux authors                                 *
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

#include <stddef.h>

#include "common/compiler/stdint.h"
#include "common/hardware/util.h"
#include "common/hardware/hwregs.h"
#include "common/psxlibc/circularbuffer.h"
#include "common/psxlibc/ioctl.h"
#include "common/psxlibc/stdio.h"
#include "common/syscalls/syscalls.h"
#include "openbios/kernel/libcmisc.h"
#include "openbios/tty/tty.h"

int g_cachedInstallTTY;
int g_installTTY;

static const struct Device s_ttyDevice = {
    .name = "tty",
    .flags = 3,
    .blockSize = 1,
    .desc = "CONSOLE",
    .init = dev_tty_init,
    .open = dev_tty_open,
    .action = dev_tty_action,
    .close = psxdummy,
    .ioctl = dev_tty_ioctl,
    .read = psxdummy,
    .write = psxdummy,
    .erase = psxdummy,
    .undelete = psxdummy,
    .firstfile = psxdummy,
    .nextfile = psxdummy,
    .format = psxdummy,
    .chdir = psxdummy,
    .rename = psxdummy,
    .deinit = psxdummy,
    .check = psxdummy,
};

int addConsoleDevice() {
    return syscall_addDevice(&s_ttyDevice);
}

static const struct Device s_dummyDevice = {
    .name = "tty",
    .flags = 1,
    .blockSize = 1,
    .desc = "CONSOLE",
    .init = psxdummy,
    .open = psxdummy,
    .action = psxdummy,
    .close = psxdummy,
    .ioctl = psxdummy,
    .read = psxdummy,
    .write = psxdummy,
    .erase = psxdummy,
    .undelete = psxdummy,
    .firstfile = psxdummy,
    .nextfile = psxdummy,
    .format = psxdummy,
    .chdir = psxdummy,
    .rename = psxdummy,
    .deinit = psxdummy,
    .check = psxdummy,
};

int addDummyConsoleDevice() {
    return syscall_addDevice(&s_dummyDevice);
}

static volatile uint8_t * s_atconsStatPtr;
static volatile uint8_t * s_atconsIRQPtr;
static struct CircularBuffer s_circ;

/* The following code is from the DTL-H2000 bios,
   instead of the retail bios, which is much more
   complex, and useless for the purpose of this project. */
void dev_tty_init() {
    s_atconsStatPtr = &ATCONS_STAT;
    ATCONS_IRQ2 &= 0xfe;
    s_atconsIRQPtr = &ATCONS_IRQ;
    flushWriteQueue();
    s_atconsIRQPtr[0] = 0x20;
    s_atconsIRQPtr[2] |= 0x10;
    flushWriteQueue();
    s_circ.start = s_circ.end = NULL;
}

int dev_tty_open(struct File * file) {
    file->flags |= PSXF_SCAN2;
    s_circ.start = s_circ.end = s_circ.buffer;
}

static int ttyGetChar() {
    if ((*s_atconsStatPtr & 0x10) != 0) {
        int c = s_atconsStatPtr[2];
        s_atconsIRQPtr[0] = 0x20;
        s_atconsIRQPtr[2] |= 0x10;
        flushWriteQueue();
        return c | 0x100;
    }
    return 0;
}

static void ttyPutChar(int c) {
    while (s_circ.flags & PSXCIRC_STOPPED) syscall_cdevscan();
    while ((*s_atconsStatPtr & 3) == 0) syscall_cdevscan();
    s_atconsStatPtr[2] = c;
    s_atconsIRQPtr[2] |= 0x10;
    flushWriteQueue();
}

int dev_tty_action(struct File * file, enum FileAction action) {
    int count = file->count;
    switch (action) {
        case PSXREAD:
            while (count > 0) {
                int c;
                while ((c = ttyGetChar())) syscall_cdevinput(&s_circ, c);
                if (((file->flags & PSXF_NBLOCK) == 0) && (s_circ.start == s_circ.end)) {
                    do {
                        syscall_cdevscan();
                    } while (s_circ.start == s_circ.end);
                }
                if (s_circ.start == s_circ.end) return count - file->count;
                *file->buffer++ = syscall_circgetc(&s_circ);
                count = --file->count;
            }
            break;
        case PSXWRITE:
            if ((file->flags & PSXF_WRITE) != 0) {
                while (count > 0) {
                    ttyPutChar(*file->buffer++);
                    count = file->count--;
                }
                break;
            }
        default:
            count = syscall_ioabort("tty(atcons) bad function");
            break; 
    }
    return count;
}

int dev_tty_ioctl(struct File *file,int req, int arg) {
    char c;
    switch (req) {
        case PSXFIOCSCAN:
            while ((c = ttyGetChar())) syscall_cdevinput(&s_circ, c);
            break;
        case PSXTIOCRAW:
            if (arg) {
                s_circ.flags |= PSXCIRC_RAW;
            } else {
                s_circ.flags &= ~PSXCIRC_RAW;
            }
            break;
        case PSXTIOCFLUSH:
            s_circ.start = s_circ.end = s_circ.buffer;
            break;
        case PSXTIOCREOPEN:
            return dev_tty_open(file);
            break;
        default:
            file->errno = PSXEINVAL;
            return -1;
            break;
    }
    return 0;
}
