/*
myRTC.h
*/
#pragma once
#ifndef _MYRTC_h
#define _MYRTC_h

#include "app.h"				//application specifica definitions

class myRTC
{	
public:	

	bool timeServerError = true;
	void Init();
	timeVals* getTime();	//Function to be called in the main loop to read the time
	int getOffset();		//Gets the time offset in minutes
	void setOffset(int minutes_offset);	// sets the time offset in minutes
	String makeTimeString(time_t timeVal);
	String makeStrTime(time_t timeVal);
	String makeStrDate(time_t timeVal);

private:
	// Variables
	WiFiUDP UDP;	
	IPAddress ServerIP;	
	byte NTPBuffer[48];
	int _minutes_offset = 0;
	unsigned long diffTime = 0;
	unsigned long lastMillis = 0;
	unsigned long accumTime = 0;
	unsigned long updateInterval = 0;
	uint32_t timeServer = 0;
	timeVals time;
	// Functions
	
	void sendRequest();
	uint32_t getData();
	
};

extern myRTC RTC;
#endif
