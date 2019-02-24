//=======================================================================
//                    Maintanance function
//=======================================================================
void maintanance(){
        if(abs(millis() - timer1) > skipGetDelay) { //enable pressing buttons after a skipGetDelay ms
                justSentGeneral = 0;
                justSentDirection = 0;
        }
        if(abs(millis() - timer5) > skipMsgDelay) { //enable new message after skipMsgDelay ms
                justSentMsgDisplay = 0;
        }
        if(abs(millis() - timerlcd) > jumpSign) { //jump to next sign after jumpSign ms
                showLCD++;
                if(showLCD > 7) {
                        showLCD = 0;
                }
                timerlcd = millis();
        }
        if(abs(millis() - timer2) > turnBacklightDelay) {//turn off backlight if nothing happends for some time
                lcd.noBacklight();
                lcdBacklightStatus = 0;
        }else if((lcdBacklightStatus == 0 && abs(millis() - timer2) < turnBacklightDelay) || turnOnLCD == 1) {//if something is happening just update timer, so backlight won't turn off
                timer2 = millis();
                lcd.backlight();
                lcdBacklightStatus = 1;
                turnOnLCD = 0;
        }

        if(button[15] == 0) {
                webserver = webserver1;//check if PC 1 is available
        }else if(button[15] == 1) {
                webserver = webserver2;//check if PC 2 is available
        }

        if(disableAlarmLockLogic == 1 && cnc_state[4] == 1 && autoResetStartupAlarm == true) { //run $X on first connect if idle, and if autoResetStartupAlarm is true
                disableAlarmLockLogic = 0;
                delay(1000);
                getCommandGeneral(GETdisablealarmlock);
        }
        if(disableAlarmLockLogic == 1 && cnc_state[4] == 1 && autoResetAlarm == true) { //run $X on every time alarm is triggered (not Hard Limit alarm) if idle, and if autoResetAlarm is true
                disableAlarmLockLogic = 0;
                delay(1000);
                getCommandGeneral(GETdisablealarmlock);
        }

        // led control - once every 1001st cycle
        if(cycleCounter % 1001 == 0) {
                if(laserMode == 1 && ledLaserStatus == 0) { //turn on laser mode led
                        digitalWrite(ledLaser, HIGH);
                        ledLaserStatus = 1;
                        Serial.println("Laser mode is on!");
                }else if(laserMode == 0 && ledLaserStatus == 1) { //turn off laser mode Led
                        digitalWrite(ledLaser, LOW);
                        ledLaserStatus = 0;
                        Serial.println("Laser mode is off!");
                }

                if(button[11] == 1 && ledSlowStatus == 0) { //turn on slow speed mode led - speed toggle pressed
                        digitalWrite(ledSlow, HIGH);
                        ledSlowStatus = 1;
                        Serial.println("Slow speed mode is on!");
                }else if(button[11] == 0 && ledSlowStatus == 1) { //turn off slow speed mode Led - speed toggle de-pressed
                        digitalWrite(ledSlow, LOW);
                        ledSlowStatus = 0;
                        Serial.println("Slow speed mode is off!");
                }
        }

        //handle OTA updates
        if((cnc_state[4] != 1 || cnc_state[5] != 1 || cnc_state[6] != 1 || cnc_state[7] != 1 || cnc_state[8] != 1 || cnc_state[9] != 1 || cnc_state[10] != 1) && cycleCounter % 1102 == 0 && enableOTAUpdate == true) { //check for OTA ONLY when UGS is not available, or PC is not connected, every 1102th cycle
                ArduinoOTA.handle();
        }

        // increase cycle counter
        cycleCounter++;
        if(cycleCounter > 2000000001) { //OVF precaution
                cycleCounter = 1;
        }
        return;
}




//=======================================================================
//                    Parse UGS data
//=======================================================================
void parseString(String pendantPayload){
        // Length (with one extra character for the null terminator)
        int pendantPayload_len = pendantPayload.length() + 1;

        // Prepare the character array (the buffer)
        char StringToChar[pendantPayload_len];
        pendantPayload.toCharArray(StringToChar, pendantPayload_len);
        for(byte n = 0; n < 18; n++) {
                for(byte m = 0; m < varLength; m++) { //fills file name array to SPACE character, so if it changes it doesn't stay the sam as the last one
                        variable[n][m] = 0;
                }
        }
        //Serial.println(pendantPayload);
        bool newvar = 0;
        int i = 0, j = -1, k = 0;
        bool skippedLatestComment = 0;
        //for(int i = 0, j = -1, k = 0; i < pendantPayload_len; i++) {
        while(StringToChar[i] != 125) {//while the character is not }
                if(StringToChar[i] == 58 && StringToChar[i+1] == 34) {// 58 = :, 34 = "
                        newvar = 1;//start saving to array in next iteration
                        k = 0;//variable char array index
                        j++;//array index
                }else if(j == 1 && StringToChar[i] == 108 && StringToChar[i+1] == 97 && StringToChar[i+2] == 116 && StringToChar[i+3] == 101 && StringToChar[i+4] == 115 && StringToChar[i+5] == 116 && StringToChar[i+6] == 67 && StringToChar[i+7] == 111 && StringToChar[i+8] == 109 && StringToChar[i+9] == 109 && StringToChar[i+10] == 101 && StringToChar[i+11] == 110 && StringToChar[i+12] == 116 && StringToChar[i+13] == 34 && StringToChar[i+14] == 58 ) { // if there is latest comment (checks if there are letters "latestComment":"), skip to active state, don't save comment
                        while(StringToChar[i] != 34 && StringToChar[i+1] != 44) { //search until ",
                                i++;
                        }
                        j--;
                        skippedLatestComment = 1; //so it doesn't overwrite file name array
                }else if(StringToChar[i] == 44) {// 44 = ,
                        newvar = 0;//stop saving to array until next :
                        k = 0;//variable char array index
                }
                if(newvar == 1 && ((StringToChar[i] > 47 && StringToChar[i] < 58) || (StringToChar[i] > 65 && StringToChar[i] < 91) || (StringToChar[i] > 96 && StringToChar[i] < 123) || StringToChar[i] == 45 || StringToChar[i] == 46 || StringToChar[i] == 95 || StringToChar[i] == 43 || StringToChar[i] == 40 || StringToChar[i] == 41 || (j == 1 && StringToChar[i] == 32)) && StringToChar[i] != 125 && StringToChar[i] != 123) {//save only small caps, high caps, +, -, . (, )
                        //save var to array
                        if(j == 1 && skippedLatestComment == 1) { //skip

                        }else{ //save
                                variable[j][k] = StringToChar[i];
                        }
                        k++;
                        if(k >= varLength - 1) {
                                k = varLength - 1;
                        }
                }else{
                        k = 0;
                }
                i++;
        }
        //change non text variables into numbers
        if(pendantPayload.indexOf("COMM_IDLE") > 0) {
                cnc_state[0] = 0;
                cnc_state[1] = 0;
                cnc_state[2] = 0;
                cnc_state[3] = 1;
                cnc_state[4] = 1;
                cnc_state[5] = 0;
                cnc_state[6] = 0;
                cnc_state[7] = 0;
                cnc_state[14] = 0;
                directionStatusX = 0;
                directionStatusY = 0;
                directionStatusZ = 0;
                if(pendantPayload.indexOf("Alarm") > 0) {
                        cnc_state[9] = 1;
                        turnOnLCD = 1;
                        if(autoResetAlarm == true) {
                                disableAlarmLockLogic = 1; //automatically $X
                        }
                        if(iftttSend == 1) {
                                iftttMessage(iftttMessageGeneralAlarm);
                        }

                }else if(pendantPayload.indexOf("ALARM:1 (Hard limit)") > 0) {
                        cnc_state[8] = 1;
                        turnOnLCD = 1;
                        if(iftttSend == 1) {
                                iftttMessage(iftttMessageHardAlarm);
                        }
                }else if(pendantPayload.indexOf("Sleep") > 0) {
                        cnc_state[10] = 1;
                        if(iftttSend == 1) {
                                iftttMessage(iftttMessageSleeping);
                        }
                }else if(pendantPayload.indexOf("Idle") > 0) {
                        cnc_state[8] = 0;
                        cnc_state[9] = 0;
                        cnc_state[10] = 0;
                        iftttSend = 0;
                }
                if(resetZAxisAfterProbe == 1) {
                        zAxisProbe();
                }
                iftttSend = 0;
        }else if(pendantPayload.indexOf("COMM_SENDING_PAUSED") > 0) {
                turnOnLCD = 1;
                cnc_state[0] = 0;
                cnc_state[1] = 0;
                cnc_state[2] = 0;
                cnc_state[3] = 1;
                cnc_state[4] = 0;
                cnc_state[5] = 1;
                cnc_state[6] = 1;
                cnc_state[7] = 0;
                cnc_state[10] = 0;
                cnc_state[12] = 1;
                cnc_state[13] = 1;
                cnc_state[14] = 1;
        }else if(pendantPayload.indexOf("COMM_SENDING") > 0) {
                turnOnLCD = 1;
                cnc_state[0] = 0;
                cnc_state[1] = 0;
                cnc_state[2] = 0;
                cnc_state[3] = 1;
                cnc_state[4] = 0;
                cnc_state[6] = 0;
                cnc_state[10] = 0;
                cnc_state[12] = 1;
                cnc_state[13] = 1;
                cnc_state[14] = 1;
                if(pendantPayload.indexOf("Run") > 0) {//Job running
                        cnc_state[5] = 1;
                        cnc_state[7] = 0;
                        iftttSend = 1; //when status changes it should send a notification. So when it stops running it means it finished or something went wrong.
                }else{//Jogging
                        cnc_state[5] = 0;
                        cnc_state[7] = 1;
                        iftttSend = 0;
                }
        }else if(pendantPayload.indexOf("COMM_DISCONNECTED") > 0) {
                cnc_state[0] = 0;
                cnc_state[1] = 0;
                cnc_state[2] = 0;
                cnc_state[3] = 0;
                cnc_state[4] = 1;
                cnc_state[5] = 0;
                cnc_state[6] = 0;
                cnc_state[7] = 0;
                cnc_state[8] = 0;
                cnc_state[9] = 0;
                cnc_state[10] = 0;
                cnc_state[11] = 0;
                cnc_state[12] = 0;
                cnc_state[13] = 0;
                cnc_state[14] = 0;
        }
        workx = atof(variable[3]);
        worky = atof(variable[4]);
        workz = atof(variable[5]);
        machinex = atof(variable[6]);
        machiney = atof(variable[7]);
        machinez = atof(variable[8]);
        rowsInFile = atoi(variable[9]);
        if(rowsInFile < 0) {
                rowsInFile = 0;
        }
        sentRows = atoi(variable[10]);
        if(sentRows < 0) {
                sentRows = 0;
        }
        remainingRows = atoi(variable[11]);
        if(remainingRows < 0) {
                remainingRows = 0;
        }
        duration = atoi(variable[13]);
        if(duration < 0) {
                duration = 0;
        }
        if(sentRows != 0) { //If this is not there ESP goes wild into reboot cycle. It can't divide by 0.
                timeRemaining = ((float)duration / (float)sentRows) * remainingRows; //Estemated time remaining, based on time duration, sent rows and remaining rows
        }else{
                timeRemaining = 0;
        }

        if(timeRemaining < 0) {
                timeRemaining = 0;
        }
        if(rowsInFile != 0) {
                rowProgress = (float)sentRows / ((float)sentRows + (float)remainingRows) * 100; //if there are empty rows they are not subtracted from total rows in file, so this is why you don't use that number
        }else{
                rowProgress = 0;
        }
        if(variable[14][0] == 116) {
                cnc_state[11] = 1;
        }else{
                cnc_state[11] = 0;
        }
        if(variable[15][0] == 116) {
                cnc_state[12] = 1;
        }else{
                cnc_state[12] = 0;
        }

        if(variable[16][0] == 116) {
                cnc_state[13] = 1;
        }else{
                cnc_state[13] = 0;
        }
        displayLCD();
        return;
}




//=======================================================================
//                    Print debug output via serial
//=======================================================================
void printSerialDebug(byte mode){
        if(mode == 0) { //print buttons
                Serial.println("Button status:  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16");
                Serial.print("Button status:  ");
                for(byte i = 0; i < 16; i++) {
                        Serial.print(button[i]);
                        if(i < 15) {
                                Serial.print("  ");
                        }
                }
                Serial.println();
                return;
        }else if(mode == 1) { //print machine status
                Serial.println();
                Serial.print("workx: ");
                Serial.println(workx);
                Serial.print("worky: ");
                Serial.println(worky);
                Serial.print("workz: ");
                Serial.println(workz);
                Serial.print("machinex: ");
                Serial.println(machinex);
                Serial.print("machiney: ");
                Serial.println(machiney);
                Serial.print("machinez: ");
                Serial.println(machinez);
                Serial.print("rowsInFile: ");
                Serial.println(rowsInFile);
                Serial.print("sentRows: ");
                Serial.println(sentRows);
                Serial.print("remainingRows: ");
                Serial.println(remainingRows);
                Serial.print("timeRemaining: ");
                Serial.println(timeRemaining);
                Serial.print("duration: ");
                Serial.println(duration);
                Serial.print("rowProgress: ");
                Serial.println(rowProgress);
                Serial.print("cnc_state[11]: ");
                Serial.println(cnc_state[11]);
                Serial.print("cnc_state[12]: ");
                Serial.println(cnc_state[12]);
                Serial.print("cnc_state[13]: ");
                Serial.println(cnc_state[13]);
                return;
        }else if(mode == 2) {
                if(cycleCounter % 100 == 0) {
                        Serial.print(cycleCounter);
                        Serial.print(", average cycle duration: ");
                        float durationT3 = (millis() - timer3);
                        Serial.print(durationT3);
                        Serial.println(" / 100 ms");
                        timer3 = millis();
                }
                return;
        }else if(mode == 3) {
                float durationT4 = (millis() - timer4);
                Serial.print("Get and parse cycle duration: ");
                Serial.print(durationT4);
                Serial.println(" ms");
                timer3 = millis();
                Serial.println();
        }else if(mode == 4) {
                for(byte i = 0; i < numOfStates; i++) {
                        Serial.print("cnc_state[");
                        Serial.print(i);
                        Serial.print("] = ");
                        Serial.println(cnc_state[i]);
                }
        }else{
                return;
        }
}




//=======================================================================
//                    Print setup data
//=======================================================================
void serialPrintInfo(){ //Prints some general info on startup
        Serial.println();
        Serial.println("=======================================================================");
        Serial.println();
        Serial.print("GRBL, UGS Arduino pendant version: ");
        Serial.println(pendatVersion);
        Serial.print("LCD Language: ");
        Serial.println(lcdLanguage);
        Serial.println();
        Serial.println("Laser mode led: D3");
        Serial.println("Slow mode toggle led: D5");
        if(enableResetPhyPin == true) {
                Serial.println("Reset GRBL arduino pin: D0");
        }
        Serial.println("LCD pinout: D1: SCL, D2: SDA");
        Serial.println("RegIn pins: D4: clock enable pin, D6: data pin, D7: payload pin, D8 clock pin");
        Serial.println();
        Serial.println("Button 0: Homing / Probe");
        Serial.println("Button 1: top right button, Reset XYZ / Reset Z zero");
        Serial.println("Button 2: middle button, Shift, if slow toggle is 1, and laser mode is 1 shift turns on laser test");
        Serial.println("Button 3: bottom left button, Pause-Resume / Laser mode (toggle laser mode if enableLaserMode is enabled) OR toggle Check mode");
        Serial.println("Button 4: bottom right button, Cancel (or $X on idle) / Soft reset + $X");
        Serial.println("Button 5: Y-");
        Serial.println("Button 6: Y+");
        Serial.println("Button 7: X-");
        Serial.println("Button 8: X+");
        Serial.println("Button 9: Z-");
        Serial.println("Button 10: Z+");
        Serial.println("Button 11: slow/fast toggle for jogging");
        Serial.println("Button 12: N/A");
        Serial.println("Button 13: N/A");
        Serial.println("Button 14: N/A");
        Serial.println("Button 15: Computer toggle");
        Serial.println();
        Serial.print("Control computer 1: ");
        Serial.print(controler1Name);
        Serial.print(", ip: ");
        Serial.println(webserver1);
        Serial.print("Control computer 2: ");
        Serial.print(controler2Name);
        Serial.print(", ip: ");
        Serial.println(webserver2);
        Serial.println();
        if(enableLaserMode == true) {
                Serial.println("Toggling laser mode is enabled.");
        }
        Serial.print("Auto $X after general error (NOT LIMIT ALARM) (autoResetAlarm) is ");
        if(autoResetAlarm == true) {
                Serial.println("enabled.");
        }else{
                Serial.println("disabled.");
        }
        Serial.print("Auto $X after startup (autoResetStartupAlarm) is ");
        if(autoResetStartupAlarm == true) {
                Serial.println("enabled.");
        }else{
                Serial.println("disabled.");
        }
        Serial.print("Drill probe height (drillProbeHeight) is ");
        Serial.print(drillProbeHeight);
        Serial.println(" mm");
        if(enableLaserMode == true) {
                Serial.print("Laser probe height offset (laserProbeHeight) is ");
                Serial.print(laserProbeHeight);
                Serial.println(" mm");
        }

        Serial.print("Ifttt messaging is ");
        if(enableIftttMessaging == true) {
                Serial.println("enabled");
        }else{
                Serial.println("disabled");
        }
        Serial.print("OTA updates are ");
        if(enableOTAUpdate == true) {
                Serial.print("enabled.");
                if(enableOTAPassword == true) {
                        Serial.print(" OTA Password is ");
                        Serial.println(OTAPassword);
                }
        }else{
                Serial.println("disabled.");
        }
        Serial.println();
        Serial.println("=======================================================================");
        Serial.println();
}




//=======================================================================
//                    Physical pin for reset on output D5 (14)
//=======================================================================
void physicalReset(){ //Puts D5 that should be connected to GRBL reset/abort pin to low to reset grbl
        if(enableResetPhyPin == true) {
                digitalWrite(resetPinD5, LOW);
                delay(10);
                digitalWrite(resetPinD5, HIGH);
        }
        return;
}
