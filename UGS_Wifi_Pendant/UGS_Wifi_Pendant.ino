/* Set these to your desired credentials. */
const char *ssid = "***************";  //ENTER YOUR WIFI SETTINGS
const char *password = "***********";


/* Set two webserver local network ips for CNC. For laptop and main cnc machine. */
#define controler1Name "PC1" //PC 1 name
#define webserver1 "xxx.xxx.xxx.xxx" //PC 1 local ip

#define controler2Name "PC2" //PC 2 name
#define webserver2 "xxx.xxx.xxx.xxx" //PC 2 local ip

const String webserverport = "8080";


/* CNC timing, rate, step and other settings */
#define xyStepSizeFast 800 //x and y step on fast cycle (z step is 1/10 of that) - Default value: 800 mm
#define xyStepRateFast 5000 //x and y rate on fast cycle (z rate is 1/10 of that) - Default value: 5000 mm/min
#define xyStepSizeSlow 200 //x and y step size on slow cycle (z step is 1/10 of that) - Default value: 200 mm
#define xyStepRateSlow 500 //x and y rate on slow cycle (z rate is 1/10 of that) - Default value: 500 mm/min
#define probeStep 20 //20 mm step when probing Z axis - Default value: 20 mm
#define probeRate 50 //50mm / min feed rate for probing Z - Default value: 50 mm/min
#define drillProbeHeight 2 //Height of a probe (mm) - Default value: 2 mm
#define enableLaserMode true //if true, this will enable toggling laser mode
#define laserProbeHeight 0 //If you probe Z axis while in laser mode, you might use the different probe (for set laser height). This is offset of this probe height. Default value: 0 mm
#define skipGetDelay 50 //when button is pressed, no other button will register after skipGetDelay milliseconds - Default value: 50 ms
#define skipMsgDelay 1000 //when button is pressed, display what happened and after skipMsgDelay milliseconds display status - Default value: 1000
#define turnBacklightDelay 600000 //turn off LCD backlight after this many milliseconds of inactivity - Default value: 600000 ms
#define jumpSign 5000 //jump sign after 5000 ms (cycling data when idle or running) - Default value: 5000 ms
#define autoResetAlarm false //if you want for pendant to automatically send $X when idle and alarm was triggered (not Hard Limit alarm) set true, else it will not - Default value: false
#define autoResetStartupAlarm true //automatically send $X on startup to remove general alarm - Default value: true
#define enableResetPhyPin false //true to enable hardware reset pin to arduino. Caution: if esp reboots it may reset arduino! Default value: false
#define enableSerialOutput true //disable if not needed. Will make code run a small bit faster. Default value: true

// Language //
// Uncomment language you want and comment the unwanted one
#define LanguageEnglish
//#define LanguageSlovenian


//if you encounter errors when compiling use libraries located in libraries folder and move them to arduino libraries folder



/*  LCD connections
 * 3V - VCC
 * GND - GND
 * D1 - SCL
 * D2 - SDA
 */

//D3 - led for laser mode status (on if lasermode is on)
//D5 - led for speed mode status (on if slow)


/*
   PCB buttons

   button[0] - Homing / Probe
   button[1] - Reset XYZ / Reset Z zero
   button[2] - Shift, if slow toggle is 1, and laser mode is 1 shift turns on laser test
   button[3] - Pause-Resume / Laser mode (toggle laser mode if enableLaserMode is enabled) OR toggle Check mode
   button[4] - Cancel (or $X on idle) / Soft reset + $X
   button[5] - Y-
   button[6] - Y+
   button[7] - X-
   button[8] - X+
   button[9] - Z-
   button[10] - Z+
   button[11] - slow/fast toggle for jogging
   button[12] - N/A
   button[13] - N/A
   button[14] - N/A
   button[15] - PC1, PC2 toggle
 */


/*
   To do: fix soft reset when universal gcode sender starts supporting this


 */
