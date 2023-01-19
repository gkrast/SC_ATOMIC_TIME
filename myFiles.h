/*
This class header is for Settings storage
*/
#pragma once
#ifndef _MYFILES_h
#define _MYFILES_h

#include "app.h"				// Application specific definitions


class myConfigFile
{
public:
	/////////////////////////////// Properties

	//Config Data stored in the Config file

	// Wi-Fi access points credentials
	NetLogin wifi_nets_login[3];

	//List of three available local WiFi networks
	LocalNet wifi_local_nets[3];

	//Time offset for RTC
	int time_offset = -7; // GMT-7

	//Display orientation
	bool lcd_orientation_normal = true;
	
	//<< add more setting here

	/////////////////////////////// Methods
	myConfigFile();
	~myConfigFile();

	void Init();
	bool read();
	bool write();
	String config_file();

private:
	void setProps(String json);
	String getProps();
};


extern myConfigFile settings;			// Configuration file with settings
#endif