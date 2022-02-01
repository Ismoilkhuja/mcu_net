#include "usart.h"

void USART1_Init(){
    // тактирование USART1
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

    // скорость работы USART1 = 115200 бод, при частоте 16 МГц
    USART1->BRR = 0x8B;

    USART1->CR1 |=  USART_CR1_UE |
            USART_CR1_TE |
            USART_CR1_RE;

    NVIC_EnableIRQ(USART1_IRQn);
}

void USART1_Transmit(const uint8_t data){
    while (!(USART1->SR & USART_SR_TXE));
    USART1->DR = data;
}

void USART1_Transmit_Str(const char* string){
    while (*string) USART1_Transmit(*string++);
}

uint8_t USART1_Receive(void){
    while (!(USART1->SR & USART_SR_RXNE));
    return USART1->DR;
}

void USART1_Transmit_Buffer(const uint8_t* buffer, uint32_t size){
    for (uint32_t i =0; i<size; i++)
        USART1_Transmit(buffer[i]);
}
