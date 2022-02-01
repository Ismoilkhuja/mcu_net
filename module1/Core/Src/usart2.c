#include "usart2.h"

void USART2_Init(){
    // Включаю тактирование USART2
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
    // Включаю тактирование GPIOA, если не включено
    if (~(RCC->AHB1ENR & RCC_AHB1ENR_GPIOAEN))
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    // Альтернативные фукции пинов PA2 - TX, PA3 - RX при AF07 = 0111
    GPIOA->AFR[0] |= 0x7 << GPIO_AFRL_AFSEL2_Pos | 0x7 << GPIO_AFRL_AFSEL3_Pos;

    GPIOA->MODER |= 0b1010 << GPIO_MODER_MODE2_Pos;

    // скорость USART - 115200 бод при частоте 16МГц
    USART2->BRR = 0x8B;

    USART2->CR1 |=  USART_CR1_TE |
                    USART_CR1_RE |
//                    USART_CR1_RXNEIE |
                    USART_CR1_UE;

//    NVIC_EnableIRQ(USART2_IRQn);
}

void USART2_Send(const uint8_t data){
    while (!(USART2->SR & USART_SR_TXE));
    USART2->DR = data;
}


void USART2_SendStr(const char* string){
    while (*string) USART2_Send(*string++);
}

