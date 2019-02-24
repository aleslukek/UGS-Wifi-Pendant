# UGS-Wifi-Pendant
This is an addon for an [Universal Gcode Sender](https://github.com/winder/Universal-G-Code-Sender). It enables a bit more complicated control of your CNC as well as LCD output of useful info.


## What it does?
It takes buttons input and translates it to web requests that commands UGS via it's wifi pendant. You can move CNC (x, y z), slow move, start home cycle, z probe, reset XYZ zero, reset Z zero, return to XY zero, return to Z zero, pause, start job, cancel, toggle laser mode, soft reset (not fully working yet, will get around to fix this), toggle source computer.

When the job is done it can send a notification via IFTTT.com.

OTA updates via local network (tested via Arduino IDE)!

It uses LCD to output CNC data, such as machine position, work position, job duration, job time remaining, job sent rows, total rows, progress, file selected, errors and things like that.

When job is running most of the buttons are disabled. Only pause, cancel and toggles are working. So you can't accidentally send commands that have nothing to do with job running.



## What buttons do?

* Button 0 - Homing / Z Probe
* Button 1 - Reset XYZ Zero / Reset Z zero
* Button 2 - Shift, if slow toggle is 1, and laser mode is 1 shift turns on laser test
* Button 3 - Pause-Resume / Laser mode (toggle laser mode if enableLaserMode is enabled) OR toggle Check mode
* Button 4 - Cancel (or $X on idle) / Soft reset + $X
* Button 5 - Y- / Return to XY zero
* Button 6 - Y+ / Return to XY zero
* Button 7 - X- / Return to XY zero
* Button 8 - X+ / Return to XY zero
* Button 9 - Z- / Return to Z zero
* Button 10 - Z+ / Return to Z zero
* Button 11 - Slow/fast toggle for jogging
* Button 12 - N/A
* Button 13 - N/A
* Button 14 - N/A
* Button 15 - PC1, PC2 toggle

--

* Buttons 5 - 10: jogging buttons. They work in two ways: press and hold a button will start jogging. When you release a button it will stop the movement. Second way is a quick press on a button it will start moving until the full step size is reached or until you press cancel or any jog button again.
* Button 2 acts as shift. Example: If you press button 0 CNC starts homing cycle. If you press button 2 (shift) + button 0 then it starts probing.  
* Button 3 start job if file selected when CNC is idle and pauses job when CNC is running.
* Button 2 + Button 3 (shift + pause) toggles laser mode $32=1 and $32=0. This pendant is unaware of current CNC laser mode. So if you press those two buttons even if laser mode is on it will turn it on not off. And vice versa. When laser mode is on led 1 is on.
* Button 4 cancels running job or jog, but acts as $X when idle. Soft reset currently doesn't work if hard limit switches are triggered. This is UGS problem that needs addressing not mine.
Button 11 is as toggle. When not pressed CNC jogs with higher feed rate. When toggle is pressed, led 2 turns on and jogging is slower.
* Button 2 + any X/Y button returns to XY work zero (G90 G0 X0 Y0). This action observes slow/quick toggle and acts accordingly.
* Button 2 + any Z button returns to Z work zero (G90 G0 Z0). This action observes slow/quick toggle and acts accordingly.
* When laser mode is on ($32=1) and slow toggle is also on (so both LEDs are on) button 2 acts as laser test fire. It fires laser at slowest power. Be aware that if you have spindle connected to Spindle enable pin on grbl Arduino it will turn on your spindle. That might be much more dangerous than laser on lowest power. But lasers are dangerous as well, don't get me wrong. USE AT YOUR OWN RISK!



## What it needs?

  * ESP8266
  * At least two shift in registers 74HC165N
  * LM324N Op amp -> this could help with signalling 0V when button not pressed and +V when button pressed
  * I2C LCD 16*2
  * Buttons - momentary 5x
  * Moar buttons - rocker switch on-off-on 3x
  * Toggle buttons/switches 2x
  * Trimmer 100k or 10k
  * Screw terminal
  * 22uF capacitors 16x
  * 1000k resistors 16x
  * 2 LEDs


* Be mindful that button must be connected in such a way that signals 0V when not pressed and +V (3.3V) when pressed. If you only send +V when button is pressed and not 0V when not pressed it will trigger shift registers.

## IFTTT notification

To enable IFTTT notification you must find your IFTTT Key found [somewhere here](https://ifttt.com/maker_webhooks) and assign it to "iftttKey" variable. Also at this point you must make an event (trigger) with event name. IFTTT event name must match to a variable "iftttEventName" found in UGS_Wifi_Pendant.ino. I would recommend IFTTT notification action to be: "CNC event on {{OccurredAt}}. Message: {{Value1}}". Value1 is an event message.

IFTTT notification happens when job is aborted by an error (hard limit or general error) or when CNC goes to sleep mode after a job - if a gcode file has $SLP at the end.
Canceling and pausing a job wont trigger a notification (since you are probably standing next to it). Also if a job does not have a $SLP at the end of a gcode file this will not send a notification. You should always use $SLP when doing a job when you are not around.


## How to compile...

Obviously you'll need Arduino IDE or something that sends code to your ESP8266.
Then you'll have to set some things up, really simply. You don't need to change any other code other than the one in UGS_Wifi_Pendant.ino

* Wifi settings, self-explanatory. Just enter ssid and wifi password.

* If you have CNC dedicated pc (or Raspberry pi) but also use laptop occasionally to control CNC you can use toggle to switch between them. I highly recommend to set static network ip for those devices on your local router. Name pc 1 and give it local ip. Also for pc 2. Or a mac or anything else that runs UGS. Server port is set to default to 8080 for both devices. I don't see a need for different ports on different devices. So it's the same one for both of them.

        controler1Name "PC1" //PC 1 name
        webserver1 "xxx.xxx.xxx.xxx" //PC 1 local ip
        controler2Name "PC2" //PC 2 name
        webserver2 "xxx.xxx.xxx.xxx" //PC 2 local ip
        webserverport = "8080";


        iftttKey - your IFTTT Webhook key
        iftttEventName - ifttt event name. MUST match the one on IFTTT website.



#### Personal settings that make anything working better.
         xyStepSizeFast 800  - x and y step size for quick jog. Might be as long as you want.
         xyStepRateFast 5000 - x and y feed rate for quick jog
         xyStepSizeSlow 200 - x and y step size for slow jog
         xyStepRateSlow 500 - x and y feed rate for slow jog
         probeStep 20 - probing Z axis step size
         probeRate 50 - probing Z axis feed rate
         drillProbeHeight 2 - height of a probe. When it reaches a probe it will offset Z axis for this amount of mm
         enableLaserMode true - if you also use laser keep this enabled. If not it won't use any of those laser features
         laserProbeHeight 0 - probing Z axis probe offset. Same function as drillProbeHeight but different offset if you need
         skipGetDelay 50 - when button is pressed, no other button will register after skipGetDelay milliseconds
         skipMsgDelay 1000 - when button is pressed, display what happened and after skipMsgDelay milliseconds display regular status
         turnBacklightDelay 600000 - turn off LCD backlight after this many milliseconds of inactivity
         jumpSign 5000 - jump sign after 5000 ms (cycling data when idle or running)
         autoResetAlarm false - if you want for pendant to automatically send $X when idle and alarm was triggered (not Hard Limit alarm) set true, else it will not
         autoResetStartupAlarm true - automatically send $X on startup to remove general alarm
         enableSerialOutput true - Enables serial output. For debugging purposes. Disable if not needed. Will make code run a small bit faster.
         enableIftttMessaging true - Enables IFTTT notifications.

         enableOTAUpdate true - enables Over The Air (over local wifi) firmware updates.
         enableOTAPassword true - enables the need for password when OTA updating.
         OTAPassword "******" - if enableOTAUpdate is enabled, enter a password you will have to enter when OTA updating next time. This password is printed on fresh boot via serial.


* LCD language. Uncomment the wanted one and comment the unwanted one. Currently English and Slovenian. If you'd like to contribute or change, language is in file e_lcd_lang.ino
#define LanguageEnglish
//#define LanguageSlovenian


* If you encounter errors when compiling use libraries located in libraries folder and move them to arduino libraries folder


* This code checks for buttons state and button logic more than 1000 times per second. When state of any button changes it sends a request. It is done fairly quickly and I haven't encountered any lags or delays. LCD is updated almost every half a second or so. So everything works quite tightly.


# Connections
 * 3V - LCD VCC
 * GND - LCD GND
 * D1 - LCD SCL
 * D2 - LCD SDA
 * D3 - led for laser mode status (on if laser mode is on)
 * D5 - led for speed mode status (on if slow)

Please refer to schematic in schematic folder for if you'd like to make a PCB for it. I made it, trust me, it is worth it. Soldering this is a path you don't want to go to. I used https://easyeda.com to design the schematic (you can easily import it in easyeda. I haven't tested other schematic apps) and pcb and https://jlcpcb.com/ to manufacture 10 pieces for 2 USD + shipping.



# Known issues
* Soft reset won't work until UGS starts to support soft reset via get request.
* Laser test fire doesn't fire consistently. When it does it might freeze grbl. But sometimes it does not. Not because get requests are not getting through but for some (so far to me) unknown reason, I assume connected to grbl.
* Very rarely jogging stops responding. It start moving and then it doesn't stop. Cancel button doesn't work, it jogs until it hits hard limit switch. For that reason I would strongly recommend to ONLY use this pendant on machines that have hard limits working in all directions. For safety reason of course. I haven't pinpointed the issue to either wifi pendant, ugs, grbl or esp pendant.


# WARNING
Before embarking on this project READ THIS README! And then again and again. Then think before you do anything that can endanger personal safety or cause damage. Make sure you understand how it works before start using it!

Use this UGS_Wifi_Pendant **ONLY** on machines that have hard limit switches in **ALL** directions! If things start moving and you for some reason don't or can't press stop button at least have limit switches that stop everything.

Read and understand what buttons and combination of buttons do! Failing to understand this might endanger your personal safety and might cause damage to your machine. And other things.

Wear your safety glasses and ear mufflers.

**AND ALWAYS KEEP YOUR HANDS AND OTHER EQUIPMENT OFF YOUR CNC WHEN USING IT!**
