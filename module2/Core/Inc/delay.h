#ifndef DELAY_H
#define DELAY_H

#include "main.h"

void SysTick_Handler();

void delay_ms(const uint32_t milliseconds);

#endif // DELAY_H
