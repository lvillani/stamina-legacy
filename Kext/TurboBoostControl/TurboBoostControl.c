// TurboBoostControl - Allows userspace programs to enable/disable Turbo Boost
// Copyright (C) 2018 Lorenzo Villani
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, version 3 of the License.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include <mach/mach_types.h>
#include <sys/sysctl.h>
#include <sys/systm.h>
#include <sys/types.h>

//
// Turbo Boost
//
// Turbo Boost is the marketing name for what Intel formally calls "Opportunistic Processor
// Performance Operation". It can be controlled by flipping bit 38 of the IA32_MISC_ENABLE (0x1a0)
// MSR register.
//
// Bit values:
//
// - 1: Turbo Boost disabled
// - 0: Turbo Boost enabled
//
// References:
//
// - Intel® 64 and IA-32 Architectures Software Developer’s Manual
//   Volume 3B: System Programming Guide, Part 2
//   https://www.intel.com/content/dam/www/public/us/en/documents/manuals/64-ia-32-architectures-software-developer-vol-3b-part-2-manual.pdf
//

//
// MSR
//

static const uint32_t IA32_MISC_ENABLE = 0x1a0;

static inline uint64_t rdmsr()
{
    uint32_t edx = 0;
    uint32_t eax = 0;
    asm volatile("rdmsr"
                 : "=d"(edx), "=a"(eax)
                 : "c"(IA32_MISC_ENABLE));

    return ((uint64_t)edx << 32) | eax;
}

static inline void wrmsr(uint64_t value)
{
    uint32_t edx = value >> 32;
    uint32_t eax = (uint32_t)value;
    asm volatile("wrmsr"
                 :
                 : "c"(IA32_MISC_ENABLE), "d"(edx), "a"(eax));
}

//
// Turbo Boost
//

static const uint64_t TURBO_BOOST = (uint64_t)1 << 38;

static bool is_turbo_boost_enabled()
{
    return !(rdmsr() & TURBO_BOOST);
}

static void set_turbo_boost_enabled(bool enabled)
{
    if (enabled) {
        wrmsr(rdmsr() ^ TURBO_BOOST);
    } else {
        wrmsr(rdmsr() | TURBO_BOOST);
    }
}

//
// Sysctl
//

static bool TURBO_BOOST_STATUS = 0;

static int sysctl_turbo_boost_status SYSCTL_HANDLER_ARGS
{
    int error = sysctl_handle_int(oidp, oidp->oid_arg1, oidp->oid_arg2, req);

    if (!error && req->newptr) {
        // Write (TURBO_BOOST_STATUS is already set)
        set_turbo_boost_enabled(TURBO_BOOST_STATUS);
    } else {
        // Read
        TURBO_BOOST_STATUS = is_turbo_boost_enabled();
        error = SYSCTL_OUT(req, &TURBO_BOOST_STATUS, sizeof TURBO_BOOST_STATUS);
    }

    return error;
}

SYSCTL_PROC(_debug, OID_AUTO, turboboost, CTLTYPE_INT | CTLFLAG_RW | CTLFLAG_ANYBODY,
    &TURBO_BOOST_STATUS, 0, &sysctl_turbo_boost_status, "I", "Turbo Boost Control");

//
// Module
//

kern_return_t TurboBoostControl_start(kmod_info_t* ki, void* d);
kern_return_t TurboBoostControl_stop(kmod_info_t* ki, void* d);

kern_return_t TurboBoostControl_start(kmod_info_t* ki, void* d)
{
    sysctl_register_oid(&sysctl__debug_turboboost);
    return KERN_SUCCESS;
}

kern_return_t TurboBoostControl_stop(kmod_info_t* ki, void* d)
{
    sysctl_unregister_oid(&sysctl__debug_turboboost);
    return KERN_SUCCESS;
}
