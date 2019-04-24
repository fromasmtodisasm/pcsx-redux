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

#include <chrono>

#include "core/psxemulator.h"
#include "core/r3000a.h"

#include "main/settings.h"

class SystemImpl : public PCSX::System {
    virtual void printf(const char *fmt, ...) final {
        // print message to debugging console
        va_list a;
        va_start(a, fmt);
        vprintf(fmt, a);
        va_end(a);
    }

    virtual void biosPrintf(const char *fmt, ...) final {
        // print message to debugging console
        va_list a;
        va_start(a, fmt);
        vprintf(fmt, a);
        va_end(a);
    }

    virtual void vbiosPrintf(const char *fmt, va_list a) final {
        vprintf(fmt, a);
    }

    virtual void message(const char *fmt, ...) final {
        va_list a;
        va_start(a, fmt);
        vprintf(fmt, a);
        va_end(a);
    }

    virtual void log(const char *facility, const char *fmt, va_list a) final {
        vprintf(fmt, a);
    }

    virtual void update() final {
        // called on vblank to update states
    }

    virtual void runGui() final {
        // called when the UI needs to show up
    }

    virtual void softReset() final {
        // debugger or UI is requesting a reset
    }

    virtual void hardReset() final {
        // debugger or UI is requesting a reset
    }

    virtual void close() final {
        // emulator is requesting a shutdown of the emulation
    }

    bool m_enableStdout = true;
};

int main(int argc, char **argv) {
    SystemImpl *system = new SystemImpl;
    PCSX::g_system = system;

    PCSX::g_emulator.settings.get<PCSX::Emulator::SettingBios>().value = "bios.bin";
    PCSX::g_emulator.settings.get<PCSX::Emulator::SettingHLE>().value = false;
    auto start = std::chrono::high_resolution_clock::now();
    PCSX::g_emulator.EmuInit();
    auto init = std::chrono::high_resolution_clock::now();
    PCSX::g_emulator.EmuReset();
    auto finish = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> initDuration = init - start;
    std::chrono::duration<double> resetDuration = finish - init;
    std::chrono::duration<double> totalDuration = finish - start;

    printf("init: %g seconds\nreset: %g seconds\ntotal: %g seconds\n", initDuration.count(), resetDuration.count(), totalDuration.count());

    return 0;
}
