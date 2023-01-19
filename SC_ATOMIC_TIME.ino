/* The Public License
 ******************************************************************************
 *                                                                            *
 * Copyright (c) 2023 by Senzotic											  *
 *                                                                            *
 ******************************************************************************
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.

	This code demonstrates the functionality of the SVR-A Senzotic module
	----------------------------------------------------------------------------------------------------------
	Atomic Time Application description:
	----------------------------------------------------------------------------------------------------------
	During start up the program shows initialization  messages on the display.
	The Wi-Fi radio of the ESP8266 is initialized and then an attempt is made to connect to one of three local networks.
	If the connection is successful the device calls one of four time servers to obtain accurate time information.
	The time and date is then displayed on the LCD and sent to the UART port every 500msec,so that the green LED is periodically flashing
	The User may configure the connection to the Wi-Fi network with a terminal command "connect" sent over the UART port.
	UART settings: 9600, 8 bits, No Parity, One stop bit, No flow control
	This application code is provided as is without any warrantee. Its sole purpose is to help in the development of applications
	using the SVR-A module.
*/

// LCD driver libraries
#include <SSD1306Wire.h>
#include <OLEDDisplayUi.h>
#include <OLEDDisplayFonts.h>
#include <OLEDDisplay.h>


// Driver for serial interfaces
#include <SPI.h>				// SPI functions
#include <Wire.h>				// IIC functions and more

// Other Libraries
#include <FS.h>					// File system library
#include <ESP8266WiFi.h>        // Include the Wi-Fi library
#include <ESP8266WiFiMulti.h>   // Include the Wi-Fi-Multi library
#include <ArduinoJson.hpp>		// JSON serialization
#include <ArduinoJson.h>		// JSON serialization


#include "app.h"				// Application classes

//Global Vars
Errors Error;				    // Current error

/////////////////////////////////////////////////////////////////////
//                  APPLICATION INITIALIZATION                     //
/////////////////////////////////////////////////////////////////////

void setup() {
	
	Serial.begin(9600);
	cout << "Initilizing..." << "\r" << endl << endl;

	settings.Init();  
	LCD.Init();
	LCD.appRev();
	LCD.setTitle("Initializing...");
	LCD.setHoldTime(500);	
	myTerminal.init();
	
	myServer.WiFi_Init();	

	if (myServer.WiFiConnected) {
		RTC.Init();			
	}
	
	LCD.clearLCD();
}

/////////////////////////////////////////////////////////////////////
//                       MAIN LOOP                                 //
/////////////////////////////////////////////////////////////////////

void loop() {

	// Check for serial commands and execute after Enter (LF+CR)
	String buf;
	static String com;
	if (Serial.available()) {
		buf = Serial.readString();
		com += buf;
		if (com.endsWith("\n")) {
			com = com.substring(0, com.length() - 2);
			myTerminal.exe(com);
			com = "";
		}
	}

	// Get current time
	timeVals*  theTime = RTC.getTime();
	
	// Update the RSSI value for display on the LCD
	if (!myServer.WiFiConnected) {
		LCD.setRSSI(-100);
	}
	else {
		LCD.setRSSI(WiFi.RSSI());
	}	

	// Set display buffer
	String LCD_Buffer[MAX_BUFF];
	LCD_Buffer[0] = RTC.makeStrTime(theTime->local);
	LCD_Buffer[1] = RTC.makeStrDate(theTime->local);
	LCD.loadBuffer(LCD_Buffer);
	
	LCD.showData();

	// Send time info to the serial port
	if (myTerminal.mode == COMMAND) {
		cout << theTime->strLocal.c_str() << "\r" << endl;
	}

}  // end of main loop


