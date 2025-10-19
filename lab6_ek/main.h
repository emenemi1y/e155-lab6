/**
    Main Header: Contains general defines and selected portions of CMSIS files
    @file main.h
    @author Josh Brake
    @version 1.0 10/7/2020
*/

#ifndef MAIN_H
#define MAIN_H

#include "STM32L432KC.h"

#define LED_PIN PB3 // LED pin for blinking on Port B pin 3
#define BUFF_LEN 32
#define CPHA 1 // CPHA must be set to 1
#define DELAY_TIM TIM15

#endif // MAIN_H