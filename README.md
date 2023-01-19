# SC_ATOMIC_TIME
This code demonstrates the functionality of the SVR-A Senzotic module
----------------------------------------------------------------------------------------------------------
Atomic Time Application description:
----------------------------------------------------------------------------------------------------------
During start up the program shows initialization messages on the display.
The Wi-Fi radio of the ESP8266 is initialized and then an attempt is made to connect to one of three local networks.
If the connection is successful the device calls one of four time servers to obtain accurate time information.
The time and date is then displayed on the LCD and sent to the UART port every 500 msec, so that the green LED is periodically flashing.
The User may configure the connection to the Wi-Fi network with a terminal command called "connect” sent over the UART port. After selecting the network name, the application requests the password and resets. The Time Servers will be available only when the device is connected to a Wi-Fi network with connection to the Internet.
The UART settings are:        9600, 8 bits, No Parity, One stop bit, No flow control
This application code is provided as is without any warrantee. Its sole purpose is to help in the development of applications using the SVR-A module.
Happy coding!
