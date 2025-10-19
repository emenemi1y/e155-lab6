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

void initSPI(int br, int cpol, int cpha, int res){

  // Enable system clock for SPI3
  RCC->APB1ENR1 |= RCC_APB1ENR1_SPI3EN;

  // Configure GPIO pins for SPI3
  // Set to alternate function mode
  pinMode(SPI_SCK, GPIO_ALT);     // SPI3_SCK
  pinMode(SPI_MISO, GPIO_ALT);    // SPI3_MISO
  pinMode(SPI_MOSI, GPIO_ALT);    // SPI3_MOSI
  pinMode(SPI_CE, GPIO_OUTPUT);   // Manual CS

  // Set output speed type to high for SCK
  GPIOB->OSPEEDR |= (GPIO_OSPEEDR_OSPEED3);

  // Set to AF06 for SPI alternate functions 
  GPIOB->AFR[0] |= _VAL2FLD(GPIO_AFRL_AFSEL3, 6);
  GPIOB->AFR[0] |= _VAL2FLD(GPIO_AFRL_AFSEL4, 6);
  GPIOB->AFR[0] |= _VAL2FLD(GPIO_AFRL_AFSEL5, 6);

  // Set the baud rate 
  SPI3->CR1 |= _VAL2FLD(SPI_CR1_BR, br);

  // Set to controller configuration
  SPI3->CR1 |= SPI_CR1_MSTR;

  SPI3->CR1 |= _VAL2FLD(SPI_CR1_CPOL, cpol); // Polarity
  SPI3->CR1 |= _VAL2FLD(SPI_CR1_CPHA, cpha); // Phase
  SPI3->CR2 |= _VAL2FLD(SPI_CR2_DS, 0b0111); // Data length for transfer

  // Enable SPI
  SPI3->CR1 |= (SPI_CR1_SPE);

}


/* Transmits a character (1 byte) over SPI and returns the received character.
 *    -- send: the character to send over SPI
 *    -- return: the character received over SPI */

char spiSendReceive(char send){
  while(!(SPI3->SR & SPI_SR_TXE)); // Wait until the transmit buffer is empty
  *(volatile char *) (&SPI3->DR) = send;  // Trasnsmit the character over SPI
  while(!(SPI3->SR & SPI_SR_RXNE)); // Wait until data has been received
  char rec = (volatile char) SPI3->DR;
  return rec; // Return received character
}