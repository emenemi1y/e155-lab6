// STM32L432KC_SPI.c
// TODO: <YOUR NAME>
// TODO: <YOUR EMAIL>
// TODO: <DATE>
// TODO: <SHORT DESCRIPTION OF WHAT THIS FILE DOES>

#include "STM32L432KC_SPI.h"
#include "STM32L432KC_RCC.h"
#include "STM32L432KC_GPIO.h"

/* Enables the SPI peripheral and intializes its clock speed (baud rate), polarity, and phase.
 *    -- br: (0b000 - 0b111). The SPI clk will be the master clock / 2^(BR+1).
 *    -- cpol: clock polarity (0: inactive state is logical 0, 1: inactive state is logical 1).
 *    -- cpha: clock phase (0: data captured on leading edge of clk and changed on next edge, 
 *          1: data changed on leading edge of clk and captured on next edge)
 * Refer to the datasheet for more low-level details. */ 

void initSPI(int br, int cpol, int cpha){

  // Enable system clock for SPI3
  RCC->APB1ENR1 |= RCC_APB1ENR1_SPI3EN;

  // Configure GPIO pins for SPI3
  // Set to alternate function mode
  pinMode(PB3, GPIO_ALT);
  pinMode(PB4, GPIO_ALT);
  pinMode(PB5, GPIO_ALT);

  // Set to AF6 (SPI3)
  gpioAFSel(PB3, 6);
  gpioAFSel(PB4, 6);
  gpioAFSel(PB5, 6);

  // Set the baud rate 
  SPI3->CR1 |= (br << SPI_CR1_BR_Pos);

  // Set clock phase and polarity
  SPI3->CR1 |= (cpol << SPI_CR1_CPOL_Pos);
  SPI3->CR1 |= (cpha << SPI_CR1_CPHA_Pos);

  // Set data length for transfer to 9 bits 
  SPI3->CR2 |= (0b1000 << SPI_CR2_DS_Pos);

  // Set to controller configuration
  SPI3->CR1 |= SPI_CR1_MSTR;

}

/* Transmits a character (1 byte) over SPI and returns the received character.
 *    -- send: the character to send over SPI
 *    -- return: the character received over SPI */

char spiSendReceive(char send){


}