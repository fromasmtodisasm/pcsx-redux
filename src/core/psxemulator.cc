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

#include "core/psxemulator.h"
#include "core/psxbios.h"
#include "core/psxhw.h"
#include "core/r3000a.h"

PCSX::Emulator::Emulator()
    : m_psxMem(new PCSX::Memory())
    , m_psxCounters(new PCSX::Counters())
    , m_psxBios(PCSX::Bios::factory())
    , m_hw(new PCSX::HW())
{}

PCSX::Emulator::~Emulator() { }

int PCSX::Emulator::EmuInit() {
    assert(g_system);
    if (m_psxMem->psxMemInit() == -1) return -1;
    int ret = PCSX::R3000Acpu::psxInit();
    return ret;
}

void PCSX::Emulator::EmuReset() {
    m_psxMem->psxMemReset();
    m_psxCpu->psxReset();
}

void PCSX::Emulator::EmuShutdown() {
    m_psxMem->psxMemShutdown();
    m_psxCpu->psxShutdown();
}

void PCSX::Emulator::EmuUpdate() {}

PCSX::Emulator& PCSX::g_emulator = PCSX::Emulator::getEmulator();
