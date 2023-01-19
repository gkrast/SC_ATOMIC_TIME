#include "myServer.h"

ESP8266WiFiMulti wifiMulti;		// Wi-Fi Connection class for multiple access points
myServerClass myServer;			// This class


// Initialize the Wi-Fi connection
void myServerClass::WiFi_Init() {

	LCD.showText("Searching for networks...");
	// Scan  local Networks and get the first three with the best RSSI
	int n = WiFi.scanNetworks();
	int k = 0;
	for (int i = 0; i < n; i++) {
		cout << "Found Net:  " << WiFi.SSID(i).c_str() << endl;
		cout << "RSSI:  " << WiFi.RSSI(i) << endl;
		if (WiFi.RSSI(i) > -75) {
			settings.wifi_local_nets[k].SSID = WiFi.SSID(i);
			settings.wifi_local_nets[k].RSSI = WiFi.RSSI(i);
			LCD.showText(settings.wifi_local_nets[k].SSID + " @ " + String(WiFi.RSSI(i)));
			k++;
			if (k == 3) break; // hold only three names
		}
	}

	LCD.showText("Connecting to network...");
	wifiMulti.cleanAPlist();
	WiFi.disconnect();
	WiFi.mode(WIFI_STA);// Set in station mode
	WiFi.setOutputPower(20.5); // max power
	delay(1000); // required delay for power stabilization

	// Add to the connection array three nets by  Name and Password
	if (settings.wifi_nets_login[0].Name != "" && settings.wifi_nets_login[0].Pswd != "") {
		wifiMulti.addAP(settings.wifi_nets_login[0].Name.c_str(), settings.wifi_nets_login[0].Pswd.c_str());
	}
	if (settings.wifi_nets_login[1].Name != "" && settings.wifi_nets_login[1].Pswd != "") {
		wifiMulti.addAP(settings.wifi_nets_login[1].Name.c_str(), settings.wifi_nets_login[1].Pswd.c_str());
	}
	if (settings.wifi_nets_login[2].Name != "" && settings.wifi_nets_login[2].Pswd != "") {
		wifiMulti.addAP(settings.wifi_nets_login[2].Name.c_str(), settings.wifi_nets_login[2].Pswd.c_str());
	}
	
	// Wait for the Wi-Fi to connect: scan for Wi-Fi networks, and connect to the strongest of the networks above
	int i = 0;
	while ((wifiMulti.run() != WL_CONNECTED) && (i < 10)) {
		delay(1000);
		i++;
	}

	// Check for WiFi connection status
	myServer.WiFiConnected = (i == 10) ? false : true;

	if (myServer.WiFiConnected) {
		cout << "Connected to: " << WiFi.SSID().c_str() << endl;
		cout << "IP address: " << WiFi.localIP().toString().c_str() << endl;
	}
	else {
		cout << "Cannot connect to WiFi!" << endl;
	}

	if (myServer.WiFiConnected) {
		// Find the password for the connected network
		for (int i = 0; i < 3; i++) {
			if (WiFi.SSID() == settings.wifi_nets_login[i].Name) {
				myServer.wifi_pswd = settings.wifi_nets_login[i].Pswd;
				break;
			};
		}		
		LCD.showText("SSID: " + WiFi.SSID());
		LCD.showText("IP: " + WiFi.localIP().toString());
		LCD.showText("WiFi: OK");
	}
	else {
		LCD.showText("WiFi ERROR!");
		Error = (!Error) ? WIFI_ERROR : Error;
	}

}


