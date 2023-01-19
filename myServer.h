// myServer.h

#ifndef _MYSERVER_h
#define _MYSERVER_h

#include "app.h"

class myServerClass {
	
public:
	bool WiFiConnected = false;				// State of Wi-Fi connection
	String wifi_pswd;						// Current wifi connection password	
	void WiFi_Init();
};

extern myServerClass myServer;

#endif

