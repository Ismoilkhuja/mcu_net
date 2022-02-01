#include "usart.h"

void USART_Init(){
    // Включаю тактирование USART1
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
    // Включаю тактирование GPIOA, если не включено
    if (~(RCC->AHB1ENR & RCC_AHB1ENR_GPIOAEN))
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    // Альтернативные фукции пинов PA9 - TX, PA10 - RX при AF07 = 0111
    GPIOA->AFR[1] |= 0x7 << GPIO_AFRH_AFSEL9_Pos | 0x7 << GPIO_AFRH_AFSEL10_Pos;

    GPIOA->MODER |= 0b1010 << GPIO_MODER_MODE9_Pos;

    // скорость USART - 115200 бод при частоте 16МГц
    USART1->BRR = 0x8B;

    USART1->CR1 |=  USART_CR1_TE |
                    USART_CR1_RE |
                    USART_CR1_RXNEIE |
                    USART_CR1_UE;

    NVIC_EnableIRQ(USART1_IRQn);
}

void USART_Send(const uint8_t data){
    while (!(USART1->SR & USART_SR_TXE));
    USART1->DR = data;
}


void USART_SendStr(const char* string){
    while (*string) USART_Send(*string++);
}

void USART_Send_Buffer(const uint8_t* buffer, const uint32_t buffer_size){
    for (uint32_t i=0; i<buffer_size; i++){
        USART_Send(buffer[i]);
    }
}

uint8_t USART_Receive(void){
    while (!(USART1->SR & USART_SR_RXNE));
    return USART1->DR;
}
