#pragma once

#include "stm32f1xx.h"

void USART1_Init();

void USART1_Transmit(const uint8_t data);

void USART1_Transmit_Str(const char* string);

void USART1_Transmit_Buffer(const uint8_t* buffer, uint32_t size);

uint8_t USART1_Receive(void);
