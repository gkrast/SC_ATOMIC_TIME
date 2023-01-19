// myTerminal.h

#ifndef _MYTERMINAL_h
#define _MYTERMINAL_h

#include "app.h"

// Global Error codes
enum inputMode {
	COMMAND,
	GMT,
	SELECT_NET,
	SET_PASSWORD,
	HELP
	// add more here
};

class MyTerminalClass
{
 
 public:
	inputMode mode;
	void init();	
	void exe(String com); // main command executive
	// Commands
	void help();			// Shows a list of commands
	void reset();		    // Software reset
	void config();			// Reads the config file without updating the settings
	void flip();			// Flip the vertical orientation of the LCD
	void nets();			// List the top three wi-fi networks in range
	void gmt();				// Set the local time zone
};

extern MyTerminalClass myTerminal;

#endif

