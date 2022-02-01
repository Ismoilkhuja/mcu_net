#pragma once

#include "stm32f4xx.h"

void USART_Init();

void USART_Send(const uint8_t data);

void USART_SendStr(const char* string);

void USART_Send_Buffer(const uint8_t* buffer, const uint32_t buffer_size);

uint8_t USART_Receive(void);
