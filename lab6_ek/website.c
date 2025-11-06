// website.c
// Source code for website functions

#include "STM32L432KC_RCC.h"
#include "STM32L432KC_GPIO.h"
#include "DS1722.h"
#include "website.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

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