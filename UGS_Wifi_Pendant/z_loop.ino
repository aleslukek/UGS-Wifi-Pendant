//=======================================================================
//                    Main Program Loop
//=======================================================================
void loop() {
        read_shift_regs(); //read button state always, even if not connected
        buttonValues = read_shift_regs();
        //Serial.println(buttonValues);
        if(oldButtonValues != buttonValues) { //if any button was pressed, do your magic
                buttons();
                directionButtons();
                oldButtonValues = buttonValues;
                timer2 = millis(); //if any button was pressed reset backlight timer
                if(lcdBacklightStatus == 0) { //turn backlight on if it was turned off
                        lcd.backlight();
                        lcdBacklightStatus = 1;
                }
        }

        if((cnc_state[4] == 1 || cnc_state[7] == 1) && (directionStatus == 1 || directionStatusX == 1 || directionStatusY == 1 || directionStatusZ == 1) && (button[5] == 0 && button[6] == 0 && button[7] == 0 && button[8] == 0 && button[9] == 0 && button[10] == 0) ) { //re-check if button was de-pressed to stop the movement: when idle or jogging, buttons are not pressed, it should be moving
                directionStatusX = 0;
                directionStatusY = 0;
                directionStatusZ = 0;
                getCommandGeneral(GETcancel);
        }

        maintanance();
        if(cycleCounter % 1000 == 0) { //read UGS GET output every 1000 cycles
                timer4 = millis();
                //printSerialDebug(0);
                readUGSPendant();
                //printSerialDebug(1);
                if(cnc_state[0] == 1) { //if not connected to wifi, retry to connect
                        reConnectToWifi();
                }
                //printSerialDebug(3); //to time 1000 cycles and one get response read and parse
        }
        //printSerialDebug(3); //to time average of 100 cycles
}
