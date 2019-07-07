//=======================================================================
//                    Shift in function,
//
// This function is essentially a "shift-in" routine reading the
// serial Data from the shift register chips and representing
// the state of those pins in an unsigned integer (or long).
//
//=======================================================================

unsigned int read_shift_regs(){
        long bitVal;
        unsigned int bytesVal = 0;
        // Trigger a parallel Load to latch the state of the data lines
        digitalWrite(clockEnablePin, HIGH);
        digitalWrite(ploadPin, LOW);
        delayMicroseconds(PULSE_WIDTH_USEC);
        digitalWrite(ploadPin, HIGH);
        digitalWrite(clockEnablePin, LOW);

        // Loop to read each bit value from the serial out line of the SN74HC165N.
        for(int i = DATA_WIDTH-1; i >= 0; i--) {
                bitVal = digitalRead(dataPin);
                button[i] = digitalRead(dataPin);
                /* Set the corresponding bit in bytesVal.
                 */
                bytesVal |= (bitVal << ((DATA_WIDTH-1) - i));

                //button[i] = 0; //uncommenting this will disable Regins, for testing without shift registers and without buttons. Comment this when everything is connected

                // Pulse the Clock (rising edge shifts the next bit).
                digitalWrite(clockPin, HIGH);
                delayMicroseconds(PULSE_WIDTH_USEC);
                digitalWrite(clockPin, LOW);
        }
        return(bytesVal);
}




//=======================================================================
//                    Control buttons logic
//=======================================================================
void buttons(){
        if(cnc_state[0] == 0 && cnc_state[1] == 0 && cnc_state[2] == 0 && cnc_state[3] == 1) { //if CNC is available, then listen to the buttons
                if(button[0] == 1 && button[2] == 0) {//button 1 pressed, no shift
                        if(cnc_state[5] == 1 || cnc_state[14] == 1) {//if job is running, ignore commands
                                cnc_state[14] = 1;
                                buttonPressMsg = 120;
                        }else{
                                getCommandGeneral(GEThomingcycle);
                                buttonPressMsg = 14;
                        }
                        return;
                }else if(button[0] == 1 && button[2] == 1) {//button 1 pressed, with shift
                        if(cnc_state[5] == 1 || cnc_state[14] == 1) {//if job is running, ignore commands
                                cnc_state[14] = 1;
                                buttonPressMsg = 120;
                        }else{
                                zAxisProbe();
                                buttonPressMsg = 1;
                        }
                        return;
                }else if(button[1] == 1 && button[2] == 0) {//button 2 pressed, no shift
                        if(cnc_state[5] == 1 || cnc_state[14] == 1) {//if job is running, ignore commands
                                cnc_state[14] = 1;
                                buttonPressMsg = 120;
                        }else{
                                getCommandGeneral(GETresetzero);
                                buttonPressMsg = 2;
                        }
                        return;
                }else if(button[1] == 1 && button[2] == 1) {//button 2 pressed, with shift
                        if(cnc_state[5] == 1 || cnc_state[14] == 1) {//if job is running, ignore commands
                                cnc_state[14] = 1;
                                buttonPressMsg = 120;
                        }else{
                                #ifdef OLD_PENDANT
                                getCommandGeneral(GETresetZzero);
                                #else
                                postSendGcode(POSTresetZzero0);
                                #endif
                                buttonPressMsg = 3;
                        }
                        return;
                }else if(button[3] == 1 && button[2] == 0) {//button 4 pressed, no shift -> pause if running, resume if job, send if idle
                        if((cnc_state[5] == 1 || cnc_state[7] == 1) && cnc_state[6] == 0) {//job is running, or it is jogging, and NOT on hold, PAUSE
                                getCommandGeneral(GETpause);
                                buttonPressMsg = 4;
                        }else if((cnc_state[5] == 1 || cnc_state[7] == 1) && cnc_state[6] == 1) {//job is running, or it is jogging, and if it's on hold, RESUME
                                #ifdef OLD_PENDANT
                                getCommandGeneral(GETresume);
                                #else
                                postSendGcode(POSTsendResume);
                                #endif
                                buttonPressMsg = 8;
                        }else if((cnc_state[5] == 0 || cnc_state[7] == 0) && cnc_state[6] == 0) {//job is NOT running, or it is NOT jogging, and it is NOT on hold, SEND
                                #ifdef OLD_PENDANT
                                getCommandGeneral(GETsend);
                                #else
                                postSendGcode(POSTsendResume);
                                #endif
                                buttonPressMsg = 9;
                        }
                        return;
                }else if(button[3] == 1 && button[2] == 1 && (cnc_state[5] != 1 || cnc_state[14] != 1) && enableLaserMode == true) {//button 4 pressed, with shift, Toggle laser mode if enableLaserMode is true, if enableLaserMode is not enabled this will toggle check mode
                        #ifdef OLD_PENDANT
                        getCommandGeneral(GETtoggleLaser);
                        #else
                        postSendGcode(GETtoggleLaser);
                        #endif
                        if(laserMode == 0){
                          #ifdef OLD_PENDANT
                          getCommandGeneral(GETlaserTestOff);
                          #else
                          postSendGcode(GETlaserTestOff);
                          #endif
                        }
                        return;
                }else if(button[3] == 1 && button[2] == 1 && (cnc_state[5] != 1 || cnc_state[14] != 1) && enableLaserMode == false) {//button 4 pressed, with shift, Toggle check, enableLaserMode is false
                        #ifdef OLD_PENDANT
                        getCommandGeneral(GETtogglecheckmode);
                        #else
                        postSendGcode(GETtogglecheckmode);
                        #endif
                        return;
                }else if(button[4] == 1 && button[2] == 0) {//button 5 pressed, no shift
                        if(cnc_state[5] == 1) { //if on job cancel job
                                getCommandGeneral(GETcancelJob);
                        }else if(cnc_state[6] == 1 || cnc_state[7] == 1) {// if on hold or jogging cancel
                                getCommandGeneral(GETcancel);
                        }else if(cnc_state[5] == 0 && cnc_state[6] == 0 && cnc_state[7] == 0) { //if idle reset alarm ($X)
                                getCommandGeneral(GETdisablealarmlock);
                        }else{ //cancel
                                getCommandGeneral(GETcancel);
                        }
                        buttonPressMsg = 6;
                        return;
                }else if(button[4] == 1 && button[2] == 1) {//button 5 pressed, with shift
                        if(cnc_state[5] == 1 || cnc_state[14] == 1) {
                                cnc_state[14] = 1;
                                buttonPressMsg = 120;
                        }else{
                                buttonPressMsg = 7;
                                displayLCD();
                                if(enableResetPhyPin != true){
                                        getCommandGeneral(GETsoftreset);//run soft reset twice (toggle check mode twice resets the grbl)

                                }else if(cnc_state[8] == 1 && cnc_state[5] != 1 && cnc_state[3] == 1 && enableResetPhyPin == true) { //if hard limit is triggered, job not running and arduino is available
                                        physicalReset(); //D5 to low that resets grbl reset physical pin
                                        delay(1000);
                                        getCommandGeneral(GETdisablealarmlock);
                                }

                        }
                        return;
                }else if(button[2] == 1 && button[0] == 0 && button[1] == 0 && button[3] == 0 && button[4] == 0 && button[5] == 0 && button[6] == 0 && button[7] == 0 && button[8] == 0 && button[9] == 0 && button[10] == 0 && button[11] == 1 && laserMode == 1 && laserTest == 0) { //shift + slow button with laser mode -> laser test
                        if(laserMode == 1 && button[11] == 1 && laserTest == 0) {//recheck if slow button is on, and lasermode is on. Do a laser test if laserTest is not running
                                laserTest = 1;
                                #ifdef OLD_PENDANT
                                getCommandGeneral(GETlaserTestOn);
                                #else
                                postSendGcode(GETlaserTestOn);
                                #endif
                                return;
                        }else{
                                return;
                        }
                }else if(button[2] == 0 && button[0] == 0 && button[1] == 0 && button[3] == 0 && button[4] == 0 && button[5] == 0 && button[6] == 0 && button[7] == 0 && button[8] == 0 && button[9] == 0 && button[10] == 0 && button[11] == 1 && laserMode == 1 && laserTest == 1) { //no shift + slow button with laser mode -> stop laser test
                        if(laserMode == 1 && button[11] == 1 && laserTest == 1) {//recheck if slow button is on, and lasermode is on. Stop a laser test if laserTest is running
                                laserTest = 0;
                                #ifdef OLD_PENDANT
                                getCommandGeneral(GETlaserTestOff);
                                #else
                                postSendGcode(GETlaserTestOff);
                                #endif
                                return;
                        }else{
                                return;
                        }
                }else{
                        return;
                }
        }
}




//=======================================================================
//                    Jogging button logic
//=======================================================================
void directionButtons(){
        if(cnc_state[0] == 0 && cnc_state[1] == 0 && cnc_state[2] == 0 && cnc_state[3] == 1) { //if CNC is available, then listen to the buttons
                if(button[5] == 1 && directionStatusY == 0 && button[11] == 0 && button[2] == 0) {//button pushed, move it: Y negative, quickly
                        directionStatusY = 1;
                        #ifdef OLD_PENDANT
                        getDirection(0, -1, 0, 0);
                        #else
                        postDirection(0, -1, 0, 0);
                        #endif
                        buttonPressMsg = 108;
                        return;
                }else if(button[5] == 1 && directionStatusY == 0 && button[11] == 1 && button[2] == 0) {//button pushed, move it: Y negative, slowly
                        directionStatusY = 1;
                        #ifdef OLD_PENDANT
                        getDirection(0, -1, 0, 1);
                        #else
                        postDirection(0, -1, 0, 1);
                        #endif
                        buttonPressMsg = 109;
                        return;
                }else if(button[5] == 0 && directionStatusY == 1) {//button depressed, stop it!
                        directionStatusY = 0;
                        getCommandGeneral(GETcancel);
                        return;
                }else if(button[6] == 1 && directionStatusY == 0 && button[11] == 0 && button[2] == 0) {//button pushed, move it: Y positive, quickly
                        directionStatusY = 1;
                        #ifdef OLD_PENDANT
                        getDirection(0, 1, 0, 0);
                        #else
                        postDirection(0, 1, 0, 0);
                        #endif
                        buttonPressMsg = 110;
                        return;
                }else if(button[6] == 1 && directionStatusY == 0 && button[11] == 1 && button[2] == 0) {//button pushed, move it: Y positive, slowly
                        directionStatusY = 1;
                        #ifdef OLD_PENDANT
                        getDirection(0, 1, 0, 1);
                        #else
                        postDirection(0, 1, 0, 1);
                        #endif
                        buttonPressMsg = 111;
                        return;
                }else if(button[6] == 0 && directionStatusY == 1) {//button depressed, stop it!
                        directionStatusY = 0;
                        getCommandGeneral(GETcancel);
                        return;
                }else if(button[7] == 1 && directionStatusX == 0 && button[11] == 0 && button[2] == 0) {//button pushed, move it: X negative, quickly
                        directionStatusX = 1;
                        #ifdef OLD_PENDANT
                        getDirection(-1, 0, 0, 0);
                        #else
                        postDirection(-1, 0, 0, 0);
                        #endif
                        buttonPressMsg = 112;
                        return;
                }else if(button[7] == 1 && directionStatusX == 0 && button[11] == 1 && button[2] == 0) {//button pushed, move it: X negative, slowly
                        directionStatusX = 1;
                        #ifdef OLD_PENDANT
                        getDirection(-1, 0, 0, 1);
                        #else
                        postDirection(-1, 0, 0, 1);
                        #endif
                        buttonPressMsg = 113;
                        return;
                }else if(button[7] == 0 && directionStatusX == 1) {//button depressed, stop it!
                        directionStatusX = 0;
                        getCommandGeneral(GETcancel);
                        return;
                }else if(button[8] == 1 && directionStatusX == 0 && button[11] == 0 && button[2] == 0) {//button pushed, move it: X positive, quickly
                        directionStatusX = 1;
                        #ifdef OLD_PENDANT
                        getDirection(1, 0, 0, 0);
                        #else
                        postDirection(1, 0, 0, 0);
                        #endif
                        buttonPressMsg = 114;
                        return;
                }else if(button[8] == 1 && directionStatusX == 0 && button[11] == 1 && button[2] == 0) {//button pushed, move it: Y positive, slowly
                        directionStatusX = 1;
                        #ifdef OLD_PENDANT
                        getDirection(1, 0, 0, 1);
                        #else
                        postDirection(1, 0, 0, 1);
                        #endif
                        buttonPressMsg = 115;
                        return;
                }else if(button[8] == 0 && directionStatusX == 1) {//button depressed, stop it!
                        directionStatusX = 0;
                        getCommandGeneral(GETcancel);
                        return;
                }else if(button[9] == 1 && directionStatusZ == 0 && button[11] == 0 && button[2] == 0) {//button pushed, move it: X negative, quickly
                        directionStatusZ = 1;
                        #ifdef OLD_PENDANT
                        getDirection(0, 0, -1, 0);
                        #else
                        postDirection(0, 0, -1, 0);
                        #endif
                        buttonPressMsg = 116;
                        return;
                }else if(button[9] == 1 && directionStatusZ == 0 && button[11] == 1 && button[2] == 0) {//button pushed, move it: X negative, slowly
                        directionStatusZ = 1;
                        #ifdef OLD_PENDANT
                        getDirection(0, 0, -1, 1);
                        #else
                        postDirection(0, 0, -1, 1);
                        #endif
                        buttonPressMsg = 117;
                        return;
                }else if(button[9] == 0 && directionStatusZ == 1) {//button depressed, stop it!
                        directionStatusZ = 0;
                        getCommandGeneral(GETcancel);
                        return;
                }else if(button[10] == 1 && directionStatusZ == 0 && button[11] == 0 && button[2] == 0) {//button pushed, move it: X positive, quickly
                        directionStatusZ = 1;
                        #ifdef OLD_PENDANT
                        getDirection(0, 0, 1, 0);
                        #else
                        postDirection(0, 0, 1, 0);
                        #endif
                        buttonPressMsg = 118;
                        return;
                }else if(button[10] == 1 && directionStatusZ == 0 && button[11] == 1 && button[2] == 0) {//button pushed, move it: Y positive, slowly
                        directionStatusZ = 1;
                        #ifdef OLD_PENDANT
                        getDirection(0, 0, 1, 1);
                        #else
                        postDirection(0, 0, 1, 1);
                        #endif
                        buttonPressMsg = 119;
                        return;
                }else if((button[5] == 1 || button[6] == 1 || button[7] == 1 || button[8] == 1) && button[9] == 0 && button[10] == 0 && button[2] == 1 && button[11] == 0) { //move to XY zero GETmoveToZeroXYquickly: any X or any Y PLUS Shift and no slow toggle
                        #ifdef OLD_PENDANT
                        getCommandGeneral(GETmoveToZeroXYquickly);
                        #else
                        postSendGcode(GETmoveToZeroXYquickly);
                        #endif
                        buttonPressMsg = 10;
                        return;
                }else if((button[5] == 1 || button[6] == 1 || button[7] == 1 || button[8] == 1) && button[9] == 0 && button[10] == 0 && button[2] == 1 && button[11] == 1) { //move to XY zero GETmoveToZeroXYslowly: any X or any Y PLUS Shift PLUS slow toggle
                        #ifdef OLD_PENDANT
                        getCommandGeneral(GETmoveToZeroXYslowly);
                        #else
                        postSendGcode(GETmoveToZeroXYslowly);
                        #endif
                        buttonPressMsg = 11;
                        return;
                }else if(button[5] == 0 && button[6] == 0 && button[7] == 0 && button[8] == 0 && (button[9] == 1 || button[10] == 1) && button[2] == 1 && button[11] == 0) { //move to XY zero GETmoveToZeroZquickly: any Z PLUS Shift and no slow toggle
                        #ifdef OLD_PENDANT
                        getCommandGeneral(GETmoveToZeroZquickly);
                        #else
                        postSendGcode(GETmoveToZeroZquickly);
                        #endif
                        buttonPressMsg = 12;
                        return;
                }else if(button[5] == 0 && button[6] == 0 && button[7] == 0 && button[8] == 0 && (button[9] == 1 || button[10] == 1) && button[2] == 1 && button[11] == 1) { //move to XY zero GETmoveToZeroZquickly: any Z PLUS Shift with slow toggle
                        #ifdef OLD_PENDANT
                        getCommandGeneral(GETmoveToZeroZslowly);
                        #else
                        postSendGcode(GETmoveToZeroZslowly);
                        #endif
                        buttonPressMsg = 13;
                        return;
                }else if(button[10] == 0 && directionStatusZ == 1) {//button depressed, stop it!
                        directionStatusZ = 0;
                        getCommandGeneral(GETcancel);
                        return;
                }else{
                        return;
                }
        }
}
