//=======================================================================
//                    Main Program Setup
//=======================================================================
void setup() {
        delay(100);
        lcd.begin(16,2);     // The begin call takes the width and height. This
        lcd.init();          // Should match the number provided to the constructor.
        lcd.backlight();     // Turn on the backlight.
        lcdBacklightStatus = 1;
        if(enableSerialOutput == true) { //this will disable serial if not enabled
                Serial.begin(115200);
        }
        WiFi.mode(WIFI_OFF); //Prevents reconnection issue (taking too long to connect)
        WiFi.disconnect();
        delay(100);
        WiFi.mode(WIFI_STA); //This line hides the viewing of ESP as wifi hotspot

        WiFi.begin(ssid, password); //Connect to your WiFi router
        Serial.println("");
        if(enableResetPhyPin == true) {
                pinMode(resetPinD5, OUTPUT);
        }
        digitalWrite(resetPinD5, HIGH);
        pinMode(ledLaser, OUTPUT);
        digitalWrite(ledLaser, HIGH);
        pinMode(ledSlow, OUTPUT);
        digitalWrite(ledSlow, HIGH);
        pinMode(ploadPin, OUTPUT);
        pinMode(clockEnablePin, OUTPUT);
        pinMode(clockPin, OUTPUT);
        pinMode(dataPin, INPUT);
        buttonValues = read_shift_regs();
        serialPrintInfo();
        oldButtonValues = buttonValues;
        Serial.println();
        Serial.print("Connecting");
        lcd.setCursor(0, 0);
        lcd.print(lcdMsgConnecting);
        lcd.setCursor(0, 1);
        if(button[15] == 0) {
                lcd.print("( ");
                lcd.print(controler1Name);
                lcd.print(" )");
        }else if(button[15] == 1) {
                lcd.print("( ");
                lcd.print(controler2Name);
                lcd.print(" )");
        }
        // Wait for connection
        while (WiFi.status() != WL_CONNECTED) {
                delay(500);
                Serial.print(".");
        }
        //If connection successful show IP address in serial monitor
        Serial.println("");
        Serial.print("Connected to ");
        Serial.println(ssid);
        Serial.print("IP address: ");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(lcdMsgConnected);

        Serial.println(WiFi.localIP()); //IP address assigned to your ESP
        if(enableOTAUpdate == true) {
                otaSetup(); //Prepare for OTA update;
        }
        timer2 = millis();
        timer3 = millis();
        if(button[15] == 0) {
                Serial.println("Searching for PC1 (button not pressed).");
        }else{
                Serial.println("Searching for PC2 (button pressed).");
        }
        if(button[15] == 0) {
                cnc_state[1] = Ping.ping(webserver1, 1);//check if PC 1 is available
        }else if(button[15] == 1) {
                cnc_state[1] = Ping.ping(webserver2, 1);//check if PC 2 is available
        }
        digitalWrite(ledSlow, LOW);
        digitalWrite(ledLaser, LOW);
        Serial.println("Setup finished");
}
