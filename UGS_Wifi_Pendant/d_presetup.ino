#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266Ping.h>
#include <LiquidCrystal_I2C.h>

#define  pendatVersion  1.02

LiquidCrystal_I2C lcd(0x3F, 16, 2);
#define resetPinD5 16 //D0

#define ledLaser 0 //D3 - led for laser mode status (on if lasermode is on)
#define ledSlow  14//D5 - led for speed mode status (on if slow)


bool justSentGeneral = 0;
bool justSentDirection = 0;
bool justSentMsgDisplay = 0;
bool ledLaserStatus = 0;
bool ledSlowStatus = 0;
int buttonPressMsg = 0; //what message to display when certain button is pressed
unsigned long timer1 = 0; //reset justSentGeneral to 0 after skipGetDelay ms
unsigned long timer2 = 0; //turn off backlight after turnBacklightDelay ms
unsigned long timer3 = 0; //calculate cycle duration
unsigned long timer4 = 0; //calculate cycle ugs get and parse duration
unsigned long timer5 = 0; //reset msg display after skipMsgDelay ms
unsigned int cycleCounter = 1;
unsigned int timerlcd = 0;
bool disableAlarmLockLogic = 1; //will send $X on startup if enabled. Also there to automatically send $X if alarm is set. Not for Hard Limit alarm
bool resetZAxisAfterProbe = 0;
bool turnOnLCD = 0;
bool lcdBacklightStatus = 0;
byte showLCD = 0;
bool laserMode = 0;
bool laserTest = 0;
bool cnc_state[15] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};


/*

   CNC_State index:
   0 - esp wifi not connected
   1 - PC not available
   2 - PC available ugs or pendant not running
   3 - PC available, ugs available, arduino (cnc) connected to PC via serial
   4 - CNC is idle
   5 - CNC job is running
   6 - CNC job is on hold
   7 - CNC is jogging
   8 - Hard Limit Alarm triggered
   9 - General Alarm triggered
   10 - CNC is sleeping ($SLP triggered)
   11 - send btn enabled
   12 - pause btn enabled
   13 - cancel btn enabled
   14 - disable certain buttons while job is being done
 */

#define varLength 17
char textArray[500];
char variable[18][varLength];
String variableNames[17];

float workx, worky, workz, machinex, machiney, machinez;
float rowProgress;
int rowsInFile, sentRows, remainingRows, timeRemaining, duration;

bool button[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
bool prevButton[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
unsigned int buttonValues;
unsigned int oldButtonValues;

/*
   button[0] - top left button, Homing / Probe
   button[1] - top right button, Reset XYZ / Reset Z zero
   button[2] - middle button, Shift, if slow toggle is 1, and laser mode is 1 shift turns on laser test
   button[3] - bottom left button, Pause-Resume / Laser mode (toggle laser mode)
   button[4] - bottom right button, Cancel (or $X on idle) / Soft reset + $X
   button[5] - Y- / return to XY zero
   button[6] - Y+ / return to XY zero
   button[7] - X- / return to XY zero
   button[8] - X+ / return to XY zero
   button[9] - Z- / return to Z zero
   button[10] - Z+ / return to Z zero
   button[11] - slow/fast toggle for jogging
   button[12] - N/A
   button[13] - N/A
   button[14] - N/A
   button[15] - PC 1, PC 2 toggle
 */

// How many shift register chips are daisy-chained.
#define NUMBER_OF_SHIFT_CHIPS   2

// Width of data (how many ext lines).
#define DATA_WIDTH   NUMBER_OF_SHIFT_CHIPS * 8

// Width of pulse to trigger the shift register to read and latch.
#define PULSE_WIDTH_USEC   5

#define ploadPin        13 // Connects to Parallel load pin the 165
#define clockEnablePin  2 // Connects to Clock Enable pin the 165
#define dataPin         12 // Connects to the Q7 pin the 165
#define clockPin        15 // Connects to the Clock pin the 165

#define   GETmovexyz                "/sendGcode/?gCode=%24J%3DG21G91"
#define   GETresetzero              "/sendGcode/?gCode=G10%20P0%20L20%20X0%20Y0%20Z0"
#define   GETresetZzero             "/sendGcode/?gCode=G10%20P0%20L20%20Z0.0000"
#define   GETreturntozero           "/sendGcode/?gCode=RETURN_TO_ZERO"
#define   GETmoveToZeroZslowly      "/sendGcode/?gCode=G90%20G0%20Z0%20F50" //return to Z0 slowly
#define   GETmoveToZeroZquickly     "/sendGcode/?gCode=G90%20G0%20Z0%20F500" //return to X0 quickly
#define   GETmoveToZeroXYslowly     "/sendGcode/?gCode=G90%20G0%20X0%20Y0%20F500" //return to XY slowly
#define   GETmoveToZeroXYquickly    "/sendGcode/?gCode=G90%20G0%20X0%20Y0%20F5000" //return to XY quickly
#define   GETreturntoworkspace      "/sendGcode/?gCode=G90%20G0%20X0%20Y0%20Z0"
#define   GEThomingcycle            "/sendGcode/?gCode=%24H"
#define   GETdisablealarmlock       "/sendGcode/?gCode=%24X"
#define   GETtogglecheckmode        "/sendGcode/?gCode=%24C"
#define   GETsoftreset              "/sendGcode/?gCode=%24C" //run toggle check mode twice in a row
//#define   GETsoftreset              "/sendGcode/?gCode=%24C" //^X, 0x18, ctrl-x doesnt work. So physical pin is needed for hard limit alarm
#define   GETspindleON              "/sendGcode/?gCode=M8"
#define   GETspindleOFF             "/sendGcode/?gCode=M9"
#define   GETpause                  "/sendGcode/?gCode=PAUSE_RESUME_FILE"
#define   GETsend                   "/sendGcode/?gCode=SEND_FILE"
#define   GETresume                 "/sendGcode/?gCode=PAUSE_RESUME_FILE"
#define   GETcancelJob              "/sendGcode/?gCode=CANCEL_FILE"
#define   GETcancel                 "/sendGcode/?gCode=CANCEL_FILE"
#define   GETsystemState            "/getSystemState/"
#define   GETtoggleLaser            "/sendGcode/?gCode=%2432%3D" //%36 should be $ Laser toggle is $32=
#define   GETlaserTestOn            "/sendGcode/?gCode=M3" //Run test as M04 Sxx (xx are laserTestStrength) - this will only turn on laser, but not fire it yet, since G1 command is not issued
#define   GETlaserTestPower         "/sendGcode/?gCode=M4%20S1" //Run test as M04 Sxx (xx are laserTestStrength) - this will only turn on laser, but not fire it yet, since G1 command is not issued
#define   GETlaserTestOff           "/sendGcode/?gCode=M5" //Turn off M4 with M05
#define   GETmoveLeftABit           "/sendGcode/?gCode=G1%20G91%20X-0.1%20F5000" //move relative 0.1 mm to left
#define   GETmoveRightABit          "/sendGcode/?gCode=G1%20G91%20X0.1%20F5000" //move relative 0.1 mm to right

bool shiftStatus = 0;
bool prevShiftStatus = 0;
bool directionStatusX = 0;
bool directionStatusY = 0;
bool directionStatusZ = 0;
bool directionStatus = 0;
String webserver;




//=======================================================================
//                    Functions declaration
//=======================================================================

void buttons();
void directionButtons();
void displayLCD();
void getCommandGeneral(String getCommand);
void getDirection(int xdirection, int ydirection, int zdirection, bool slow = 0);
void maintanance();
void parseString(String pendantPayload);
void physicalReset();
bool pingWebsite();
void printSerialDebug(byte mode);
void readUGSPendant();
unsigned int read_shift_regs();
void reConnectToWifi();
void serialPrintInfo();
void zAxisProbe();
