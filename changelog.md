# 1.21 - 31.7.2019
* Small bug fixes.
* Removed chosen file name on LCD.
* Showing progress % was accidentally removed in previous build. Now it works.

# 1.20 - 8.7.2019
* This is a big one, guys! Added support for New Mr. Breiler's Pendant via Wifi.
* Also! Added support for New Pendant, BUT VIA USB! So This Wifi Pendant takes data via wifi, still has IFTTT messaging but commands are sent via USB cable. Since ESP cannot act as keyboard I managed a bodge - Powershell script (serialToUGS.ps1) that runs in a back that listens Wifi Pendant COM port and converts Serial Commands to localhost web requests. So Wifi Pendant should still run in a background. Since this are localhost requests commands are quicker, more reliable even if Pendant doesn't have the best wifi signal.
* Old pendant is as is, now Json Parsing is done by ArduinoJson 6 library. Better than my previous parser.
* Soft reset now works on new pendant.
* Some small bugs removed.
* Via this new pendant it is now possible that Wifi pendant automatically connects UGS to CNC. And cancels first Alarm.
* I couldn't be bothered but I assume that by using powershell you could remove the need for wifi (ESP) - so almost any Arduino could be used. I won't check since I have other things to do.

# 1.05 - 24.2.2019
* Added OTA updates. You can now update your module wirelessly. You can enable or disable OTA updates. You can also enable or disable OTA password.
* Changed how progress is being calculated. So it better handles empty rows and comments in gcode file.
* Changed how estimated remaining time is being calculated; it should not jump around like in UGS does. And it is more accurate. And it works by applying magic.

# 1.04 - 16.2.2019
* Added a notification system; it sends notifications to your phone via IFTTT.com when job is finished/aborted.
* Introduced small delay (20ms) to cancel command in cases when cancel command is requested right after jog command. This cancel command overrules 50 ms delay to all get requests.
* Some readme updates.
* Fixed speed issue when "return to zero" was not observing rate toggle (button 11).

# 1.03 - 13.2.2019
* Bugfix, added G91 to z probe. Sometimes Z probe was probing in wrong direction, depending on Z zero.

# 1.02 - 13.2.2019
* Some small code cleanup.

# 1.01 - 3.2.2019
* Added "return to zero" function. It splits XY return to zero and Z return to zero. Shift + direction buttons. Shift + X or Shift + Y to return to XY zero and Shift + Z to return to Z zero. It observes slow/quick toggle switch position for quick or slow return.

# 1.0
* Initial release.
