/*
This class implementation is for the configuration settings

*/

#include "myFiles.h"

myConfigFile settings;			// Configuration file with settings

//Constructor
myConfigFile::myConfigFile() {
}

//Destructor
myConfigFile::~myConfigFile() {
	SPIFFS.end();	
}

//Class Initializations - must be used once in the Setup function of the Adruino sketch
void myConfigFile::Init() {
	SPIFFS.begin();
	SPIFFS.gc();

	// get config settings from the config file
	if (!read()) {
		Error = (!Error) ? SPIFF_ERROR : Error;
	};

	cout << "Configuration file values:" << "\r" << endl << endl;
	cout << "WiFi Net-1 [Name]: " << settings.wifi_nets_login[0].Name.c_str() << "\r" << endl;
	cout << "WiFi Net-1 [Psw]: " << settings.wifi_nets_login[0].Pswd.c_str() << "\r" << endl;
	cout << "WiFi Net-2 [Name]: " << settings.wifi_nets_login[1].Name.c_str() << "\r" << endl;
	cout << "WiFi Net-2 [Psw]: " << settings.wifi_nets_login[1].Pswd.c_str() << "\r" << endl;
	cout << "WiFi Net-3 [Name]: " << settings.wifi_nets_login[2].Name.c_str() << "\r" << endl;
	cout << "WiFi Net-3 [Psw]: " << settings.wifi_nets_login[2].Pswd.c_str() << "\r" << endl;
	cout << "Time Offset:" << settings.time_offset << "\r" << endl;
	cout << "LCD Orientation Normal:" << settings.lcd_orientation_normal << "\r" << endl;
	cout << "Connecting to WiFi..." << "\r" << endl;
}

//Read the config file without updateding the settings
String myConfigFile :: config_file() {
	String json;
	File f = SPIFFS.open("/config.json", "r");
	if (f) {
		return  f.readString();
	}
	else {
		cout << "Error: Cannot read Config file!" << "\r" << endl;
		return "";
	}
}

//Read the Config file
bool myConfigFile::read() {
	String json;
	File f = SPIFFS.open("/config.json", "r");
	if (f) {
		json=f.readString();
		f.close();
		setProps(json);
		return true;
	}
	else {
		cout << "Error: Cannot read Config file!" << "\r" << endl;
		return false;
	}
}

//Save the Config settings in the file
//True if all chars of the JSON string can be written in the file, false otherwise

bool myConfigFile::write() {
	bool result;
	String json = getProps();

	File f = SPIFFS.open("/config.json", "w");
	if (f) {
		f.write(json.c_str());
		f.close();
		return true;
	}
	else {
		cout << "Error: Cannot write Config file!" << "\r" << endl;
		return false; // cannot open the config file
	}
}

// Private function to parse the JSON string from the Config file into the config structure

 void myConfigFile::setProps(String json) {
	
	DynamicJsonDocument doc(1536);
	DeserializationError error = deserializeJson(doc, json);
	if (error) {
		cout << "Reading Config JSON failed: " << error.f_str() << "\r" << endl;
		return;
	}
	
	int k=0;
	for (JsonObject elem : doc["wifi_nets_login"].as<JsonArray>()) {
		const char* name = elem["name"];
		const char* pswd = elem["pswd"];
		wifi_nets_login[k].Name = name;
		wifi_nets_login[k].Pswd = pswd;

		//cout << "Name:"<< name <<"Pswd: "<< pswd << endl;
		k++;
	}
	time_offset = doc["time_offset"];
	lcd_orientation_normal = doc["lcd_orientation_normal"];
}

 //Private function to parse the config into a JSON string
 String myConfigFile::getProps() {
	 
	 String json;
	 DynamicJsonDocument doc(1536);
	 JsonObject elem;

	 JsonArray wifi_ap = doc.createNestedArray("wifi_nets_login");
	 for (int k = 0; k < 3; k++) {
		 elem = wifi_ap.createNestedObject();
		 elem["name"] = wifi_nets_login[k].Name;
		 elem["pswd"] = wifi_nets_login[k].Pswd;
	 }
	 doc["time_offset"] = time_offset;
	 doc["lcd_orientation_normal"] = lcd_orientation_normal;
	 serializeJson(doc, json);
	 return json;
 }

 


