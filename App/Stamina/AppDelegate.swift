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

import Cocoa

@NSApplicationMain
class AppDelegate: NSObject, NSApplicationDelegate {
    var iconOff: NSImage!
    var iconOn: NSImage!
    var statusItem: NSStatusItem!

    func applicationDidFinishLaunching(_ aNotification: Notification) {
        menuBarInit()
        checkKextAvailable()
        restoreFromDefaults()
    }

    func applicationWillTerminate(_ aNotification: Notification) {
    }

    func menuBarInit() {
        iconOff = NSImage(named: NSImage.Name(rawValue: "turbo-off"))
        iconOff.isTemplate = true

        iconOn = NSImage(named: NSImage.Name(rawValue: "turbo-on"))
        iconOn.isTemplate = true

        statusItem = NSStatusBar.system.statusItem(withLength: -2)
        statusItem.action = #selector(AppDelegate.toggleTurboBoost)
        statusItem.highlightMode = true
    }

    @objc func toggleTurboBoost() {
        if statusItem.image == iconOff {
            set_turbo_boost_enabled(true)
            statusItem.image = iconOn
        } else {
            set_turbo_boost_enabled(false)
            statusItem.image = iconOff
        }
    }

    func checkKextAvailable() {
        if !check_turbo_boost_control_available() {
            fatalError("TurboBoostControl kext must be loaded and accessible")
        }
    }

    func restoreFromDefaults() {
        let enabled = Defaults().enableTurboBoost
        statusItem.image = enabled ? iconOn : iconOff
        set_turbo_boost_enabled(enabled)
    }
}
