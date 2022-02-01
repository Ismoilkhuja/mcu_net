#pragma once

#include "util/twi.h"
#include "avr/io.h"
#include "avr/interrupt.h"

void TWI_Init();

uint8_t TWI_Receive(uint8_t slave_address, uint8_t* data, uint32_t size);

uint8_t TWI_Transmit(uint8_t slave_address, const uint8_t* data, uint32_t size);

uint8_t TWI_Busy();
