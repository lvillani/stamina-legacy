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

#pragma once

#include <stdbool.h>

/*!
 * \brief Prints diagnostic information about TurboBoostControl kernel extension.
 *
 * Checks that TurboBoostControl sysctl interface is usable and prints diagnostic info to stderr (in
 * case of errors) or stdout (if successful). This is mainly used to diagnose problems during
 * development.
 *
 * \return true if we can read and write TurboBoostControl's sysctl key, false otherwise.
 */
bool check_turbo_boost_control_available(void);

/*!
 * \brief Checks that we can access TurboBoostControl sysctl interface.
 *
 * \return true if we can read and write TurboBoostControl's sysctl key, false otherwise.
 */
bool is_turbo_boost_control_available(void);

/*!
 * \brief Returns whether Turbo Boost is enabled.
 *
 * \return true if we have access to TurboBoostControl's sysctl interface (see also \a
 *      is_turbo_boost_control_available()) and Turbo Boost enabled, false otherwise.
 */
bool is_turbo_boost_enabled(void);

/*!
 * \brief Enables or disables Turbo Boost.
 *
 * \arg \c enabled whether to enable or disable Turbo Boost.
 * \return true if the request was successful, false otherwise.
 */
bool set_turbo_boost_enabled(bool enabled);
