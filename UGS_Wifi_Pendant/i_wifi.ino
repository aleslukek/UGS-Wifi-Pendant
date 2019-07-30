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
        #ifndef BUTTONSTOSERIAL
        Serial.println("");
        #endif

        #ifndef BUTTONSTOSERIAL
        Serial.print("Connecting");
        #endif
        // Wait for connection
        while (WiFi.status() != WL_CONNECTED && Serial.available() > 0) {
                delay(500);
                Serial.print(".");
        }

        //If connection successful show IP address in serial monitor
        #ifndef BUTTONSTOSERIAL
        Serial.println("");
        Serial.print("Connected to ");
        Serial.println(ssid);
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP()); //IP address assigned to your ESP
        #endif
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
#ifndef BUTTONSTOSERIAL
void getCommandGeneral(String getCommand){
        if(getCommand == GETcancel && directionStatus == 1) { //if moving, set directionStatus as 0 when canceling the movement
                directionStatus = 0;
                justSentGeneral = 0;
                justSentDirection = 0;
        }



        #ifdef OLD_PENDANT
        if(justSentGeneral != 1 && getCommand != GETtoggleLaser) {
                String getFull = "http://" + webserver + ":" + webserverport + getCommand;
                getSendCommand(getFull);
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
                getSendCommand(getFull);
                if(laserMode == 1) { //for some reason it works if you move G1 for a bit and then M3 and M5 works even when not moving
                        getFull = "http://" + webserver + ":" + webserverport + GETlaserTestPower;
                        getSendCommand(getFull);
                        delay(100);
                        getFull = "http://" + webserver + ":" + webserverport + GETlaserTestOff;
                        getSendCommand(getFull);
                        delay(100);
                        getFull = "http://" + webserver + ":" + webserverport + GETmoveLeftABit;
                        getSendCommand(getFull);
                        delay(200);
                        getFull = "http://" + webserver + ":" + webserverport + GETmoveRightABit;
                        getSendCommand(getFull);
                }else if(laserMode == 0) {
                        delay(100);
                        getFull = "http://" + webserver + ":" + webserverport + GETlaserTestOff;
                        getSendCommand(getFull);
                }
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
                getSendCommand(getFull);
                timer1 = millis();
                timer5 = millis();
                justSentGeneral = 1;
                justSentMsgDisplay = 1;
        }else{
        }
        #endif
        Serial.print("General command: ");
        Serial.println(getCommand);
        return;
}
#endif




//=======================================================================
//                    Send Get Command
//=======================================================================
int getSendCommand(String getUrl){
        HTTPClient http;   //Declare object of class HTTPClient
        int returnCode;
        http.begin(getUrl);
        returnCode = http.GET();
        http.end();   //Close connection
        return returnCode;
}




//=======================================================================
//                    Sent Commands via Serial for BUTTONSTOSERIAL
//=======================================================================
#ifdef BUTTONSTOSERIAL
void sendCommandViaSerial(String getUrl, bool postOrGet, String postContent){
        Serial.print("BEGIN");
        Serial.println();
        Serial.print(postOrGet); //POST
        Serial.println();
        Serial.print(postContent); //body
        Serial.println();
        Serial.print(getUrl); //url
        Serial.println();
        Serial.print("END");
        Serial.println();
        return;
}



//=======================================================================
//                    Get Command set-up for BUTTONSTOSERIAL
//=======================================================================
void getCommandGeneral(String getCommand){
        if(getCommand == GETcancel && directionStatus == 1) { //if moving, set directionStatus as 0 when canceling the movement
                directionStatus = 0;
                justSentGeneral = 0;
                justSentDirection = 0;
        }
        String getFull = localhost;
        getFull = getFull + webserverport;
        if(justSentGeneral != 1) {
                getFull = getFull + getCommand;
                sendCommandViaSerial(getFull);
                timer1 = millis();
                timer5 = millis();
                justSentGeneral = 1;
                justSentMsgDisplay = 1;
        }else{
        }
        return;
}
#endif



//=======================================================================
//                    Send Jog Command GET method
//=======================================================================
#ifdef OLD_PENDANT
void getDirection(int xdirection, int ydirection, int zdirection, bool slow){
        //printSerialDebug(0);
        Serial.print("get direction, ");
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
                getCommandGeneral(getFull);
                timer1 = millis();
                timer5 = millis();
                justSentDirection = 1;
                justSentMsgDisplay = 1;
                directionStatus = 1;
                Serial.println("Direction command done");
        }
        return;
}
#endif




//=======================================================================
//                    Send Jog Command POST method - New Pendant
//=======================================================================
#ifndef OLD_PENDANT
void postDirection(int xdirection, int ydirection, int zdirection, bool slow){
        int step = xyStepSizeFast;
        int rate = xyStepRateFast;
        if(cnc_state[5] == 1 || cnc_state[14] == 1) {        //if job is running, ignore commands
                cnc_state[14] = 1;
                buttonPressMsg = 120;
                return;
        }else{
                if(slow == 1) {
                        step = xyStepSizeSlow;
                        rate = xyStepRateSlow;
                }else{
                        step = xyStepSizeFast;
                        rate = xyStepRateFast;
                }
                String direction = "";
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
                String postContent = POSTmovexyz;
                postContent = postContent + " " + direction + step + " F" + rate;
                postSendGcode(postContent);
                timer1 = millis();
                timer5 = millis();
                justSentDirection = 1;
                justSentMsgDisplay = 1;
                directionStatus = 1;
        }
        return;
}
#endif



//=======================================================================
//                    Send Gcode with POST (new pendant)
//=======================================================================
#ifndef OLD_PENDANT
void postSendGcode(String command){
        #ifdef NEW_BREILERS_PENDANT
        HTTPClient http;
        String postUrl = "http://" + webserver + ":" + webserverport;
        #endif
        #ifdef BUTTONSTOSERIAL
        String postUrl = localhost + webserverport;
        #endif
        bool getLaserToggle = 0;
        if(command == GETtoggleLaser) {
                postUrl = postUrl + POSTgcode;
                laserMode = !laserMode;
                if(laserMode == 1) {
                        command = GETtoggleLaser2On;
                }else{
                        command = GETtoggleLaser2Off;
                }
        }else if(command == POSTsendResume) { //send, resume
                postUrl = postUrl + POSTsendResume;
        }else{
                postUrl = postUrl + POSTgcode;
        }

        #ifdef NEW_BREILERS_PENDANT
        http.begin(postUrl);
        http.addHeader("content-type","application/json");
        if(command != POSTsendResume) {
                String postContent = "{\"commands\"";
                postContent = postContent + ":" + command + "}";
                int httpCode = http.POST(postContent);
        }else{
                int httpCode = http.POST("");
        }
        http.end(); //Close connection
        #endif
        #ifdef BUTTONSTOSERIAL
        sendCommandViaSerial(postUrl, 1, command);
        #endif
        return;
}
#endif





//=======================================================================
//                    Z Axis probe
//=======================================================================
#ifdef OLD_PENDANT
void zAxisProbe(){
        if(resetZAxisAfterProbe == 0 && cnc_state[4] == 1) {//Not yet probed, start probing
                //G91 G38.2 Z-20 F100 -> to activate Probe
                String getFull = "http://" + webserver + ":" + webserverport + "/sendGcode/?gCode=G91%20G38.2%20Z-" + probeStep + "%20F" + probeRate;
                getCommandGeneral(getFull);
                timer1 = millis();
                timer5 = millis();
                justSentDirection = 1;
                justSentMsgDisplay = 1;
                resetZAxisAfterProbe = 1;
                Serial.println("Sending Z probe move commands");
        }else if(resetZAxisAfterProbe == 1 && (cnc_state[8] == 0 || cnc_state[9] == 0) && cnc_state[4] == 1) {//Probe finished without alarm, reset Z axis for probe height
                delay(500);
                //G92 Z2.0 -> reset z height, for probe plate height (2mm)
                String getFull = "http://" + webserver + ":" + webserverport + GETresetZzero;
                if(laserMode == 0) {
                        getFull = getFull + drillProbeHeight; //different probe height offset for drilling
                }else if(laserMode == 1) {
                        getFull = getFull + laserProbeHeight; //different probe height offset for laser
                }
                getCommandGeneral(getFull);
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
#endif




//=======================================================================
//                    Z Axis probe
//=======================================================================
#ifndef OLD_PENDANT
void zAxisProbe(){
        if(resetZAxisAfterProbe == 0 && cnc_state[4] == 1) {//Not yet probed, start probing
                //G91 G38.2 Z-20 F100 -> to activate Probe
                String zprobeStart = "G91 G38.2 Z-";
                zprobeStart = zprobeStart + probeStep + " F" + probeRate;
                postSendGcode(zprobeStart);
                timer1 = millis();
                timer5 = millis();
                justSentDirection = 1;
                justSentMsgDisplay = 1;
                resetZAxisAfterProbe = 1;
                Serial.println("Sending Z probe move commands");
        }else if(resetZAxisAfterProbe == 1 && (cnc_state[8] == 0 || cnc_state[9] == 0) && cnc_state[4] == 1) {//Probe finished without alarm, reset Z axis for probe height
                delay(500);
                String postFull = POSTresetZzero;
                if(laserMode == 0) {
                        postFull = postFull + drillProbeHeight; //different probe height offset for drilling
                }else if(laserMode == 1) {
                        postFull = postFull + laserProbeHeight; //different probe height offset for laser
                }
                postSendGcode(postFull);
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
#endif



//=======================================================================
//                    Read UGS output
//=======================================================================
void readUGSPendant(){
        String getFull = "http://" + webserver + ":" + webserverport + GETsystemState;
        HTTPClient http;         //Declare object of class HTTPClient
        int httpCode;
        http.begin(getFull);
        httpCode = http.GET();
        String payload = http.getString();         //Get the response payload
        //Serial.println(payload);    //Print request response payload
        http.end();         //Close connection
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
                                cnc_state[2] = 1;//PC is available, but ugs or pendant are not
                        }else{//PC is not available
                                cnc_state[0] = 0;
                                cnc_state[1] = 1;//PC is not available
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
        return;
}




//=======================================================================
//                    IFTTT Message
//=======================================================================
void iftttMessage(String message){
        if(enableIftttMessaging == true) {
                delay(100);
                String getFull = "http://maker.ifttt.com/trigger/";
                getFull = getFull + iftttEventName;
                getFull = getFull + "/with/key/";
                getFull = getFull + iftttKey;
                getFull = getFull + "?value1=";
                getFull = getFull + message;
                getCommandGeneral(getFull);
                timer1 = millis();
                justSentGeneral = 1;
                #ifndef BUTTONSTOSERIAL
                Serial.print("Ifttt message sent: ");
                Serial.println(message);
                #endif
                iftttSend = 0;
        }else{
                return;
        }
        return;
}




//=======================================================================
//                    OTA Setup function
//=======================================================================
void otaSetup(){
        if(enableOTAPassword == true) {
                ArduinoOTA.setPassword((const char *)OTAPassword); //To be able to OTA update, you must enter a password if enableOTAPassword is enabled
        }
        ArduinoOTA.onStart([]() {
                #ifndef BUTTONSTOSERIAL
                Serial.println("OTA Start");
                #endif
        });
        ArduinoOTA.onEnd([]() {
                #ifndef BUTTONSTOSERIAL
                Serial.println("\n OTA End");
                #endif
        });
        ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
                #ifndef BUTTONSTOSERIAL
                Serial.printf("Progress: %u%% \r", (progress / (total / 100)));
                #endif
        });
        ArduinoOTA.onError([](ota_error_t error) {
                #ifndef BUTTONSTOSERIAL
                Serial.printf("OTA Error[%u]: ", error);
                if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
                else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
                else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
                else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
                else if (error == OTA_END_ERROR) Serial.println("End Failed");
                #endif
        });
        ArduinoOTA.begin();
}




//=======================================================================
//                    Connect CNC to computer
//=======================================================================
#ifndef OLD_PENDANT
void connectCNC(){
        delay(1000);
        HTTPClient http;

        #ifdef NEW_BREILERS_PENDANT
        String postUrl = "http://" + webserver + ":" + webserverport + POSTSelectUSBPort + USBport;
        http.begin(postUrl);
        http.addHeader("content-type","application/json");
        int httpCode = http.POST("");
        http.end(); //Close connection
        #endif
        #ifdef BUTTONSTOSERIAL
        String postUrl = localhost + webserverport + POSTSelectUSBPort + USBport;
        sendCommandViaSerial(postUrl, 1, "");
        #endif

        delay(2000);
        getCommandGeneral(GETconnect);
        delay(5000);
        connectCNCOnce = 1;
}
#endif
