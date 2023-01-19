#include "myLCD.h"

myLCD LCD;

//Create the display object
SSD1306Wire display(0x3c, SDA, SCL);

/// <summary>
/// Initializations of the LCD library
/// </summary>
void myLCD::Init()
{
    display.init();
    if (settings.lcd_orientation_normal) {
        display.flipScreenVertically();
    }
    display.setFont(ArialMT_Plain_10);
    font = ArialMT_Plain_10;
}

//Flips the screen
void myLCD::flipScreen() {
    settings.lcd_orientation_normal = !settings.lcd_orientation_normal;
    if (settings.lcd_orientation_normal) {
        display.flipScreenVertically();
    }
    else {
        display.resetOrientation();
    }
    display.clear();
    display.display();
}

/// <summary>
/// Change the current font size
/// </summary>
/// <param name="f"></param>
void myLCD::fontSize(font_sizes f) {
    switch (f) {
        case SMALL:           
            font = Arial_Small;
            break;
        case MEDIUM:
            font = Arial_Medium;
            break;
        case LARGE:
            font = Arial_Large;
            break;
    }
    display.setFont(font);
}

/// <summary>
/// Update LCD with data
/// </summary>
void myLCD::showData() {
    
    // draw title
    display.clear();
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    fontSize(MEDIUM);
    display.drawString(0, 0, "Time Server");
        
    //draw horizontal line
    display.drawHorizontalLine(0, 22, 128);
    
    //Time
    fontSize(LARGE);
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.drawString(64, 28, buffer[0]);
    
    //Date
    fontSize(SMALL);
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.drawString(64, 54, buffer[1]);

    //RSSI indication
    if (RSSI < -80) {
        showRSSI(NONE);
    }
    else if (RSSI >= -80 && RSSI < -70) {
        showRSSI(WEAK);
    }
    else if (RSSI >= -70 && RSSI < -60) {
        showRSSI(FAIR);
    }
    else {
        showRSSI(GOOD);
    }
    display.display();
    delay(holdTime);
}

/// <summary>
/// Display RSSI guage with 3-bars
/// </summary>
/// <param name="k"></param>
void myLCD::showRSSI(RSSI_vals k) {
    switch (k) {
    case WEAK:
        display.fillRect(107, 15, 5, 5);
        display.fillRect(115, 18, 5, 0);
        display.fillRect(123, 18, 5, 0);
        break;
    case FAIR:
        display.fillRect(107, 15, 5, 5);
        display.fillRect(115, 10, 5, 10);
        display.fillRect(123, 18, 5, 0);
        break;
    case GOOD:
        display.fillRect(107, 15, 5, 5);
        display.fillRect(115, 10, 5, 10);
        display.fillRect(123, 5, 5, 15);
        break;
    default:
        display.fillRect(107, 18, 5, 0);
        display.fillRect(115, 18, 5, 0);
        display.fillRect(123, 18, 5, 0);
    }
}

/// <summary>
/// Shows running messages on the LCD with title or without
/// </summary>
void myLCD::showText(String text) {
    int k = 0;
    if (title == "") {
        display.setTextAlignment(TEXT_ALIGN_LEFT);
        fontSize(SMALL);
        display.clear();
        //Move up one position when more than 5 lines
        if (index > 4) {
            for (k = 0; k < 4; k++) {
                buffer[k] = buffer[k + 1];
            }
            buffer[4] = text;
        }
        else {
            buffer[index] = text;
            index++;
        }
        for (k = 0; k < 5; k++) {
            display.drawString(0, (k * 12), buffer[k]);
        }
    }else{
        //draw title
        display.clear();
        display.setTextAlignment(TEXT_ALIGN_LEFT);
        fontSize(MEDIUM);
        display.drawString(0, 0, title);
        //draw horizontal line
        display.drawHorizontalLine(0, 20, 128);
        //push messages under the line
        fontSize(SMALL);
        //Move up one position when more than 3 lines
        if (index > 2) {
            for (k = 0; k < 2; k++) {
                buffer[k] = buffer[k + 1];
            }
            buffer[2] = text;
        }
        else {
            buffer[index] = text;
            index++;
        }
        for (k = 0; k < 3; k++) {
            display.drawString(0, 23 + (k * 12), buffer[k]);
        }
    }
    display.display();
    delay(holdTime);
}

/// <summary>
/// loads the internal message buffer with data
/// </summary>
/// <param name="text"></param>
void myLCD::loadBuffer(String text[MAX_BUFF]) {
    int k = 0;
    for (k = 0; k < MAX_BUFF; k++) {
        buffer[k] = text[k];
    }
}

/// <summary>
/// Sets the index of the LCD queue to zero
/// </summary>
void myLCD::clearLCD() {
    int k;
    for (k = 0; k < MAX_BUFF; k++) {
        buffer[k] = "";
    }
    display.clear();
    index = 0;
    title = "";
}

/// <summary>
/// Set title of a view with Title
/// </summary>
/// <param name="text"></param>
void myLCD::setTitle(String text) {
    title=text;
}

/// <summary>
/// Sets the current value of the signal strength
/// </summary>
/// <param name="RSSI"> enum with RSSI values</param>
void myLCD::setRSSI(int32_t RSSI_value) {
    RSSI = RSSI_value;
}  

/// <summary>
/// Sets the time to hold display before 
/// refreshing with new data 
/// </summary>
/// <param name="sec"></param>
void myLCD::setHoldTime(int sec) {
    holdTime = sec;
}


/// <summary>
/// Shows Name and Revsion of the App
/// </summary>
void myLCD::appRev() {
    display.clear();
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    fontSize(LARGE);
    display.drawString(64, 12, APP);
    fontSize(SMALL);
    display.drawString(64, 42, "Version "+ (String)REV);
    display.display();
    delay(3000);
}


