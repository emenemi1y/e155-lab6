// website.h
// Emily Kendrick
// ekendrick@hmc.edu
// 11/5/2025
// Header file for the website


// Defining the web page in two chunks: everything before the current time, and everything after the current time

char* webpageStart = "<!DOCTYPE html><html><head><title>E155 Lab 6</title>\
	<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\
	</head>\
	<body><h1>E155 Lab 6</h1>";
char* ledStr = "<p>LED Control:</p><form action=\"ledon\"><input type=\"submit\" value=\"Turn the LED on!\"></form>\
	<form action=\"ledoff\"><input type=\"submit\" value=\"Turn the LED off!\"></form>";
char* tempStr = "<p>Temperature Measurement:</p>";
char* tempRes = "<p>Temperature Resolution:</p><form action=\"8bit\"><input type=\"submit\" value=\"1.0&deg;C\"></form>\
        </p><form action=\"9bit\"><input type=\"submit\" value=\"0.5&deg;C\"></form>\
        </p><form action=\"10bit\"><input type=\"submit\" value=\"0.25&deg;C\"></form>\
        </p><form action=\"11bit\"><input type=\"submit\" value=\"0.125&deg;C\"></form>\
        </p><form action=\"12bit\"><input type=\"submit\" value=\"0.0625&deg;C\"></form>";
char* webpageEnd   = "</body></html>";

#define LED_PIN PB6 // LED pin for blinking on Port B pin 3

int inString(char request[], char des[]);
int updateLEDStatus(char request[]);
int updateResolution(char request[]);

