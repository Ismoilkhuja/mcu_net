#pragma once

#include "stm32f4xx.h"

void USART2_Init();

void USART2_Send(const uint8_t data);

void USART2_SendStr(const char* string);

