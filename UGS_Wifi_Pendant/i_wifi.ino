//=======================================================================
//                    Ping function
//=======================================================================
bool pingWebsite(){
        bool result = Ping.ping("8.8.8.8", 1);
        if(result == true) {//ping sucessful
                return 0;
        }else{
                return 1;
        }
}




//=======================================================================
//                    Reconnect to Wifi
//=======================================================================
void reConnectToWifi(){
        if(lcdBacklightStatus == 0) { //turn backlight on if it was turned off
                lcd.backlight();
                lcdBacklightStatus = 1;
        }
        WiFi.mode(WIFI_OFF);  //Prevents reconnection issue (taking too long to connect)
        delay(500);
        WiFi.mode(WIFI_STA);  //This line hides the viewing of ESP as wifi hotspot

        WiFi.begin(ssid, password); //Connect to your WiFi router
        Serial.println("");

        Serial.print("Connecting");
        // Wait for connection
        while (WiFi.status() != WL_CONNECTED && Serial.available() > 0) {
                delay(500);
                Serial.print(".");
        }

        //If connection successful show IP address in serial monitor
        Serial.println("");
        Serial.print("Connected to ");
        Serial.println(ssid);
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP()); //IP address assigned to your ESP
        cnc_state[0] = 0;
        if(button[15] == 0) {
                cnc_state[1] = Ping.ping(webserver1, 1);//check if PC 1 is available
        }else if(button[15] == 1) {
                cnc_state[1] = Ping.ping(webserver2, 1);//check if PC 2 is available
        }

}




//=======================================================================
//                    Send General Commands
//=======================================================================
void getCommandGeneral(String getCommand){
        //printSerialDebug(0);
        HTTPClient http;         //Declare object of class HTTPClient
        if(getCommand == GETcancel && directionStatus == 1) { //if moving, set directionStatus as 0 when canceling the movement
                directionStatus = 0;
                justSentGeneral = 0;
                justSentDirection = 0;
        }
        if(justSentGeneral != 1 && getCommand != GETtoggleLaser) {
                String getFull = "http://" + webserver + ":" + webserverport + getCommand;
                http.begin(getFull);
                int httpCode = http.GET();
                String payload = http.getString();         //Get the response payload
                //Serial.println(payload);    //Print request response payload
                http.end();         //Close connection
                timer1 = millis();
                timer5 = millis();
                justSentGeneral = 1;
                justSentMsgDisplay = 1;
        }else if(justSentGeneral != 1 && getCommand == GETtoggleLaser) { //turn on laser mode
                if(laserMode == 1) {
                        laserMode = 0;
                }else{
                        laserMode = 1;
                }
                String getFull = "http://" + webserver + ":" + webserverport + GETtoggleLaser + laserMode;
                http.begin(getFull);
                int httpCode = http.GET();
                String payload = http.getString();         //Get the response payload
                //Serial.println(payload);    //Print request response payload
                if(laserMode == 1) { //for some reason it works if you move G1 for a bit and then M3 and M5 works even when not moving
                        getFull = "http://" + webserver + ":" + webserverport + GETlaserTestPower;
                        http.begin(getFull);
                        httpCode = http.GET();
                        delay(100);
                        getFull = "http://" + webserver + ":" + webserverport + GETlaserTestOff;
                        http.begin(getFull);
                        httpCode = http.GET();
                        delay(100);
                        getFull = "http://" + webserver + ":" + webserverport + GETmoveLeftABit;
                        http.begin(getFull);
                        httpCode = http.GET();
                        delay(200);
                        getFull = "http://" + webserver + ":" + webserverport + GETmoveRightABit;
                        http.begin(getFull);
                        httpCode = http.GET();
                }else if(laserMode == 0) {
                        delay(100);
                        getFull = "http://" + webserver + ":" + webserverport + GETlaserTestOff;
                        http.begin(getFull);
                        httpCode = http.GET();
                }
                http.end();         //Close connection
                timer1 = millis();
                timer5 = millis();
                justSentGeneral = 1;
                justSentMsgDisplay = 1;
        }else if(((justSentGeneral != 1 && getCommand == GETlaserTestOn) || getCommand == GETlaserTestOff) && laserMode == 1) { //toggle laser at lowest power if laser mode is enabled
                String getFull = "http://" + webserver + ":" + webserverport;
                if(laserTest == 1) { //Start laser test
                        getFull = getFull + GETlaserTestOn;
                }else{ //end laser test
                        getFull = getFull + GETlaserTestOff;
                }
                http.begin(getFull);
                int httpCode = http.GET();
                //String payload = http.getString();         //Get the response payload
                //Serial.println(payload);    //Print request response payload
                http.end();         //Close connection
                timer1 = millis();
                timer5 = millis();
                justSentGeneral = 1;
                justSentMsgDisplay = 1;
        }else{
        }
        Serial.print("General command: ");
        Serial.println(getCommand);
        return;
}




//=======================================================================
//                    Send Jog Command
//=======================================================================
void getDirection(int xdirection, int ydirection, int zdirection, bool slow){
        //printSerialDebug(0);
        Serial.print("get direction, ");
        HTTPClient http;
        int step = xyStepSizeFast;
        int rate = xyStepRateFast;
        if(cnc_state[5] == 1 || cnc_state[14] == 1) {        //if job is running, ignore commands
                cnc_state[14] = 1;
                Serial.print("job running, ");
                buttonPressMsg = 120;
                return;
        }else{
                Serial.print("job not running, ");
                if(slow == 1) {
                        step = xyStepSizeSlow;
                        rate = xyStepRateSlow;
                }else{
                        step = xyStepSizeFast;
                        rate = xyStepRateFast;
                }
                String direction = "";
                HTTPClient http;         //Declare object of class HTTPClient
                if(zdirection != 0 && xdirection == 0 && ydirection == 0) {        //small step for Z, larger for XY
                        step = step / 10;
                        rate = rate / 10;
                        if(zdirection > 0) {
                                direction = "Z";
                        }else if(zdirection < 0) {
                                direction = "Z-";
                        }
                }else if(xdirection != 0 && ydirection == 0 && zdirection == 0) {
                        if(xdirection > 0) {
                                direction = "X";
                        }else if(xdirection < 0) {
                                direction = "X-";
                        }
                }else if(ydirection != 0 && xdirection == 0 && zdirection == 0) {
                        if(ydirection > 0) {
                                direction = "Y";
                        }else if(ydirection < 0) {
                                direction = "Y-";
                        }
                }
                String getFull = "http://" + webserver + ":" + webserverport + GETmovexyz + direction + step + "F" + rate;
                http.begin(getFull);
                int httpCode = http.GET();
                /*int httpCode = http.GET();
                   String payload = http.getString(); //Get the response payload*/
                //Serial.println(payload);    //Print request response payload
                http.end();         //Close connection
                timer1 = millis();
                timer5 = millis();
                justSentDirection = 1;
                justSentMsgDisplay = 1;
                directionStatus = 1;
                Serial.println("Direction command done");
        }
        return;
}




//=======================================================================
//                    Z Axis probe
//=======================================================================
void zAxisProbe(){
        HTTPClient http;
        if(resetZAxisAfterProbe == 0 && cnc_state[4] == 1) {//Not yet probed, start probing
                //G38.2 Z-20 F100 -> to activate Probe
                String getFull = "http://" + webserver + ":" + webserverport + "/sendGcode/?gCode=G38.2%20Z-" + probeStep + "%20F" + probeRate;
                http.begin(getFull);
                int httpCode = http.GET();
                http.end(); //Close connection
                timer1 = millis();
                timer5 = millis();
                justSentDirection = 1;
                justSentMsgDisplay = 1;
                resetZAxisAfterProbe = 1;
                Serial.println("Sending Z probe move commands");
        }else if(resetZAxisAfterProbe == 1 && (cnc_state[8] == 0 || cnc_state[9] == 0) && cnc_state[4] == 1) {//Probe finished without alarm, reset Z axis for probe height
                delay(500);
                //G92 Z2.0 -> reset z height, for probe plate height (2mm)
                String getFull = "http://" + webserver + ":" + webserverport + "/sendGcode/?gCode=G92%20Z";
                if(laserMode == 0) {
                        getFull = getFull + drillProbeHeight; //different probe height offset for drilling
                }else if(laserMode == 1) {
                        getFull = getFull + laserProbeHeight; //different probe height offset for laser
                }
                http.begin(getFull);
                http.end(); //Close connection
                int httpCode = http.GET();
                timer1 = millis();
                timer5 = millis();
                justSentDirection = 1;
                justSentMsgDisplay = 1;
                resetZAxisAfterProbe = 0;
        }else{
                justSentGeneral = 1;
                justSentMsgDisplay = 1;
                resetZAxisAfterProbe = 0;
                buttonPressMsg = 31; //Z probe error msg
        }
        Serial.println("Z Axis probe");
        return;

}




//=======================================================================
//                    Read UGS output
//=======================================================================
void readUGSPendant(){
        HTTPClient http; //Declare object of class HTTPClient
        String getFull = "http://" + webserver + ":" + webserverport + GETsystemState;
        http.begin(getFull);
        int httpCode = http.GET();
        String payload = http.getString(); //Get the response payload
        //Serial.println(payload);    //Print request response payload
        if(httpCode < 0) {//No connection to cnc
                disableAlarmLockLogic = 1; //will remove alarm on reconnect
                if(cnc_state[0] == 1) {
                        cnc_state[0] = pingWebsite();//if 0 ping is successful, if 1 ping failed
                }
                if(cnc_state[0] == 0) {//if there is internet connection, check if PC 2 is available
                        bool result;//check if PC 2 is available
                        if(button[15] == 0) {
                                result = Ping.ping(webserver1, 1);//check if PC 2 is available
                        }else if(button[15] == 1) {
                                result = Ping.ping(webserver2, 1);//check if PC 2 is available
                        }
                        if(result == 1) {//it is available - so UGS (and or wifi pendant) is not running
                                cnc_state[0] = 0;
                                cnc_state[1] = 0;
                                cnc_state[2] = 1;//PC 2 is available, but ugs or pendant are not
                        }else{//PC 2 is not available
                                cnc_state[0] = 0;
                                cnc_state[1] = 1;//PC 2 is not available
                                cnc_state[2] = 0;
                        }
                }else{//ESP doesn't have internet connection
                        cnc_state[0] = 1;//no internet connection
                        cnc_state[1] = 0;
                        cnc_state[2] = 0;
                }
                cnc_state[3] = 0;
                cnc_state[4] = 0;
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
                displayLCD();
        }else{
                parseString(payload);
        }
        http.end(); //Close connection
        return;
}
