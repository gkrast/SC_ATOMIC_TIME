#include "myRTC.h"

myRTC RTC;


// Get and set the offsets in minutes
int  myRTC::getOffset() {
	return _minutes_offset;
}
void  myRTC::setOffset(int minutes_offset) {
	_minutes_offset = minutes_offset;
}

//Begin the RTC process by obtaing the servers IP and sending the first packet
void myRTC::Init() {
	// Start the time keeping process with NTP server
	RTC.setOffset(settings.time_offset * 60);
	for (int i = 0; i < 4; i++) {
		UDP.begin(123);
		WiFi.hostByName(NTPSERVER[i].c_str(), ServerIP);// Get the IP address of the NTP server
		sendRequest();
		delay(6000); // wait 6 sec for Time Server response
		timeServer = getData();
		if (timeServer > 0) break;
	}
	if (timeServer == 0) { // server not available error
		timeServerError = true;
		time.UTC = 0;
		time.local = 0;		
		cout << "Time Server Error!" << "\r" << endl;
		LCD.showText("Time Server Error!");
		Error = (!Error) ? TIME_SERVER_ERROR : Error;

	}
	else {
		timeServerError = false;
		time.UTC = timeServer;
		time.local = time.UTC + _minutes_offset * 60;
		cout << "Time server found. RTC Started" << "\r" << endl;

		time.strUTC= makeTimeString(time.UTC);
		time.strLocal = makeTimeString(time.local);

		cout << "UTC Time: " << time.strUTC.c_str() << "\r" << endl;
		cout << "Local Time: " << time.strLocal.c_str() << "\r" << endl;

		LCD.showText("Real Time Clock: OK");
	}
	lastMillis = millis(); // set base for relative time
}

//Returns the time values in the UNIX format
//If the time is in 1970 year it means it is  expressed in relative units (seconds) from the system reset
timeVals* myRTC::getTime() {
	
	struct tm* gmt = gmtime(&time.UTC);
	int seconds = 0;

	if (millis() < lastMillis) {
		// handle overflow of millisecond counter (once approx every 50 days)
		diffTime = (0xFFFFFFFFUL - lastMillis) + millis();
	}
	else {
		if (lastMillis != 0) {
			diffTime = millis() - lastMillis;
		}
		else {
			diffTime = 0;
		}	
	}

	// Update time variables
	lastMillis = millis();
	accumTime += diffTime;
	if (accumTime / 1000 > 1) {
		updateInterval += accumTime / 1000;
		seconds = accumTime / 1000;
		accumTime = diffTime % 1000;		
	}

	
	// Send Time Server request now if the time is wrong or every INTERVAL sec
	if ((gmt->tm_year + 1900) < 2023) {
		updateInterval = 0;
		sendRequest();
	} else if (updateInterval > INTERVAL) {
		updateInterval = 0;
		sendRequest();
	}
	

	// Parse the response
	timeServer = getData();
	if (timeServer) {
		time.UTC = timeServer;
		time.local = time.UTC + _minutes_offset * 60;
		timeServerError = false;
		time.strUTC = makeTimeString(time.UTC);
		time.strLocal = makeTimeString(time.local);
		cout << "Time updated from server" << "\r" << endl;
	}
	else {
		if (seconds>0) {
			time.UTC += seconds;
			time.local = (!timeServerError) ? time.UTC + _minutes_offset * 60 : time.UTC;
			//cout << "[Time]: " << time.strUTC.c_str() << " [Sec]: " << secondCtr << endl;
			time.strUTC = makeTimeString(time.UTC);
			time.strLocal = makeTimeString(time.local);
		}
	}
	return &time;
}

/////////////////////// PRIVATE FUNCTIONS /////////////////////////////////////////////

// Send data request to the Time Server
void myRTC::sendRequest() {
	memset(NTPBuffer, 0, 48);  // set all bytes in the buffer to 0
	// Initialize values needed to form NTP request
	NTPBuffer[0] = 0b11100011;   // LI, Version, Mode
	// send a packet requesting a timestamp:
	UDP.beginPacket(ServerIP, 123); // NTP requests are to port 123
	UDP.write(NTPBuffer, 48);
	UDP.endPacket();
}

// Convert time value into a string Value
String myRTC::makeTimeString(time_t timeVal) {
	char str[19];
	struct tm* gmt = gmtime(&timeVal);
	sprintf(str, "%02d-%02d-%02d %02d:%02d:%02d", gmt->tm_year + 1900,gmt->tm_mon + 1, gmt->tm_mday, gmt->tm_hour, gmt->tm_min, gmt->tm_sec);
	return String(str);
}

// Conver time value into time string
String myRTC::makeStrTime(time_t timeVal) {
	char str[8];
	struct tm* gmt = gmtime(&timeVal);
	sprintf(str, "%02d:%02d:%02d", gmt->tm_hour, gmt->tm_min, gmt->tm_sec);
	return String(str);
}

// Conver time value into date string
String myRTC::makeStrDate(time_t timeVal) {
	char str[10];
	struct tm* gmt = gmtime(&timeVal);
	sprintf(str, "%02d-%02d-%02d", gmt->tm_year + 1900, gmt->tm_mon + 1, gmt->tm_mday);
	return String(str);
}

// Get the UNIX time returned by the Time Server
uint32_t myRTC::getData() {
	if (UDP.parsePacket() == 0) { // If there's no response (yet)
		return 0;
	}
	UDP.read(NTPBuffer, 48); // read the packet into the buffer
	// Combine the 4 timestamp bytes into one 32-bit number
	uint32_t NTPTime = (NTPBuffer[40] << 24) | (NTPBuffer[41] << 16) | (NTPBuffer[42] << 8) | NTPBuffer[43];
	// Convert NTP time to a UNIX timestamp:
	// Unix time starts on Jan 1 1970. That's 2208988800 seconds in NTP time:
	const uint32_t seventyYears = 2208988800UL;
	// subtract seventy years:
	uint32_t UNIXTime = NTPTime - seventyYears;
	return UNIXTime;
}
