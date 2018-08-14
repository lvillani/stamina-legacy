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

import Foundation

class Defaults {
    private let enableTurboBoostKey = "EnableTurboBoost"

    public var enableTurboBoost: Bool {
        get {
            if let val = UserDefaults().dictionaryRepresentation()[enableTurboBoostKey] {
                switch val {
                case let boolVal as Bool:
                    return boolVal
                default:
                    return true
                }
            } else {
                return true
            }
        }

        set {
            UserDefaults().set(self, forKey: enableTurboBoostKey)
        }
    }
}
