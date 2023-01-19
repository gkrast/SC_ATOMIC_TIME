/* myLCD.h 
*/

#ifndef _MYLCD_h
#define _MYLCD_h

#include "app.h"


/// <summary>
/// Class definitions
/// </summary>
class myLCD
{
 private:
	 const uint8_t* font;  //current LCD font
	 String buffer[MAX_BUFF];		//LCD message queue
	 int index = 0;		//index of the current message in the push buffer

	 int RSSI = 0;
	
	 long holdTime = 0;		//time interval of LCD refreshing in msec
	 unsigned long refreshTimer = millis();   //refresh timer in msec
	 String title;	   //Title for LCD mode with Title
	 
	 //Private Functions
     void fontSize(font_sizes f);
	 void showRSSI(RSSI_vals k);

 public:
	bool no_altitude = true;

	void Init();		
	void appRev();
	//Push control functions
	void loadBuffer(String text[MAX_BUFF]); // Loads internal buffer
	void clearLCD();				//Clears the LCD, dabuffer buffer and title
    void showText(String text);			//Pushes string to the screen
	void showData();					//Updates LCD with sensor data loaded in the buffer
	void setTitle(String text);		//Sets the titel of LCD mode with Title
	void setHoldTime(int sec);		//Sets the hold time between refreshes
	void setRSSI(int32_t RSSI_value);
	void flipScreen();	//flips the screen
};

extern myLCD LCD;

#endif
