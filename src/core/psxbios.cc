/***************************************************************************
 *   Copyright (C) 2007 Ryan Schultz, PCSX-df Team, PCSX team              *
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

/*
 * Internal simulated HLE BIOS.
 */

#include "core/psxbios.h"

const char *PCSX::Bios::A0names[256] = {
    // 0x00
    "open",
    "lseek",
    "read",
    "write",
    "close",
    "ioctl",
    "exit",
    "sys_a0_07",
    "getc",
    "putc",
    "todigit",
    "atof",
    "strtoul",
    "strtol",
    "abs",
    "labs",
    // 0x10
    "atoi",
    "atol",
    "atob",
    "setjmp",
    "longjmp",
    "strcat",
    "strncat",
    "strcmp",
    "strncmp",
    "strcpy",
    "strncpy",
    "strlen",
    "index",
    "rindex",
    "strchr",
    "strrchr",
    // 0x20
    "strpbrk",
    "strspn",
    "strcspn",
    "strtok",
    "strstr",
    "toupper",
    "tolower",
    "bcopy",
    "bzero",
    "bcmp",
    "memcpy",
    "memset",
    "memmove",
    "memcmp",
    "memchr",
    "rand",
    // 0x30
    "srand",
    "qsort",
    "strtod",
    "malloc",
    "free",
    "lsearch",
    "bsearch",
    "calloc",
    "realloc",
    "InitHeap",
    "_exit",
    "getchar",
    "putchar",
    "gets",
    "puts",
    "printf",
    // 0x40
    "sys_a0_40",
    "LoadTest",
    "Load",
    "Exec",
    "FlushCache",
    "InstallInterruptHandler",
    "GPU_dw",
    "mem2vram",
    "SendGPUStatus",
    "GPU_cw",
    "GPU_cwb",
    "SendPackets",
    "sys_a0_4c",
    "GetGPUStatus",
    "GPU_sync",
    "sys_a0_4f",
    // 0x50
    "sys_a0_50",
    "LoadExec",
    "GetSysSp",
    "sys_a0_53",
    "_96_init()",
    "_bu_init()",
    "_96_remove()",
    "sys_a0_57",
    "sys_a0_58",
    "sys_a0_59",
    "sys_a0_5a",
    "dev_tty_init",
    "dev_tty_open",
    "sys_a0_5d",
    "dev_tty_ioctl",
    "dev_cd_open",
    // 0x60
    "dev_cd_read",
    "dev_cd_close",
    "dev_cd_firstfile",
    "dev_cd_nextfile",
    "dev_cd_chdir",
    "dev_card_open",
    "dev_card_read",
    "dev_card_write",
    "dev_card_close",
    "dev_card_firstfile",
    "dev_card_nextfile",
    "dev_card_erase",
    "dev_card_undelete",
    "dev_card_format",
    "dev_card_rename",
    "dev_card_6f",
    // 0x70
    "_bu_init",
    "_96_init",
    "_96_remove",
    "sys_a0_73",
    "sys_a0_74",
    "sys_a0_75",
    "sys_a0_76",
    "sys_a0_77",
    "_96_CdSeekL",
    "sys_a0_79",
    "sys_a0_7a",
    "sys_a0_7b",
    "_96_CdGetStatus",
    "sys_a0_7d",
    "_96_CdRead",
    "sys_a0_7f",
    // 0x80
    "sys_a0_80",
    "sys_a0_81",
    "sys_a0_82",
    "sys_a0_83",
    "sys_a0_84",
    "_96_CdStop",
    "sys_a0_86",
    "sys_a0_87",
    "sys_a0_88",
    "sys_a0_89",
    "sys_a0_8a",
    "sys_a0_8b",
    "sys_a0_8c",
    "sys_a0_8d",
    "sys_a0_8e",
    "sys_a0_8f",
    // 0x90
    "sys_a0_90",
    "sys_a0_91",
    "sys_a0_92",
    "sys_a0_93",
    "sys_a0_94",
    "sys_a0_95",
    "AddCDROMDevice",
    "AddMemCardDevide",
    "DisableKernelIORedirection",
    "EnableKernelIORedirection",
    "sys_a0_9a",
    "sys_a0_9b",
    "SetConf",
    "GetConf",
    "sys_a0_9e",
    "SetMem",
    // 0xa0
    "_boot",
    "SystemError",
    "EnqueueCdIntr",
    "DequeueCdIntr",
    "sys_a0_a4",
    "ReadSector",
    "get_cd_status",
    "bufs_cb_0",
    "bufs_cb_1",
    "bufs_cb_2",
    "bufs_cb_3",
    "_card_info",
    "_card_load",
    "_card_auto",
    "bufs_cd_4",
    "sys_a0_af",
    // 0xb0
    "sys_a0_b0",
    "sys_a0_b1",
    "do_a_long_jmp",
    "sys_a0_b3",
    "?? sub_function",
};

const char *PCSX::Bios::B0names[256] = {
    // 0x00
    "SysMalloc",
    "sys_b0_01",
    "sys_b0_02",
    "sys_b0_03",
    "sys_b0_04",
    "sys_b0_05",
    "sys_b0_06",
    "DeliverEvent",
    "OpenEvent",
    "CloseEvent",
    "WaitEvent",
    "TestEvent",
    "EnableEvent",
    "DisableEvent",
    "OpenTh",
    "CloseTh",
    // 0x10
    "ChangeTh",
    "sys_b0_11",
    "InitPAD",
    "StartPAD",
    "StopPAD",
    "PAD_init",
    "PAD_dr",
    "ReturnFromExecption",
    "ResetEntryInt",
    "HookEntryInt",
    "sys_b0_1a",
    "sys_b0_1b",
    "sys_b0_1c",
    "sys_b0_1d",
    "sys_b0_1e",
    "sys_b0_1f",
    // 0x20
    "UnDeliverEvent",
    "sys_b0_21",
    "sys_b0_22",
    "sys_b0_23",
    "sys_b0_24",
    "sys_b0_25",
    "sys_b0_26",
    "sys_b0_27",
    "sys_b0_28",
    "sys_b0_29",
    "sys_b0_2a",
    "sys_b0_2b",
    "sys_b0_2c",
    "sys_b0_2d",
    "sys_b0_2e",
    "sys_b0_2f",
    // 0x30
    "sys_b0_30",
    "sys_b0_31",
    "open",
    "lseek",
    "read",
    "write",
    "close",
    "ioctl",
    "exit",
    "sys_b0_39",
    "getc",
    "putc",
    "getchar",
    "putchar",
    "gets",
    "puts",
    // 0x40
    "cd",
    "format",
    "firstfile",
    "nextfile",
    "rename",
    "delete",
    "undelete",
    "AddDevice",
    "RemoteDevice",
    "PrintInstalledDevices",
    "InitCARD",
    "StartCARD",
    "StopCARD",
    "sys_b0_4d",
    "_card_write",
    "_card_read",
    // 0x50
    "_new_card",
    "Krom2RawAdd",
    "sys_b0_52",
    "sys_b0_53",
    "_get_errno",
    "_get_error",
    "GetC0Table",
    "GetB0Table",
    "_card_chan",
    "sys_b0_59",
    "sys_b0_5a",
    "ChangeClearPAD",
    "_card_status",
    "_card_wait",
};

const char *PCSX::Bios::C0names[256] = {
    // 0x00
    "InitRCnt",
    "InitException",
    "SysEnqIntRP",
    "SysDeqIntRP",
    "get_free_EvCB_slot",
    "get_free_TCB_slot",
    "ExceptionHandler",
    "InstallExeptionHandler",
    "SysInitMemory",
    "SysInitKMem",
    "ChangeClearRCnt",
    "SystemError",
    "InitDefInt",
    "sys_c0_0d",
    "sys_c0_0e",
    "sys_c0_0f",
    // 0x10
    "sys_c0_10",
    "sys_c0_11",
    "InstallDevices",
    "FlushStfInOutPut",
    "sys_c0_14",
    "_cdevinput",
    "_cdevscan",
    "_circgetc",
    "_circputc",
    "ioabort",
    "sys_c0_1a",
    "KernelRedirect",
    "PatchAOTable",
};

//#define r0 (PCSX::g_emulator.m_psxCpu->m_psxRegs.GPR.n.r0)
#define at (PCSX::g_emulator.m_psxCpu->m_psxRegs.GPR.n.at)
#define v0 (PCSX::g_emulator.m_psxCpu->m_psxRegs.GPR.n.v0)
#define v1 (PCSX::g_emulator.m_psxCpu->m_psxRegs.GPR.n.v1)
#define a0 (PCSX::g_emulator.m_psxCpu->m_psxRegs.GPR.n.a0)
#define a1 (PCSX::g_emulator.m_psxCpu->m_psxRegs.GPR.n.a1)
#define a2 (PCSX::g_emulator.m_psxCpu->m_psxRegs.GPR.n.a2)
#define a3 (PCSX::g_emulator.m_psxCpu->m_psxRegs.GPR.n.a3)
#define t0 (PCSX::g_emulator.m_psxCpu->m_psxRegs.GPR.n.t0)
#define t1 (PCSX::g_emulator.m_psxCpu->m_psxRegs.GPR.n.t1)
#define t2 (PCSX::g_emulator.m_psxCpu->m_psxRegs.GPR.n.t2)
#define t3 (PCSX::g_emulator.m_psxCpu->m_psxRegs.GPR.n.t3)
#define t4 (PCSX::g_emulator.m_psxCpu->m_psxRegs.GPR.n.t4)
#define t5 (PCSX::g_emulator.m_psxCpu->m_psxRegs.GPR.n.t5)
#define t6 (PCSX::g_emulator.m_psxCpu->m_psxRegs.GPR.n.t6)
#define t7 (PCSX::g_emulator.m_psxCpu->m_psxRegs.GPR.n.t7)
#define t8 (PCSX::g_emulator.m_psxCpu->m_psxRegs.GPR.n.t8)
#define t9 (PCSX::g_emulator.m_psxCpu->m_psxRegs.GPR.n.t9)
#define s0 (PCSX::g_emulator.m_psxCpu->m_psxRegs.GPR.n.s0)
#define s1 (PCSX::g_emulator.m_psxCpu->m_psxRegs.GPR.n.s1)
#define s2 (PCSX::g_emulator.m_psxCpu->m_psxRegs.GPR.n.s2)
#define s3 (PCSX::g_emulator.m_psxCpu->m_psxRegs.GPR.n.s3)
#define s4 (PCSX::g_emulator.m_psxCpu->m_psxRegs.GPR.n.s4)
#define s5 (PCSX::g_emulator.m_psxCpu->m_psxRegs.GPR.n.s5)
#define s6 (PCSX::g_emulator.m_psxCpu->m_psxRegs.GPR.n.s6)
#define s7 (PCSX::g_emulator.m_psxCpu->m_psxRegs.GPR.n.s7)
#define k0 (PCSX::g_emulator.m_psxCpu->m_psxRegs.GPR.n.k0)
#define k1 (PCSX::g_emulator.m_psxCpu->m_psxRegs.GPR.n.k1)
#define gp (PCSX::g_emulator.m_psxCpu->m_psxRegs.GPR.n.gp)
#define sp (PCSX::g_emulator.m_psxCpu->m_psxRegs.GPR.n.sp)
#define fp (PCSX::g_emulator.m_psxCpu->m_psxRegs.GPR.n.s8)
#define ra (PCSX::g_emulator.m_psxCpu->m_psxRegs.GPR.n.ra)
#define pc0 (PCSX::g_emulator.m_psxCpu->m_psxRegs.pc)

#define Ra0 (assert(PSXM(a0) != NULL), (char *)PSXM(a0))
#define Ra1 (assert(PSXM(a1) != NULL), (char *)PSXM(a1))
#define Ra2 (assert(PSXM(a2) != NULL), (char *)PSXM(a2))
#define Ra3 (assert(PSXM(a3) != NULL), (char *)PSXM(a3))
#define Rv0 (assert(PSXM(v0) != NULL), (char *)PSXM(v0))
#define Rsp (assert(PSXM(sp) != NULL), (char *)PSXM(sp))

class BiosImpl : public PCSX::Bios {
    typedef struct {
        uint32_t desc;
        int32_t status;
        int32_t mode;
        uint32_t fhandler;
    } EvCB[32];

    enum {
        EvStUNUSED = 0x0000,
        EvStWAIT = 0x1000,
        EvStACTIVE = 0x2000,
        EvStALREADY = 0x4000,

        EvMdINTR = 0x1000,
        EvMdNOINTR = 0x2000,
    };

    typedef struct {
        int32_t next;
        int32_t func1;
        int32_t func2;
        int32_t pad;
    } SysRPst;

    typedef struct {
        int32_t status;
        int32_t mode;
        uint32_t reg[32];
        uint32_t func;
    } TCB;

    typedef struct {
        uint32_t _pc0;
        uint32_t gp0;
        uint32_t t_addr;
        uint32_t t_size;
        uint32_t d_addr;
        uint32_t d_size;
        uint32_t b_addr;
        uint32_t b_size;
        uint32_t S_addr;
        uint32_t s_size;
        uint32_t _sp, _fp, _gp, ret, base;
    } EXEC;

    struct DIRENTRY {
        char name[20];
        int32_t attr;
        int32_t size;
        uint32_t next;
        int32_t head;
        char system[4];
    };

    typedef struct {
        char name[32];
        uint32_t mode;
        uint32_t offset;
        uint32_t size;
        uint32_t mcfile;
    } FileDesc;

    uint32_t *s_jmp_int = NULL;
    int *s_pad_buf = NULL;
    char *s_pad_buf1 = NULL, *s_pad_buf2 = NULL;
    int s_pad_buf1len, s_pad_buf2len;

    uint32_t s_regs[35];
    EvCB *s_Event;
    EvCB *s_HwEV;  // 0xf0
    EvCB *s_EvEV;  // 0xf1
    EvCB *s_RcEV;  // 0xf2
    EvCB *s_UeEV;  // 0xf3
    EvCB *s_SwEV;  // 0xf4
    EvCB *s_ThEV;  // 0xff
    uint32_t *s_heap_addr = NULL;
    uint32_t *s_heap_end = NULL;
    uint32_t s_SysIntRP[8];
    int s_CardState = -1;
    TCB s_Thread[8];
    int s_CurThread = 0;
    FileDesc s_FDesc[32];
    uint32_t s_card_active_chan = 0;

    bool m_hleSoftCall = false;

    inline void softCall(uint32_t pc) {
        pc0 = pc;
        ra = 0x80001000;

        m_hleSoftCall = true;

        while (pc0 != 0x80001000) PCSX::g_emulator.m_psxCpu->ExecuteBlock();

        m_hleSoftCall = false;
    }

    inline void softCall2(uint32_t pc) {
        uint32_t sra = ra;
        pc0 = pc;
        ra = 0x80001000;

        m_hleSoftCall = true;

        while (pc0 != 0x80001000) PCSX::g_emulator.m_psxCpu->ExecuteBlock();
        ra = sra;

        m_hleSoftCall = false;
    }

    inline void DeliverEvent(uint32_t ev, uint32_t spec) {
        if (s_Event[ev][spec].status != EvStACTIVE) return;

        //  s_Event[ev][spec].status = EvStALREADY;
        if (s_Event[ev][spec].mode == EvMdINTR) {
            softCall2(s_Event[ev][spec].fhandler);
        } else
            s_Event[ev][spec].status = EvStALREADY;
    }

    inline void SaveRegs() {
        memcpy(s_regs, PCSX::g_emulator.m_psxCpu->m_psxRegs.GPR.r, 32 * 4);
        s_regs[32] = PCSX::g_emulator.m_psxCpu->m_psxRegs.GPR.n.lo;
        s_regs[33] = PCSX::g_emulator.m_psxCpu->m_psxRegs.GPR.n.hi;
        s_regs[34] = PCSX::g_emulator.m_psxCpu->m_psxRegs.pc;
    }

    inline void LoadRegs() {
        memcpy(PCSX::g_emulator.m_psxCpu->m_psxRegs.GPR.r, s_regs, 32 * 4);
        PCSX::g_emulator.m_psxCpu->m_psxRegs.GPR.n.lo = s_regs[32];
        PCSX::g_emulator.m_psxCpu->m_psxRegs.GPR.n.hi = s_regs[33];
    }

    void psxBios_printf() {  // 0x3f
        char tmp[1024];
        char tmp2[1024];
        uint32_t save[4];
        char *ptmp = tmp;
        int n = 1, i = 0, j;

        memcpy(save, Rsp, 4 * 4);
        psxMu32ref(sp) = SWAP_LE32((uint32_t)a0);
        psxMu32ref(sp + 4) = SWAP_LE32((uint32_t)a1);
        psxMu32ref(sp + 8) = SWAP_LE32((uint32_t)a2);
        psxMu32ref(sp + 12) = SWAP_LE32((uint32_t)a3);

        while (Ra0[i]) {
            switch (Ra0[i]) {
                case '%':
                    j = 0;
                    tmp2[j++] = '%';
                _start:
                    switch (Ra0[++i]) {
                        case '.':
                        case 'l':
                            tmp2[j++] = Ra0[i];
                            goto _start;
                        default:
                            if (Ra0[i] >= '0' && Ra0[i] <= '9') {
                                tmp2[j++] = Ra0[i];
                                goto _start;
                            }
                            break;
                    }
                    tmp2[j++] = Ra0[i];
                    tmp2[j] = 0;

                    switch (Ra0[i]) {
                        case 'f':
                        case 'F':
                            ptmp += sprintf(ptmp, tmp2, (float)psxMu32(sp + n * 4));
                            n++;
                            break;
                        case 'a':
                        case 'A':
                        case 'e':
                        case 'E':
                        case 'g':
                        case 'G':
                            ptmp += sprintf(ptmp, tmp2, (double)psxMu32(sp + n * 4));
                            n++;
                            break;
                        case 'p':
                        case 'i':
                        case 'u':
                        case 'd':
                        case 'D':
                        case 'o':
                        case 'O':
                        case 'x':
                        case 'X':
                            ptmp += sprintf(ptmp, tmp2, (unsigned int)psxMu32(sp + n * 4));
                            n++;
                            break;
                        case 'c':
                            ptmp += sprintf(ptmp, tmp2, (unsigned char)psxMu32(sp + n * 4));
                            n++;
                            break;
                        case 's':
                            ptmp += sprintf(ptmp, tmp2, (char *)PSXM(psxMu32(sp + n * 4)));
                            n++;
                            break;
                        case '%':
                            *ptmp++ = Ra0[i];
                            break;
                    }
                    i++;
                    break;
                default:
                    *ptmp++ = Ra0[i++];
            }
        }
        *ptmp = 0;

        memcpy(Rsp, save, 4 * 4);

        PCSX::g_system->biosPrintf("%s", tmp);

        pc0 = ra;
    }

    static const size_t PSXSTRBUFMAX = 255;
    char psxstrbuf[PSXSTRBUFMAX + 1];
    unsigned short psxstrbuf_count = 0;

    void psxBios_putchar() {  // 3d
        char logchar = (a0 == 0xa ? '>' : (char)a0);
        if (psxstrbuf_count < PSXSTRBUFMAX) psxstrbuf[psxstrbuf_count++] = logchar;

        PCSX::g_system->biosPrintf("%c", (char)a0);
        if ((a0 == 0xa && psxstrbuf_count >= 2) || psxstrbuf_count >= PSXSTRBUFMAX) {
            psxstrbuf[psxstrbuf_count++] = '\0';
            PSXBIOS_LOG("psxBios_%s: string_[%d]_cr: %s\n", B0names[0x3d], psxstrbuf_count, psxstrbuf);
            psxstrbuf_count = 0;
        }

        pc0 = ra;
    }

    void psxBios_puts() {  // 3e/3f
        PCSX::g_system->biosPrintf("%s", Ra0);
        pc0 = ra;
    }

    void (BiosImpl::*m_biosA0[256])();
    void (BiosImpl::*m_biosB0[256])();
    void (BiosImpl::*m_biosC0[256])();

    bool callA0(unsigned index) final {
        if (m_biosA0[index]) (*this.*m_biosA0[index])();
        return !!m_biosA0[index];
    }
    bool callB0(unsigned index) final {
        if (m_biosB0[index]) (*this.*m_biosB0[index])();
        return !!m_biosB0[index];
    }
    bool callC0(unsigned index) final {
        if (m_biosC0[index]) (*this.*m_biosC0[index])();
        return !!m_biosC0[index];
    }

    void psxBiosInit() final {
        int i;

        for (i = 0; i < 256; i++) {
            m_biosA0[i] = NULL;
            m_biosB0[i] = NULL;
            m_biosC0[i] = NULL;
        }
        m_biosA0[0x3e] = &BiosImpl::psxBios_puts;
        m_biosA0[0x3f] = &BiosImpl::psxBios_printf;

        m_biosB0[0x3d] = &BiosImpl::psxBios_putchar;
        m_biosB0[0x3f] = &BiosImpl::psxBios_puts;

        if (!PCSX::g_emulator.settings.get<PCSX::Emulator::SettingHLE>()) return;
        abort();
    }

    void psxBiosShutdown() final {}
    void psxBiosException() final {}
    void psxBiosFreeze(int Mode) final {}
};

PCSX::Bios *PCSX::Bios::factory() { return new BiosImpl(); }
