#include "spi.h"

void SPI1_Init()
{
    // Тактирование SPI1
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

    //PA5	--	SPI1_SCK
    //PA6	--	SPI1_MISO
    //PA7	--	SPI1_MOSI

    // Альтернативная функция пинов, AF05 - 0b0101
    GPIOA->AFR[0] |= 0x555 << GPIO_AFRL_AFSEL5_Pos;

    // Функционал пинов - альтернативная, 0b10
    GPIOA->MODER |= 0b101010 << GPIO_MODER_MODER5_Pos;

    SPI1->CR1 |=   SPI_CR1_MSTR |
            SPI_CR1_SSI |
            SPI_CR1_SSM |
            SPI_CR1_SPE |
            SPI_CR1_CPOL |
            SPI_CR1_CPHA |
            SPI_CR1_BR_0;

    NVIC_EnableIRQ(SPI1_IRQn);
}

void SPI1_Send(const uint8_t data){
    while (!(SPI1->SR & SPI_SR_TXE));
    SPI1->DR = data;

    while (!(SPI1->SR & SPI_SR_RXNE));
    READ_REG(SPI1->DR);
}

void SPI1_SendStr(const char* string){
    while (*string) SPI1_Send(*string++);
}

uint8_t SPI1_Receive(void){
    SPI1->DR = 0x00;

    while (!(SPI1->SR & SPI_SR_RXNE));
    return SPI1->DR;
}

uint8_t SPI_Transmit_Receive(uint8_t data){
    while (!(SPI1->SR & SPI_SR_TXE));
    SPI1->DR = data;

    while (!(SPI1->SR & SPI_SR_RXNE));
    data = SPI1->DR;

    return data;
}

void SPI_Transmit_Data(const uint8_t* data, uint32_t size){
    // Согласование начало передачи с приемником
    // Передается 0x23, ожидается 0x45
    while (SPI_Transmit_Receive(0x23) != 0x45);

    for (uint32_t i=0; i<size; i++)
        SPI_Transmit_Receive(data[i]);
}

void SPI_Receive_Data(uint8_t* data, uint32_t size){
    // Согласование начало передачи с приемником
    // Передается 0x23, ожидается 0x45
    while (SPI_Transmit_Receive(0x23) != 0x45);

    for (uint32_t i=0; i<size; i++)
        data[i] = SPI_Transmit_Receive(0x00);
}
