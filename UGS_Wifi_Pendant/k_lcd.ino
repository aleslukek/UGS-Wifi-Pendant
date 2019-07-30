//=======================================================================
//               Display LCD messages
//=======================================================================
void displayLCD(){
        if(justSentMsgDisplay == 1) {//something was pressed, display what was it
                if(buttonPressMsg == 0) {
                        buttonPressMsg = 255;
                }else if(buttonPressMsg == 1) {
                        #ifndef BUTTONSTOSERIAL
                        Serial.println("Probing");
                        #endif
                        lcd.clear();
                        lcd.setCursor(0, 0);
                        lcd.print(lcdMsgProbingZ);
                        buttonPressMsg = 255;
                }else if(buttonPressMsg == 2) {
                        #ifndef BUTTONSTOSERIAL
                        Serial.println("Reset XYZ");
                        #endif
                        lcd.clear();
                        lcd.setCursor(0, 0);
                        lcd.print(lcdMsgResetXYZ);
                        lcd.setCursor(0, 1);
                        lcd.print(lcdMsgWorkingAxis);
                        buttonPressMsg = 255;
                }else if(buttonPressMsg == 3) {
                        #ifndef BUTTONSTOSERIAL
                        Serial.println("Reset Z");
                        #endif
                        lcd.clear();
                        lcd.setCursor(0, 0);
                        lcd.print(lcdMsgResetZ);
                        lcd.setCursor(0, 1);
                        lcd.print(lcdMsgWorkingAxis2);
                        buttonPressMsg = 255;
                }else if(buttonPressMsg == 4) {
                        #ifndef BUTTONSTOSERIAL
                        Serial.println("Pause");
                        #endif
                        lcd.clear();
                        lcd.setCursor(0, 0);
                        lcd.print(lcdMsgPause);
                        buttonPressMsg = 255;
                }else if(buttonPressMsg == 5) {
                        #ifndef BUTTONSTOSERIAL
                        Serial.println("Reset alarm");
                        #endif
                        lcd.clear();
                        lcd.setCursor(0, 0);
                        lcd.print(lcdMsgResetAlarm);
                        buttonPressMsg = 255;
                }else if(buttonPressMsg == 6) {
                        #ifndef BUTTONSTOSERIAL
                        Serial.println("Cancel");
                        #endif
                        lcd.clear();
                        lcd.setCursor(0, 0);
                        lcd.print(lcdMsgCancel);
                        buttonPressMsg = 255;
                }else if(buttonPressMsg == 7) {
                        #ifndef BUTTONSTOSERIAL
                        Serial.println("Soft reset");
                        #endif
                        lcd.clear();
                        lcd.setCursor(0, 0);
                        lcd.print(lcdMsgResetSystem);
                        lcd.setCursor(0, 1);
                        lcd.print(lcdMsgArduino);
                        buttonPressMsg = 255;
                }else if(buttonPressMsg == 8) {
                        #ifndef BUTTONSTOSERIAL
                        Serial.println("Resume job");
                        #endif
                        lcd.clear();
                        lcd.setCursor(0, 0);
                        lcd.print(lcdMsgContinue);
                        buttonPressMsg = 255;
                }else if(buttonPressMsg == 9) {
                        #ifndef BUTTONSTOSERIAL
                        Serial.println("Start job");
                        #endif
                        lcd.clear();
                        lcd.setCursor(0, 0);
                        lcd.print(lcdMsgStartaJob);
                        buttonPressMsg = 255;
                }else if(buttonPressMsg == 10) {
                        #ifndef BUTTONSTOSERIAL
                        Serial.println("Return to XY zero (quickly)");
                        #endif
                        lcd.clear();
                        lcd.setCursor(0, 0);
                        lcd.print(lcdMsgReturnToXYzero);
                        buttonPressMsg = 255;
                }else if(buttonPressMsg == 11) {
                        #ifndef BUTTONSTOSERIAL
                        Serial.println("Return to XY zero (slowly)");
                        #endif
                        lcd.clear();
                        lcd.setCursor(0, 0);
                        lcd.print(lcdMsgReturnToXYzero);
                        buttonPressMsg = 255;
                }else if(buttonPressMsg == 12) {
                        #ifndef BUTTONSTOSERIAL
                        Serial.println("Return to Z zero (quickly)");
                        #endif
                        lcd.clear();
                        lcd.setCursor(0, 0);
                        lcd.print(lcdMsgReturnToZzero);
                        buttonPressMsg = 255;
                }else if(buttonPressMsg == 13) {
                        #ifndef BUTTONSTOSERIAL
                        Serial.println("Return to Z zero (slowly)");
                        #endif
                        lcd.clear();
                        lcd.setCursor(0, 0);
                        lcd.print(lcdMsgReturnToZzero);
                        buttonPressMsg = 255;
                }if(buttonPressMsg == 14) {
                        #ifndef BUTTONSTOSERIAL
                        Serial.println("Homing cnc");
                        #endif
                        lcd.clear();
                        lcd.setCursor(0, 0);
                        lcd.print(lcdMsgZeroing);
                        lcd.setCursor(0, 1);
                        lcd.print(lcdMsgOfSystem);
                        buttonPressMsg = 255;
                }else if(buttonPressMsg == 108) {
                        #ifndef BUTTONSTOSERIAL
                        Serial.println("Move Y-");
                        #endif
                        lcd.clear();
                        lcd.setCursor(0, 0);
                        lcd.print(lcdMsgMoveYback);
                        lcd.setCursor(0, 1);
                        lcd.print(lcdMsgFYminus);
                        buttonPressMsg = 255;
                }else if(buttonPressMsg == 109) {
                        #ifndef BUTTONSTOSERIAL
                        Serial.println("Move Y- slowly");
                        #endif
                        lcd.clear();
                        lcd.setCursor(0, 0);
                        lcd.print(lcdMsgMoveYback);
                        lcd.setCursor(0, 1);
                        lcd.print(lcdMsgSYminus);
                        buttonPressMsg = 255;
                }else if(buttonPressMsg == 110) {
                        #ifndef BUTTONSTOSERIAL
                        Serial.println("Move Y+");
                        #endif
                        lcd.clear();
                        lcd.setCursor(0, 0);
                        lcd.print(lcdMsgMoveYforward);
                        lcd.setCursor(0, 1);
                        lcd.print(lcdMsgFYplus);
                        buttonPressMsg = 255;
                }else if(buttonPressMsg == 111) {
                        #ifndef BUTTONSTOSERIAL
                        Serial.println("Move Y+ slowly");
                        #endif
                        lcd.clear();
                        lcd.setCursor(0, 0);
                        lcd.print(lcdMsgMoveYforward);
                        lcd.setCursor(0, 1);
                        lcd.print(lcdMsgSYplus);
                        buttonPressMsg = 255;
                }else if(buttonPressMsg == 112) {
                        #ifndef BUTTONSTOSERIAL
                        Serial.println("Move X-");
                        #endif
                        lcd.clear();
                        lcd.setCursor(0, 0);
                        lcd.print(lcdMsgMoveXLeft);
                        lcd.setCursor(0, 1);
                        lcd.print(lcdMsgFXminus);
                        buttonPressMsg = 255;
                }else if(buttonPressMsg == 113) {
                        #ifndef BUTTONSTOSERIAL
                        Serial.println("Move X- slowly");
                        #endif
                        lcd.clear();
                        lcd.setCursor(0, 0);
                        lcd.print(lcdMsgMoveXLeft);
                        lcd.setCursor(0, 1);
                        lcd.print(lcdMsgSXminus);
                        buttonPressMsg = 255;
                }else if(buttonPressMsg == 114) {
                        #ifndef BUTTONSTOSERIAL
                        Serial.println("Move X+");
                        #endif
                        lcd.clear();
                        lcd.setCursor(0, 0);
                        lcd.print(lcdMsgMoveYRight);
                        lcd.setCursor(0, 1);
                        lcd.print(lcdMsgFXplus);
                        buttonPressMsg = 255;
                }else if(buttonPressMsg == 115) {
                        #ifndef BUTTONSTOSERIAL
                        Serial.println("Move X+ slowly");
                        #endif
                        lcd.clear();
                        lcd.setCursor(0, 0);
                        lcd.print(lcdMsgMoveYRight);
                        lcd.setCursor(0, 1);
                        lcd.print(lcdMsgSXplus);
                        buttonPressMsg = 255;
                }else if(buttonPressMsg == 116) {
                        #ifndef BUTTONSTOSERIAL
                        Serial.println("Move Z-");
                        #endif
                        lcd.clear();
                        lcd.setCursor(0, 0);
                        lcd.print(lcdMsgMoveZdown);
                        lcd.setCursor(0, 1);
                        lcd.print(lcdMsgFZminus);
                        buttonPressMsg = 255;
                }else if(buttonPressMsg == 117) {
                        #ifndef BUTTONSTOSERIAL
                        Serial.println("Move Z- slowly");
                        #endif
                        lcd.clear();
                        lcd.setCursor(0, 0);
                        lcd.print(lcdMsgMoveZdown);
                        lcd.setCursor(0, 1);
                        lcd.print(lcdMsgSZminus);
                        buttonPressMsg = 255;
                }else if(buttonPressMsg == 118) {
                        #ifndef BUTTONSTOSERIAL
                        Serial.println("Move Z+");
                        #endif
                        lcd.clear();
                        lcd.setCursor(0, 0);
                        lcd.print(lcdMsgMoveZup);
                        lcd.setCursor(0, 1);
                        lcd.print(lcdMsgFZplus);
                        buttonPressMsg = 255;
                }else if(buttonPressMsg == 119) {
                        #ifndef BUTTONSTOSERIAL
                        Serial.println("Move Z+ slowly");
                        #endif
                        lcd.clear();
                        lcd.setCursor(0, 0);
                        lcd.print(lcdMsgMoveZup);
                        lcd.setCursor(0, 1);
                        lcd.print(lcdMsgSZplus);
                        buttonPressMsg = 255;
                }else if(buttonPressMsg == 120) {
                        #ifndef BUTTONSTOSERIAL
                        Serial.println("Job running, cannot press this button!");
                        #endif
                        lcd.clear();
                        lcd.setCursor(0, 0);
                        lcd.print(lcdMsgCNCjobButton);
                        lcd.setCursor(0, 1);
                        lcd.print(lcdMsgIsDisabled);
                        buttonPressMsg = 255;
                }else if(buttonPressMsg == 31) {
                        #ifndef BUTTONSTOSERIAL
                        Serial.println("Z Probe error. Check what happened.");
                        #endif
                        lcd.clear();
                        lcd.setCursor(0, 0);
                        lcd.print(lcdMsgErrorW);
                        lcd.setCursor(0, 1);
                        lcd.print(lcdMsgEProbingZ);
                        buttonPressMsg = 255;
                }else if(buttonPressMsg == 255) {//do not update
                        return;
                }
                return;
        }else{//nothing was pressed, display status
                if(cnc_state[0] == 1 && cnc_state[1] == 0 && cnc_state[2] == 0 && cnc_state[3] == 0) {//esp wifi not connected
                        #ifndef BUTTONSTOSERIAL
                        Serial.println("Internet cannot be reached.");
                        #endif
                        lcd.clear();
                        lcd.setCursor(0, 0);
                        lcd.print(lcdMsgNoConn);
                        lcd.setCursor(0, 1);
                        lcd.print(lcdMsgToInternet);
                }else if(cnc_state[0] == 0 && cnc_state[1] == 1 && cnc_state[2] == 0 && cnc_state[3] == 0) {//PC not available
                        lcd.clear();
                        if(button[15] == 0) {
                                lcd.setCursor(0,0);
                                lcd.print(controler1Name);
                                #ifndef BUTTONSTOSERIAL
                                Serial.println("CNC PC 1 not available.");
                                #endif
                        }else{

                                lcd.setCursor(0,0);
                                lcd.print(controler2Name);
                                #ifndef BUTTONSTOSERIAL
                                Serial.println("CNC PC 2 not available.");
                              #endif

                        lcd.setCursor(0, 1);
                        lcd.print(lcdMsgNAvailable);
                      }
                }else if(cnc_state[0] == 0 && cnc_state[1] == 0 && cnc_state[2] == 1 && cnc_state[3] == 0) {//PC available, ugs or pendant not running
                        #ifndef BUTTONSTOSERIAL
                        Serial.println("Turn on pendant or UGS.");
                        #endif
                        lcd.clear();
                        lcd.setCursor(0, 0);
                        lcd.print(lcdMsgTurnOnUGS);
                        lcd.setCursor(0, 1);
                        lcd.print(lcdMsgEnablePendant);
                }else if(cnc_state[0] == 0 && cnc_state[1] == 0 && cnc_state[2] == 0 && cnc_state[3] == 0) {//PC available, ugs and pendant available, cnc not connected
                        #ifndef BUTTONSTOSERIAL
                        Serial.println("CNC is not connected to the computer");
                        #endif
                        lcd.clear();
                        lcd.setCursor(0, 0);
                        lcd.print(lcdMsgCNCNotConnected);
                        lcd.setCursor(0, 1);
                        lcd.print(lcdMsgToComputer);
                }else if(cnc_state[0] == 0 && cnc_state[1] == 0 && cnc_state[2] == 0 && cnc_state[3] == 1) {//PC available, ugs available, arduino (cnc) connected to PC via serial
                        if(cnc_state[4] == 1 && cnc_state[5] == 0 && cnc_state[6] == 0 && cnc_state[7] == 0 && (cnc_state[8] == 1 || cnc_state[9] == 1)) {//Alarms - general or hard limit
                                #ifdef OLD_PENDANT
                                if(cnc_state[8] == 1) {
                                        #ifndef BUTTONSTOSERIAL
                                        Serial.println("Hard Limit Alarm triggered");
                                        #endif
                                        lcd.clear();
                                        lcd.setCursor(0, 0);
                                        lcd.print(lcdMsgAlarmLimit);
                                        lcd.setCursor(5, 1);
                                        lcd.print(lcdMsgLimitTriggered);
                                }else if(cnc_state[9] == 1) {
                                        #ifndef BUTTONSTOSERIAL
                                        Serial.println("General Alarm triggered");
                                        #endif
                                        lcd.clear();
                                        lcd.setCursor(0, 0);
                                        lcd.print(lcdMsgAlarm);
                                        lcd.setCursor(5, 1);
                                        lcd.print(lcdMsgGeneralError);
                                }
                                #endif
                                #ifdef NEW_BREILERS_PENDANT
                                if(cnc_state[8] == 1 || cnc_state[9] == 1) {
                                        #ifndef BUTTONSTOSERIAL
                                        Serial.println("Alarm triggered");
                                        #endif
                                        lcd.clear();
                                        lcd.setCursor(0, 0);
                                        lcd.print(lcdMsgAlarmLimit);
                                        lcd.setCursor(5, 1);
                                        lcd.print(lcdMsgLimitTriggered);
                                }
                                #endif
                        }else if(cnc_state[5] == 0 && cnc_state[6] == 0 && (cnc_state[4] == 1 || cnc_state[7] == 1) && cnc_state[8] == 0 && cnc_state[9] == 0) {//system is idle or jogging
                                if(cnc_state[4] == 1) {
                                        #ifndef BUTTONSTOSERIAL
                                        Serial.println("System is idle");
                                        #endif
                                }else if(cnc_state[7] == 1) {
                                        #ifndef BUTTONSTOSERIAL
                                        Serial.println("System is jogging");
                                        #endif
                                }
                                if(showLCD == 0) {
                                        lcd.clear();
                                        lcd.setCursor(0, 0);
                                        lcd.print(lcdMsgWorkX);
                                        lcd.setCursor(5, 1);
                                        lcd.print(workx);
                                }else if(showLCD == 1) {
                                        lcd.clear();
                                        lcd.setCursor(0, 0);
                                        lcd.print(lcdMsgWorkY);
                                        lcd.setCursor(5, 1);
                                        lcd.print(worky);
                                }else if(showLCD == 2) {
                                        lcd.clear();
                                        lcd.setCursor(0, 0);
                                        lcd.print(lcdMsgWorkZ);
                                        lcd.setCursor(5, 1);
                                        lcd.print(workz);
                                }else if(showLCD == 3) {
                                        lcd.clear();
                                        lcd.setCursor(0, 0);
                                        lcd.print(lcdMsgAbsX);
                                        lcd.setCursor(5, 1);
                                        lcd.print(machinex);
                                }else if(showLCD == 4) {
                                        lcd.clear();
                                        lcd.setCursor(0, 0);
                                        lcd.print(lcdMsgAbsY);
                                        lcd.setCursor(5, 1);
                                        lcd.print(machiney);
                                }else if(showLCD == 5) {
                                        lcd.clear();
                                        lcd.setCursor(0, 0);
                                        lcd.print(lcdMsgAbsZ);
                                        lcd.setCursor(5, 1);
                                        lcd.print(machinez);
                                }else if(showLCD == 6) {
                                        if(cnc_state[7] == 0) {      //idle
                                                lcd.clear();
                                                lcd.setCursor(0, 0);
                                                lcd.print(lcdMsgWaiting);
                                                lcd.setCursor(0, 1);
                                                lcd.print(lcdMsgMakingMeW);
                                        }else if(cnc_state[7] == 1) {      //jogging
                                                lcd.clear();
                                                lcd.setCursor(0, 0);
                                                lcd.print(lcdMsgJogging);
                                        }else if(cnc_state[10] == 1) {      //sleeping
                                                lcd.clear();
                                                lcd.setCursor(0, 0);
                                                lcd.print(lcdMsgSleeping);
                                        }
                                }

                        }else if(cnc_state[4] == 0 && (cnc_state[5] == 1 || cnc_state[6] == 1) && cnc_state[7] == 0) {//system is running a job
                                #ifndef BUTTONSTOSERIAL
                                Serial.println("System is running a job");
                                #endif
                                if(showLCD == 0) {
                                        lcd.clear();
                                        lcd.setCursor(0, 0);
                                        lcd.print(lcdMsgAllR);
                                        lcd.setCursor(5, 1);
                                        lcd.print(rowsInFile);
                                }else if(showLCD == 1) {
                                        lcd.clear();
                                        lcd.setCursor(0, 0);
                                        lcd.print(lcdMsgSentR);
                                        lcd.setCursor(5, 1);
                                        lcd.print(sentRows);
                                }else if(showLCD == 2) {
                                        lcd.clear();
                                        lcd.setCursor(0, 0);
                                        lcd.print(lcdMsgRemainingR);
                                        lcd.setCursor(5, 1);
                                        lcd.print(remainingRows);
                                }else if(showLCD == 3) {
                                        lcd.clear();
                                        lcd.setCursor(0, 0);
                                        lcd.print(lcdMsgRemainingT);
                                        timeRemaining = timeRemaining / 1000;
                                        int hours = timeRemaining / 3600;
                                        byte minutes = (timeRemaining - (hours * 3600)) / 60;
                                        byte seconds = timeRemaining  - (hours * 3600) - (minutes * 60);
                                        if(hours > 99) {
                                                lcd.setCursor(0, 1);
                                                lcd.print(lcdMsgCantTell);
                                        }else{
                                                if(hours < 10) {
                                                        lcd.setCursor(5,1);
                                                        lcd.print("0");
                                                        if(hours == 0) {
                                                                lcd.setCursor(6,1);
                                                                lcd.print("0");
                                                        }else{
                                                                lcd.setCursor(6,1);
                                                                lcd.print(hours);
                                                        }
                                                }else{
                                                        lcd.setCursor(5,1);
                                                        lcd.print(hours);
                                                }
                                                lcd.setCursor(7,1);
                                                lcd.print(":");
                                                if(minutes < 10) {
                                                        lcd.setCursor(8,1);
                                                        lcd.print("0");
                                                        if(minutes == 0) {
                                                                lcd.setCursor(9,1);
                                                                lcd.print("0");
                                                        }else{
                                                                lcd.setCursor(9,1);
                                                                lcd.print(minutes);
                                                        }
                                                }else{
                                                        lcd.setCursor(8,1);
                                                        lcd.print(minutes);
                                                }
                                                lcd.setCursor(10,1);
                                                lcd.print(":");
                                                if(seconds < 10) {
                                                        lcd.setCursor(11,1);
                                                        lcd.print("0");
                                                        if(seconds == 0) {
                                                                lcd.setCursor(12,1);
                                                                lcd.print("0");
                                                        }else{
                                                                lcd.setCursor(12,1);
                                                                lcd.print(seconds);
                                                        }
                                                }else{
                                                        lcd.setCursor(11,1);
                                                        lcd.print(seconds);
                                                }
                                        }

                                }else if(showLCD == 4) {
                                        lcd.clear();
                                        lcd.setCursor(0, 0);
                                        lcd.print(lcdMsgDuration);
                                        duration = duration / 1000;
                                        int hours = duration / 3600;
                                        byte minutes = (duration - (hours * 3600)) / 60;
                                        byte seconds = duration  - (hours * 3600) - (minutes * 60);
                                        if(hours > 99) {
                                                lcd.setCursor(4,1);
                                        }else{
                                                lcd.setCursor(5,1);
                                        }
                                        if(hours < 10) {
                                                lcd.setCursor(5,1);
                                                lcd.print("0");
                                                if(hours == 0) {
                                                        lcd.setCursor(6,1);
                                                        lcd.print("0");
                                                }else{
                                                        lcd.setCursor(6,1);
                                                        lcd.print(hours);
                                                }
                                        }else{
                                                lcd.setCursor(5,1);
                                                lcd.print(hours);
                                        }
                                        lcd.setCursor(7,1);
                                        lcd.print(":");
                                        if(minutes < 10) {
                                                lcd.setCursor(8,1);
                                                lcd.print("0");
                                                if(minutes == 0) {
                                                        lcd.setCursor(9,1);
                                                        lcd.print("0");
                                                }else{
                                                        lcd.setCursor(9,1);
                                                        lcd.print(minutes);
                                                }
                                        }else{
                                                lcd.setCursor(8,1);
                                                lcd.print(minutes);
                                        }
                                        lcd.setCursor(10,1);
                                        lcd.print(":");
                                        if(seconds < 10) {
                                                lcd.setCursor(11,1);
                                                lcd.print("0");
                                                if(seconds == 0) {
                                                        lcd.setCursor(12,1);
                                                        lcd.print("0");
                                                }else{
                                                        lcd.setCursor(12,1);
                                                        lcd.print(seconds);
                                                }
                                        }else{
                                                lcd.setCursor(11,1);
                                                lcd.print(seconds);
                                        }
                                }else if(showLCD == 5) {
                                        if(cnc_state[5] == 1) {
                                                lcd.clear();
                                                lcd.setCursor(0, 0);
                                                lcd.print(lcdMsgCNCIs);
                                                lcd.setCursor(0, 1);
                                                lcd.print(lcdMsgWorking);
                                        }else if(cnc_state[6] == 1) {
                                                lcd.clear();
                                                lcd.setCursor(0, 0);
                                                lcd.print(lcdMsgCNCIs);
                                                lcd.setCursor(0, 1);
                                                lcd.print(lcdMsgCurrPause);
                                        }
                                }else if(showLCD == 6) {
                                        lcd.clear();
                                        lcd.setCursor(0, 0);
                                        lcd.print(lcdMsgProgress);
                                        lcd.setCursor(5, 1);
                                        lcd.print(rowProgress);
                                        lcd.setCursor(10, 1);
                                        lcd.print("%");
                                }
                        }
                }else{//I don't know what is going on
                        #ifndef BUTTONSTOSERIAL
                        Serial.print("Don't know...");
                        #endif
                        lcd.clear();
                        lcd.setCursor(0, 0);
                        lcd.print(lcdMsgSomethingWentWrong);
                        lcd.setCursor(0, 1);
                        lcd.print(lcdMsgIDK);
                }
                return;
        }
        return;
}
