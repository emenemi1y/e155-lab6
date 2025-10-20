// DS1722.c
// Emily Kendrick
// ekendrick@hmc.edu
// 10/19/2025
// Function definitions for the DS1722 temperature sensor

#include "DS1722.h"
#include "STM32L432KC_GPIO.h"
#include "STM32L432KC_SPI.h"
#include "STM32L432KC_TIM.h"
#include <math.h>

// Setup temperature sensor with 9-bit resolution
void tempSetup(void){
  // Set CE high 
  digitalWrite(SPI_CE, PIO_HIGH);

  // Write "write address" to temperature sensor
  spiSendReceive(WRITE_ADR);

  // Send configuration register settings 
  spiSendReceive(0b11100000);

  // Disable CE
  digitalWrite(SPI_CE, PIO_LOW);
}

void setResolution(int res) {
  // Set CE high
  digitalWrite(SPI_CE, PIO_HIGH);

  // Write the write address to the temperature sensor
  spiSendReceive(WRITE_ADR);

  // Send configuration register settings
  int res_bits = (int) (res - 8);
  spiSendReceive(0b11100000 | (res_bits << 1));

  // Disable CE 
  digitalWrite(SPI_CE, PIO_LOW);
}


int tempRead(void){
  
  // Read MSB: 
  // Set CE high
  digitalWrite(SPI_CE, PIO_HIGH);

  // Write the MSB address to the temperature sensor and ignore returned value
  spiSendReceive(TEMP_MSB);

  // Read returned temperature result
  int MSB = spiSendReceive(0x00);

  // Disable CE 
  digitalWrite(SPI_CE, PIO_LOW);
  
  delay_millis(TIM15, 1);

  // Read LSB:
  // Set CE high
  digitalWrite(SPI_CE, PIO_HIGH);

  // Write the LSB address to the temperature sensor and ignore returned value
  spiSendReceive(TEMP_LSB);

  // Read returned temperature result
  int LSB = spiSendReceive(0x00);

  // Disable CE
  digitalWrite(SPI_CE, PIO_LOW);  
  
  return (LSB | (MSB << 8));

}

float convertTemp(int bits) {
  float temp = 0;
  // Iterate through each bit from the temperature sensor output 
  for (int i = 0; i < 16; i++) {
    int bit = ((bits >> i) & 1);
    if (i == 16 && bit) temp -= pow(2, -7);
    else if (bit) temp += pow(2, (i - 9));
  }
  
  return temp;
}