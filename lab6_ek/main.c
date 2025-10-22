 /*
File: lab6_ek.c
Author: Emily Kendrick
Email: ekendrick@hmc.edu
Date: 10/22/25
Adapted from Prof Brake's Lab 6 starter code
*/


#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "main.h"

// For printf
int _write(int file, char *ptr, int len) {
  int i = 0;
  for (i = 0; i < len; i++){
    ITM_SendChar((*ptr++));
  }
  return len;
}

/////////////////////////////////////////////////////////////////
// Provided Constants and Functions
/////////////////////////////////////////////////////////////////

//Defining the web page in two chunks: everything before the current time, and everything after the current time
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

//determines whether a given character sequence is in a char array request, returning 1 if present, -1 if not present
int inString(char request[], char des[]) {
	if (strstr(request, des) != NULL) {return 1;}
	return -1;
}

// Updates the LED power and status variable
int updateLEDStatus(char request[])
{
        int led_status = 2;
	// The request has been received. now process to determine whether to turn the LED on or off
	if (inString(request, "ledoff")==1) {
		digitalWrite(LED_PIN, PIO_LOW);
		led_status = 0;
	}
	else if (inString(request, "ledon")==1) {
		digitalWrite(LED_PIN, PIO_HIGH);
		led_status = 1;
	}

	return led_status;
}

// Updates the resolution of the temperature sensor.
int updateResolution(char request[])
{
        int res_status = 0;
        // The request has been received. Now process to determine what to change the resolution to.
        if (inString(request, "8bit")==1){
          setResolution(8);
          res_status = 8;
        }
        else if (inString(request, "9bit")==1){
          setResolution(9);
          res_status = 9;
        }
        else if (inString(request, "10bit")==1){
          setResolution(10);
          res_status = 10;
        }
        else if (inString(request, "11bit")==1){
          setResolution(11);
          res_status = 11;
        }
        else if (inString(request, "12bit")==1){
          setResolution(12);
          res_status = 12;
        }
        return res_status;

}

/////////////////////////////////////////////////////////////////
// Solution Functions
/////////////////////////////////////////////////////////////////

int main(void) {
  configureFlash();
  configureClock();
  
  // Enable GPIO
  gpioEnable(GPIO_PORT_A);
  gpioEnable(GPIO_PORT_B);
  gpioEnable(GPIO_PORT_C);
  
  // Enable Timer 15
  RCC->APB2ENR |= RCC_APB2ENR_TIM15EN;
  initTIM(TIM15);
  
  // Set up LED pin
  pinMode(LED_PIN, GPIO_OUTPUT);
  digitalWrite(LED_PIN, 0);
  
  USART_TypeDef * USART = initUSART(USART1_ID, 125000);

  // SPI initialization code 
  initSPI(0b100, 0, CPHA); 
  tempSetup();
  
  // Initial resolution and LED statuses upon boot-up
  int res_status = 9;
  int led_status_init = 0;

  while(1) {
    /* Wait for ESP8266 to send a request.
    Requests take the form of '/REQ:<tag>\n', with TAG begin <= 10 characters.
    Therefore the request[] array must be able to contain 18 characters.
    */
    
    // Receive web request from the ESP
    char request[BUFF_LEN] = "                  "; // initialize to known value
    int charIndex = 0;
    
    // Keep going until you get end of line character
    while(inString(request, "\n") == -1) {
      // Wait for a complete request to be transmitted before processing
      while(!(USART->ISR & USART_ISR_RXNE));
      request[charIndex++] = readChar(USART);
    } 
    
    // Update LED status 
    int led_status = updateLEDStatus(request);
    
    if(led_status == 2)
      led_status = led_status_init;
    else
      led_status_init = led_status;
    
    char ledStatusStr[20];
    if (led_status == 1)
      sprintf(ledStatusStr,"LED is on!");
    else if (led_status == 0)
      sprintf(ledStatusStr,"LED is off!");
    
    
    // Update resolution status 
    int new_res_status = updateResolution(request);
    if (new_res_status == 0) 
        new_res_status = res_status;
    else 
        res_status = new_res_status;

    volatile int sensor_reading = tempRead();
    float temp = convertTemp(sensor_reading);
    
    // Resolution status string
    char resStatusStr[28];
    char tempString[20];
    if (res_status == 8) {
        sprintf(resStatusStr, "Resolution: 1.0 deg C");
        sprintf(tempString, "Temp: %d deg C", (int) temp);
    }
    else if (res_status == 9) {   
        sprintf(resStatusStr, "Resolution: 0.5 deg C");
        sprintf(tempString, "Temp: %.1f deg C", temp);
    }
    else if (res_status == 10) {
        sprintf(resStatusStr, "Resolution: 0.25 deg C");
        sprintf(tempString, "Temp: %.2f deg C", temp);
    }
    else if (res_status == 11) {
        sprintf(resStatusStr, "Resolution: 0.125 deg C");
        sprintf(tempString, "Temp: %.3f deg C", temp);
    }
    else if (res_status == 12) {
        sprintf(resStatusStr, "Resolution: 0.0625 deg C");
        sprintf(tempString, "Temp: %.4f deg C", temp);
    }
    
    
    // finally, transmit the webpage over UART
    sendString(USART, webpageStart); // webpage header code
    sendString(USART, ledStr); // button for controlling LED
    sendString(USART, tempRes); // button for controlling resolution

    sendString(USART, "<h2>LED Status</h2>");

    sendString(USART, "<p>");
    sendString(USART, ledStatusStr);
    sendString(USART, "</p>");
    
    sendString(USART, "<h2>Temperature Resolution</h2>");
    sendString(USART, "<p>");
    sendString(USART, resStatusStr);
    sendString(USART, "</p>");
  
    sendString(USART, "<h2>Temperature Measurement</h2>");
    sendString(USART, "<p>");
    sendString(USART, tempString);
    sendString(USART, "</p>");

    sendString(USART, webpageEnd);
    
  }
}
