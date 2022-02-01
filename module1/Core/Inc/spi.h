#pragma once

#include "stm32f4xx.h"

void SPI1_Init();

void SPI1_Send(const uint8_t data);

void SPI1_SendStr(const char* string);

uint8_t SPI1_Receive(void);

uint8_t SPI_Transmit_Receive(uint8_t data);

void SPI_Transmit_Data(const uint8_t* data, uint32_t size);

void SPI_Receive_Data(uint8_t* data, uint32_t size);
