#pragma once
#ifndef _APP_h
#define _APP_h

// C++ libraries
#include <fstream>
#include <iostream>
#include <bitset>
#include <iomanip>
#include <string>
#include <time.h>				
using namespace std;			// standard c++ library for cout <<

#include <Arduino.h>			// Arduino core library

// Global defines
#define APP "Time"
#define REV "1.0.0"			   // Update this number when the version is changed

#define MAX_BUFF 6			   // size of LCD  buffer

// Time server info
constexpr auto INTERVAL = 60;  // 1 min interval of time requests
const string  NTPSERVER[4] = { 
	"time-a-b.nist.gov",
	"time-b-b.nist.gov",
	"time-c-b.nist.gov",
	"time-d-b.nist.gov"
};

// Global Error codes
enum Errors {
	NO_ERROR,
	WIFI_ERROR,
	SPIFF_ERROR,
	TIME_SERVER_ERROR
	// add more here
};
extern Errors Error;

// Enumerator for font size on the LCD
enum font_sizes {
	SMALL,
	MEDIUM,
	LARGE
};

// Signal Strength Values
enum RSSI_vals {
	NONE,
	WEAK,
	FAIR,
	GOOD
};

// Global structure for time values
struct timeVals {
	time_t local;
	time_t UTC;
	String strLocal;
	String strLocalTime;
	String strLocalDate;
	String strUTC;
};


// Network logins
struct NetLogin {
	String Name;
	String Pswd;
};

// Local Net SSID and RSSI
struct LocalNet {
	String SSID;
	int RSSI;
};

// Special Arduino libraries
//#include <base64.h>				// Encoding binary data
#include <FS.h>					// File system library
#include <ESP8266WiFi.h>        // Include the Wi-Fi library
#include <ESP8266WiFiMulti.h>   // Include the Wi-Fi-Multi library
#include <WiFiUdp.h>			// UDP protocol for talking to Time Server with Network Time Protocol(NTP)
#include <FS.h>					// File system library
#include <ArduinoJson.hpp>		// JSON serialization
#include <ArduinoJson.h>		// JSON serialization
#include <Wire.h>               // I2C function
#include <SSD1306Wire.h>        // SSD1306 LCD library


// Application Classes
#include "myLCD.h"				// LCD operations
#include "fonts.h"				// Bits of fonts
#include "myFiles.h"			// File operations for logging and settings
#include "myRTC.h"				// RTC libray functions
#include "myServer.h"			// All server operations
#include "myTerminal.h"			// Termimal command interface

// Global variabls declared in the INO file
extern ESP8266WiFiMulti wifiMulti;		// Wi-Fi Connection class for multiple access points

#endif
