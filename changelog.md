#1.05 - 24.2.2019
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
