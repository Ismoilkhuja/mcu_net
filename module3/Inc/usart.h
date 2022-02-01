#pragma once

#include "avr/io.h"

void USART0_Init();

void USART0_Transmit(uint8_t data);

void USART0_Transmit_Buffer(const char* buffer);

uint8_t USART_Receive();

void USART_Receive_Buffer(uint8_t* buffer, uint32_t size);
