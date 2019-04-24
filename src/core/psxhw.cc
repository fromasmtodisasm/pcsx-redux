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
 * Functions for PSX hardware control.
 */

#include "core/psxhw.h"

// Vampire Hunter D hack

static inline void setIrq(uint32_t irq) { psxHu32ref(0x1070) |= SWAP_LEu32(irq); }

void PCSX::HW::psxHwReset() {
    if (PCSX::g_emulator.settings.get<PCSX::Emulator::SettingSioIrq>()) psxHu32ref(0x1070) |= SWAP_LE32(0x80);
    if (PCSX::g_emulator.settings.get<PCSX::Emulator::SettingSpuIrq>()) psxHu32ref(0x1070) |= SWAP_LE32(0x200);

    memset(PCSX::g_emulator.m_psxMem->g_psxH, 0, 0x10000);

    PCSX::g_emulator.m_psxCounters->psxRcntInit();
}

uint8_t PCSX::HW::psxHwRead8(uint32_t add) {
    return 0;
}

uint16_t PCSX::HW::psxHwRead16(uint32_t add) {
    unsigned short hard = 0;

    switch (add) {
        case 0x1f801070:
            PSXHW_LOG("IREG 16bit read %x\n", psxHu16(0x1070));
            return psxHu16(0x1070);
        case 0x1f801074:
            PSXHW_LOG("IMASK 16bit read %x\n", psxHu16(0x1074));
            return psxHu16(0x1074);
        case 0x1f801100:
            hard = PCSX::g_emulator.m_psxCounters->psxRcntRcount(0);
            PSXHW_LOG("T0 count read16: %x\n", hard);
            return hard;
        case 0x1f801104:
            hard = PCSX::g_emulator.m_psxCounters->psxRcntRmode(0);
            PSXHW_LOG("T0 mode read16: %x\n", hard);
            return hard;
        case 0x1f801108:
            hard = PCSX::g_emulator.m_psxCounters->psxRcntRtarget(0);
            PSXHW_LOG("T0 target read16: %x\n", hard);
            return hard;
        case 0x1f801110:
            hard = PCSX::g_emulator.m_psxCounters->psxRcntRcount(1);
            PSXHW_LOG("T1 count read16: %x\n", hard);
            return hard;
        case 0x1f801114:
            hard = PCSX::g_emulator.m_psxCounters->psxRcntRmode(1);
            PSXHW_LOG("T1 mode read16: %x\n", hard);
            return hard;
        case 0x1f801118:
            hard = PCSX::g_emulator.m_psxCounters->psxRcntRtarget(1);
            PSXHW_LOG("T1 target read16: %x\n", hard);
            return hard;
        case 0x1f801120:
            hard = PCSX::g_emulator.m_psxCounters->psxRcntRcount(2);
            PSXHW_LOG("T2 count read16: %x\n", hard);
            return hard;
        case 0x1f801124:
            hard = PCSX::g_emulator.m_psxCounters->psxRcntRmode(2);
            PSXHW_LOG("T2 mode read16: %x\n", hard);
            return hard;
        case 0x1f801128:
            hard = PCSX::g_emulator.m_psxCounters->psxRcntRtarget(2);
            PSXHW_LOG("T2 target read16: %x\n", hard);
            return hard;

            // case 0x1f802030: hard =   //int_2000????
            // case 0x1f802040: hard =//dip switches...??

        default:
            hard = psxHu16(add);
            PSXHW_LOG("*Unkwnown 16bit read at address %x\n", add);
            return hard;
    }

    PSXHW_LOG("*Known 16bit read at address %x value %x\n", add, hard);
    return hard;
}

uint32_t PCSX::HW::psxHwRead32(uint32_t add) {
    uint32_t hard;

    switch (add) {
        case 0x1f801060:
            PSXHW_LOG("RAM size read %x\n", psxHu32(0x1060));
            return psxHu32(0x1060);
        case 0x1f801070:
            PSXHW_LOG("IREG 32bit read %x\n", psxHu32(0x1070));
            return psxHu32(0x1070);
        case 0x1f801074:
            PSXHW_LOG("IMASK 32bit read %x\n", psxHu32(0x1074));
            return psxHu32(0x1074);
        case 0x1f8010a0:
            PSXHW_LOG("DMA2 MADR 32bit read %x\n", psxHu32(0x10a0));
            return SWAP_LEu32(HW_DMA2_MADR);
        case 0x1f8010a4:
            PSXHW_LOG("DMA2 BCR 32bit read %x\n", psxHu32(0x10a4));
            return SWAP_LEu32(HW_DMA2_BCR);
        case 0x1f8010a8:
            PSXHW_LOG("DMA2 CHCR 32bit read %x\n", psxHu32(0x10a8));
            return SWAP_LEu32(HW_DMA2_CHCR);
        case 0x1f8010b0:
            PSXHW_LOG("DMA3 MADR 32bit read %x\n", psxHu32(0x10b0));
            return SWAP_LEu32(HW_DMA3_MADR);
        case 0x1f8010b4:
            PSXHW_LOG("DMA3 BCR 32bit read %x\n", psxHu32(0x10b4));
            return SWAP_LEu32(HW_DMA3_BCR);
        case 0x1f8010b8:
            PSXHW_LOG("DMA3 CHCR 32bit read %x\n", psxHu32(0x10b8));
            return SWAP_LEu32(HW_DMA3_CHCR);
        case 0x1f8010f0:
            PSXHW_LOG("DMA PCR 32bit read %x\n", HW_DMA_PCR);
            return SWAP_LEu32(HW_DMA_PCR);  // DMA control register
        case 0x1f8010f4:
            PSXHW_LOG("DMA ICR 32bit read %x\n", HW_DMA_ICR);
            return SWAP_LEu32(HW_DMA_ICR);  // DMA interrupt register (enable/ack)
        // time for rootcounters :)
        case 0x1f801100:
            hard = PCSX::g_emulator.m_psxCounters->psxRcntRcount(0);
            PSXHW_LOG("T0 count read32: %x\n", hard);
            return hard;
        case 0x1f801104:
            hard = PCSX::g_emulator.m_psxCounters->psxRcntRmode(0);
            PSXHW_LOG("T0 mode read32: %x\n", hard);
            return hard;
        case 0x1f801108:
            hard = PCSX::g_emulator.m_psxCounters->psxRcntRtarget(0);
            PSXHW_LOG("T0 target read32: %x\n", hard);
            return hard;
        case 0x1f801110:
            hard = PCSX::g_emulator.m_psxCounters->psxRcntRcount(1);
            PSXHW_LOG("T1 count read32: %x\n", hard);
            return hard;
        case 0x1f801114:
            hard = PCSX::g_emulator.m_psxCounters->psxRcntRmode(1);
            PSXHW_LOG("T1 mode read32: %x\n", hard);
            return hard;
        case 0x1f801118:
            hard = PCSX::g_emulator.m_psxCounters->psxRcntRtarget(1);
            PSXHW_LOG("T1 target read32: %x\n", hard);
            return hard;
        case 0x1f801120:
            hard = PCSX::g_emulator.m_psxCounters->psxRcntRcount(2);
            PSXHW_LOG("T2 count read32: %x\n", hard);
            return hard;
        case 0x1f801124:
            hard = PCSX::g_emulator.m_psxCounters->psxRcntRmode(2);
            PSXHW_LOG("T2 mode read32: %x\n", hard);
            return hard;
        case 0x1f801128:
            hard = PCSX::g_emulator.m_psxCounters->psxRcntRtarget(2);
            PSXHW_LOG("T2 target read32: %x\n", hard);
            return hard;
        case 0x1f801014:
            hard = psxHu32(add);
            PSXHW_LOG("SPU delay [0x1014] read32: %8.8lx\n", hard);
            return hard;

        default:
            hard = psxHu32(add);
            PSXHW_LOG("*Unknown 32bit read at address %x (0x%8.8lx)\n", add, hard);
            return hard;
    }
    PSXHW_LOG("*Known 32bit read at address %x\n", add);
    return hard;
}

void PCSX::HW::psxHwWrite8(uint32_t add, uint8_t value) {
    psxHu8ref(add) = value;
    PSXHW_LOG("*Unknown 8bit write at address %x value %x\n", add, value);
}

void PCSX::HW::psxHwWrite16(uint32_t add, uint16_t value) {
    switch (add) {
        case 0x1f801070:
            PSXHW_LOG("IREG 16bit write %x\n", value);
            if (PCSX::g_emulator.settings.get<PCSX::Emulator::SettingSioIrq>()) psxHu16ref(0x1070) |= SWAP_LEu16(0x80);
            if (PCSX::g_emulator.settings.get<PCSX::Emulator::SettingSpuIrq>()) psxHu16ref(0x1070) |= SWAP_LEu16(0x200);
            psxHu16ref(0x1070) &= SWAP_LEu16(value);
            return;

        case 0x1f801074:
            PSXHW_LOG("IMASK 16bit write %x\n", value);
            psxHu16ref(0x1074) = SWAP_LEu16(value);
            return;

        case 0x1f801100:
            PSXHW_LOG("COUNTER 0 COUNT 16bit write %x\n", value);
            PCSX::g_emulator.m_psxCounters->psxRcntWcount(0, value);
            return;
        case 0x1f801104:
            PSXHW_LOG("COUNTER 0 MODE 16bit write %x\n", value);
            PCSX::g_emulator.m_psxCounters->psxRcntWmode(0, value);
            return;
        case 0x1f801108:
            PSXHW_LOG("COUNTER 0 TARGET 16bit write %x\n", value);
            PCSX::g_emulator.m_psxCounters->psxRcntWtarget(0, value);
            return;

        case 0x1f801110:
            PSXHW_LOG("COUNTER 1 COUNT 16bit write %x\n", value);
            PCSX::g_emulator.m_psxCounters->psxRcntWcount(1, value);
            return;
        case 0x1f801114:
            PSXHW_LOG("COUNTER 1 MODE 16bit write %x\n", value);
            PCSX::g_emulator.m_psxCounters->psxRcntWmode(1, value);
            return;
        case 0x1f801118:
            PSXHW_LOG("COUNTER 1 TARGET 16bit write %x\n", value);
            PCSX::g_emulator.m_psxCounters->psxRcntWtarget(1, value);
            return;

        case 0x1f801120:
            PSXHW_LOG("COUNTER 2 COUNT 16bit write %x\n", value);
            PCSX::g_emulator.m_psxCounters->psxRcntWcount(2, value);
            return;
        case 0x1f801124:
            PSXHW_LOG("COUNTER 2 MODE 16bit write %x\n", value);
            PCSX::g_emulator.m_psxCounters->psxRcntWmode(2, value);
            return;
        case 0x1f801128:
            PSXHW_LOG("COUNTER 2 TARGET 16bit write %x\n", value);
            PCSX::g_emulator.m_psxCounters->psxRcntWtarget(2, value);
            return;

        default:

            psxHu16ref(add) = SWAP_LEu16(value);
            PSXHW_LOG("*Unknown 16bit write at address %x value %x\n", add, value);
            return;
    }
    psxHu16ref(add) = SWAP_LEu16(value);
    PSXHW_LOG("*Known 16bit write at address %x value %x\n", add, value);
}

inline void PCSX::HW::psxDma0(uint32_t madr, uint32_t bcr, uint32_t chcr) {
}

inline void PCSX::HW::psxDma1(uint32_t madr, uint32_t bcr, uint32_t chcr) {
}

inline void PCSX::HW::psxDma2(uint32_t madr, uint32_t bcr, uint32_t chcr) {
}

inline void PCSX::HW::psxDma3(uint32_t madr, uint32_t bcr, uint32_t chcr) {
}

void PCSX::HW::psxHwWrite32(uint32_t add, uint32_t value) {
    switch (add) {
        case 0x1f801060:
            PSXHW_LOG("RAM size write %x\n", value);
            psxHu32ref(add) = SWAP_LEu32(value);
            return;  // Ram size
        case 0x1f801070:
            PSXHW_LOG("IREG 32bit write %x\n", value);
            if (PCSX::g_emulator.settings.get<PCSX::Emulator::SettingSioIrq>()) psxHu32ref(0x1070) |= SWAP_LEu32(0x80);
            if (PCSX::g_emulator.settings.get<PCSX::Emulator::SettingSpuIrq>()) psxHu32ref(0x1070) |= SWAP_LEu32(0x200);
            psxHu32ref(0x1070) &= SWAP_LEu32(value);
            return;
        case 0x1f801074:
            PSXHW_LOG("IMASK 32bit write %x\n", value);
            psxHu32ref(0x1074) = SWAP_LEu32(value);
            return;
        case 0x1f801080:
            PSXHW_LOG("DMA0 MADR 32bit write %x\n", value);
            HW_DMA0_MADR = SWAP_LEu32(value);
            return;  // DMA0 madr
        case 0x1f801084:
            PSXHW_LOG("DMA0 BCR 32bit write %x\n", value);
            HW_DMA0_BCR = SWAP_LEu32(value);
            return;  // DMA0 bcr
        case 0x1f801088:
            PSXHW_LOG("DMA0 CHCR 32bit write %x\n", value);
            return;
        case 0x1f801090:
            PSXHW_LOG("DMA1 MADR 32bit write %x\n", value);
            HW_DMA1_MADR = SWAP_LEu32(value);
            return;  // DMA1 madr
        case 0x1f801094:
            PSXHW_LOG("DMA1 BCR 32bit write %x\n", value);
            HW_DMA1_BCR = SWAP_LEu32(value);
            return;  // DMA1 bcr
        case 0x1f801098:
            PSXHW_LOG("DMA1 CHCR 32bit write %x\n", value);
            return;
        case 0x1f8010a0:
            PSXHW_LOG("DMA2 MADR 32bit write %x\n", value);
            HW_DMA2_MADR = SWAP_LEu32(value);
            return;  // DMA2 madr
        case 0x1f8010a4:
            PSXHW_LOG("DMA2 BCR 32bit write %x\n", value);
            HW_DMA2_BCR = SWAP_LEu32(value);
            return;  // DMA2 bcr
        case 0x1f8010a8:
            PSXHW_LOG("DMA2 CHCR 32bit write %x\n", value);
            /* A hack that makes Vampire Hunter D title screen visible,
             * but makes Tomb Raider II water effect to stay opaque
             * Root cause for this problem is that when DMA2 is issued
             * it is incompletele and still beign built by the game.
             * Maybe it is ready when some signal comes in or within given delay?
             */
            if (s_dmaGpuListHackEn && value == 0x00000401 && HW_DMA2_BCR == 0x0) {
                psxDma2(SWAP_LEu32(HW_DMA2_MADR), SWAP_LEu32(HW_DMA2_BCR), SWAP_LEu32(value));
                return;
            }
            if (PCSX::g_emulator.config().HackFix && HW_DMA2_CHCR == 0x1000401) s_dmaGpuListHackEn = true;
            return;
        case 0x1f8010b0:
            PSXHW_LOG("DMA3 MADR 32bit write %x\n", value);
            HW_DMA3_MADR = SWAP_LEu32(value);
            return;  // DMA3 madr
        case 0x1f8010b4:
            PSXHW_LOG("DMA3 BCR 32bit write %x\n", value);
            HW_DMA3_BCR = SWAP_LEu32(value);
            return;  // DMA3 bcr
        case 0x1f8010b8:
            PSXHW_LOG("DMA3 CHCR 32bit write %x\n", value);
            return;
        case 0x1f8010c0:
            PSXHW_LOG("DMA4 MADR 32bit write %x\n", value);
            HW_DMA4_MADR = SWAP_LEu32(value);
            return;  // DMA4 madr
        case 0x1f8010c4:
            PSXHW_LOG("DMA4 BCR 32bit write %x\n", value);
            HW_DMA4_BCR = SWAP_LEu32(value);
            return;  // DMA4 bcr
        case 0x1f8010c8:
            PSXHW_LOG("DMA4 CHCR 32bit write %x\n", value);
            return;

#if 0
        case 0x1f8010d0: break; //DMA5write_madr();
        case 0x1f8010d4: break; //DMA5write_bcr();
        case 0x1f8010d8: break; //DMA5write_chcr(); // Not needed
#endif
        case 0x1f8010e0:
            PSXHW_LOG("DMA6 MADR 32bit write %x\n", value);
            HW_DMA6_MADR = SWAP_LEu32(value);
            return;  // DMA6 bcr
        case 0x1f8010e4:
            PSXHW_LOG("DMA6 BCR 32bit write %x\n", value);
            HW_DMA6_BCR = SWAP_LEu32(value);
            return;  // DMA6 bcr
        case 0x1f8010e8:
            PSXHW_LOG("DMA6 CHCR 32bit write %x\n", value);
            return;
        case 0x1f8010f0:
            PSXHW_LOG("DMA PCR 32bit write %x\n", value);
            HW_DMA_PCR = SWAP_LEu32(value);
            return;
        case 0x1f8010f4:
            PSXHW_LOG("DMA ICR 32bit write %x\n", value);
            {
                uint32_t tmp = (~value) & SWAP_LEu32(HW_DMA_ICR);
                HW_DMA_ICR = SWAP_LEu32(((tmp ^ value) & 0xffffff) ^ tmp);
                return;
            }
        case 0x1f801014:
            PSXHW_LOG("SPU delay [0x1014] write32: %8.8lx\n", value);
            psxHu32ref(add) = SWAP_LEu32(value);
            return;
        case 0x1f801810:
            PSXHW_LOG("GPU DATA 32bit write %x (CMD/MSB %x)\n", value, value >> 24);
            // 0x1F means irq request, so fulfill it here because plugin can't and won't
            // Probably no need to send this to plugin in first place...
            // MML/Tronbonne is known to use this.
            // TODO FIFO is not implemented properly so commands are not exact
            // and thus we rely on hack that counter/cdrom irqs are enabled at same time
            if (PCSX::g_emulator.config().HackFix && SWAP_LEu32(value) == 0x1f00000 && (psxHu32ref(0x1070) & 0x44)) {
                setIrq(0x01);
            }
            return;
        case 0x1f801814:
            PSXHW_LOG("GPU STATUS 32bit write %x\n", value);
            if (value & 0x8000000) s_dmaGpuListHackEn = false;
            return;

        case 0x1f801100:
            PSXHW_LOG("COUNTER 0 COUNT 32bit write %x\n", value);
            PCSX::g_emulator.m_psxCounters->psxRcntWcount(0, value & 0xffff);
            return;
        case 0x1f801104:
            PSXHW_LOG("COUNTER 0 MODE 32bit write %x\n", value);
            PCSX::g_emulator.m_psxCounters->psxRcntWmode(0, value);
            return;
        case 0x1f801108:
            PSXHW_LOG("COUNTER 0 TARGET 32bit write %x\n", value);
            PCSX::g_emulator.m_psxCounters->psxRcntWtarget(0, value & 0xffff);
            return;  //  HW_DMA_ICR&= SWAP_LE32((~value)&0xff000000);
        case 0x1f801110:
            PSXHW_LOG("COUNTER 1 COUNT 32bit write %x\n", value);
            PCSX::g_emulator.m_psxCounters->psxRcntWcount(1, value & 0xffff);
            return;
        case 0x1f801114:
            PSXHW_LOG("COUNTER 1 MODE 32bit write %x\n", value);
            PCSX::g_emulator.m_psxCounters->psxRcntWmode(1, value);
            return;
        case 0x1f801118:
            PSXHW_LOG("COUNTER 1 TARGET 32bit write %x\n", value);
            PCSX::g_emulator.m_psxCounters->psxRcntWtarget(1, value & 0xffff);
            return;
        case 0x1f801120:
            PSXHW_LOG("COUNTER 2 COUNT 32bit write %x\n", value);
            PCSX::g_emulator.m_psxCounters->psxRcntWcount(2, value & 0xffff);
            return;
        case 0x1f801124:
            PSXHW_LOG("COUNTER 2 MODE 32bit write %x\n", value);
            PCSX::g_emulator.m_psxCounters->psxRcntWmode(2, value);
            return;
        case 0x1f801128:
            PSXHW_LOG("COUNTER 2 TARGET 32bit write %x\n", value);
            PCSX::g_emulator.m_psxCounters->psxRcntWtarget(2, value & 0xffff);
            return;
        default:
            psxHu32ref(add) = SWAP_LEu32(value);
            PSXHW_LOG("*Unknown 32bit write at address %x value %x\n", add, value);
            return;
    }
    psxHu32ref(add) = SWAP_LEu32(value);
    PSXHW_LOG("*Known 32bit write at address %x value %x\n", add, value);
}
