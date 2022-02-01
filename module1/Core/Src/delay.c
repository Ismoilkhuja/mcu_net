#include "delay.h"

volatile uint32_t ticks_delay = 0;

void SysTick_Handler(){
    ticks_delay++;
}

void delay_ms(const uint32_t milliseconds){
    uint32_t start = ticks_delay;
    while ((ticks_delay-start)<milliseconds) {}
}
