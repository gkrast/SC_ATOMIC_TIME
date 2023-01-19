#include "myTerminal.h"


void MyTerminalClass::init()
{
	mode = COMMAND;

}

// Reads the config file
void MyTerminalClass::config() {
	cout << "CONFIG_JSON" << "\r" << endl;
	cout << "-----------------------------------------------------------------" << "\r" << endl;
	cout << settings.config_file().c_str() << "\r" << endl;
	cout << "-----------------------------------------------------------------" << "\r" << endl;
}

//Resets the MCU
void MyTerminalClass::reset() {
	cout << "Resetting.." << "\r" << endl << endl;
	ESP.reset();
}

// Flip the vertical orientation of the LCD
void MyTerminalClass::flip() {
	LCD.flipScreen();
	cout << "LCD flipped" << "\r" << endl;
}

//show list of commands and descriptions
void MyTerminalClass::help() {
	cout << endl << "TERMINAL COMMANDS" << "\r" << endl;
	cout << "-------------------------------------------------------------------------" << "\r" << endl;
	cout << "help:		Shows this menu" << "\r" << endl;
	cout << "connect:	Connect to local wi-fi network" << "\r" << endl;
	cout << "gmt:		Set time zone offset" << "\r" << endl;
	cout << "reset:		Reset the device" << "\r" << endl;
	cout << "config:          Read the configuration" << "\r" << endl;
	cout << "flip:		Flip the orientation of the LCD" << "\r" << endl;
	cout << "-------------------------------------------------------------------------" << "\r" << endl;
}

// List the top three wi-fi networks
void MyTerminalClass::nets() {
	cout << "Select Net number:" << "\r" << endl;
	for (int i = 0; i < 3; i++) {
		cout << i + 1 << ". Net:  " << settings.wifi_local_nets[i].SSID.c_str() << ", RSSI: " << settings.wifi_local_nets[i].RSSI << "\r" << endl;
	}
	mode = SELECT_NET;
}

// Set the time zone
void MyTerminalClass::gmt() {
	cout << "Enter Time Zone from -12 to +14:" << "\r" << endl;
	mode = GMT;
}

// Command Interpreter
void MyTerminalClass :: exe(String com) {
	int i;
	static int sel;
	if (mode == COMMAND || mode == HELP) {
		cout << "Executing command < " << com.c_str() << " >" << "\r" << endl;
		com.toUpperCase();
		if (com == "HELP") {
			help();
			mode = HELP;
		}
		else if (com.startsWith("FLIP")) {
			flip();
		}
		else if (com.startsWith("RESET")) {
			reset();
		}
		else if (com.startsWith("CONFIG")) {
			config();
		}
		else if (com.startsWith("CONNECT")) {
			nets();
		}
		else if (com.startsWith("GMT")) {
			gmt();
		}

		else if (true) {
			cout << "Command not found!" << "\r" << endl;
			mode = COMMAND;
		}
	}
	else {
		switch (mode) {
			case GMT:
				settings.time_offset = com.toInt();
				settings.write();
				RTC.setOffset(settings.time_offset * 60);
				cout << "Time zone updated: < " << com.c_str() << " >" << "\r" << endl;
				mode = COMMAND;
				break;
			case SELECT_NET:
				sel = com.toInt() - 1;
				cout << "Selected network: < " << settings.wifi_local_nets[sel].SSID.c_str() << " >" << "\r" << endl;
				cout << "Enter Password:" << "\r" << endl;
				mode = SET_PASSWORD;
				break;
			case SET_PASSWORD:
				for (i = 0; i < 3; i++) {
					if (settings.wifi_nets_login[0].Name == "") break;
				}
				if (i == 3) i = 0;
				settings.wifi_nets_login[i].Name = settings.wifi_local_nets[sel].SSID;
				settings.wifi_nets_login[i].Pswd = com;
				settings.write();
				cout << "Password updated:" << "\r" << endl;
				cout << "WiFi Net-"<< i + 1 << "[Name]: " << settings.wifi_nets_login[i].Name.c_str() << "\r" << endl;
				cout << "WiFi Net-" << i + 1 << "[Psw]: " << settings.wifi_nets_login[i].Pswd.c_str() << "\r" << endl;
				reset();
				break;
		}
	}
}

MyTerminalClass myTerminal;

