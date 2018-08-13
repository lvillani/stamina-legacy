// Stamina - Low power mode for Macs
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

#include "turboboostcontrol.h"

#include <stdbool.h>
#include <stdio.h>
#include <sys/sysctl.h>
#include <sys/types.h>

static const char* SYSCTL_NAME = "debug.turboboost";

bool check_turbo_boost_control_available(void)
{
    if (!is_turbo_boost_control_available()) {
        perror("TurboBoostControl is not available");
        return false;
    }
    
    printf("TurboBoostControl is available\n");
    return true;
}

bool is_turbo_boost_control_available(void)
{
    int ret = sysctlbyname(SYSCTL_NAME, NULL, 0, NULL, 0);
    if (ret != 0) {
        return false;
    }

    // We may be able to read but not write (e.g. due to sandboxing).
    return set_turbo_boost_enabled(is_turbo_boost_enabled());
}

bool is_turbo_boost_enabled(void)
{
    int ret = 0;
    size_t ret_size = sizeof(ret);

    if (sysctlbyname(SYSCTL_NAME, &ret, &ret_size, NULL, 0) != 0) {
        return false;
    }

    return (bool) ret;
}

bool set_turbo_boost_enabled(bool enabled)
{
    int val = (int) enabled;
    size_t val_size = sizeof(val);
    return sysctlbyname(SYSCTL_NAME, NULL, 0, &val, val_size) == 0;
}
