build:
	( cd App && xcodebuild )
	( cd Kext && xcodebuild )

install: build
	rm -rf /Applications/Stamina.app
	cp -r App/build/Release/Stamina.app /Applications/
	sudo install -o root -g wheel -m 0644 App/Stamina/LaunchAgent.plist /Library/LaunchAgents/me.villani.lorenzo.Stamina.plist

	sudo rm -rf /Library/Extensions/TurboBoostControl.kext
	sudo cp -r Kext/build/Release/TurboBoostControl.kext /Library/Extensions/
	sudo chown -R root:wheel /Library/Extensions/TurboBoostControl.kext
	sudo chmod 0755 /Library/Extensions/TurboBoostControl.kext
	sudo install -o root -g wheel -m 0644 Kext/TurboBoostControl/LaunchDaemon.plist /Library/LaunchDaemons/me.villani.lorenzo.TurboBoostControl.plist

reload: install
	sudo kextunload /Library/Extensions/TurboBoostControl.kext || true
	sudo launchctl unload /Library/LaunchDaemons/me.villani.lorenzo.TurboBoostControl.plist || true
	sudo launchctl load /Library/LaunchDaemons/me.villani.lorenzo.TurboBoostControl.plist

.PHONY: build install reload
