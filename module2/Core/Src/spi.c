#include "spi.h"

void SPI1_Init(){
    // Тактирование SPI1
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

    NVIC_EnableIRQ(SPI1_IRQn);

    SPI1->CR1 |= SPI_CR1_SSM |
            SPI_CR1_SPE |
            SPI_CR1_CPOL |
            SPI_CR1_CPHA |
            SPI_CR1_BR_0;
}

void SPI1_Send(const uint8_t data){
    while (!(SPI1->SR & SPI_SR_TXE));
    SPI1->DR = data;
}

void SPI1_SendStr(const char* string){
    while (*string) SPI1_Send(*string++);
}

uint8_t SPI1_Receive(void){
    //SPI1->DR = 0;   // запускаю обмен

    while (!(SPI1->SR & SPI_SR_RXNE));

    return SPI1->DR;
}

void SPI_Receive_Data(uint8_t* data, uint32_t size){
    // Очищу буфер приемника
    while (!(SPI1->SR & SPI_SR_RXNE));
    READ_REG(SPI1->DR);
    // Данные находящиеся в регистре сдвига попадет в буфер приемника
    // и будет очищена
    while (!(SPI1->SR & SPI_SR_RXNE));
    READ_REG(SPI1->DR);

    // Согласование начало приема с ведущим
    // Отправим 0x45, ожидаем 0x23
    while (SPI_Transmit_Receive(0x45)!= 0x23);

    for (uint32_t i=0; i<size; i++){
        data[i] = SPI_Transmit_Receive(0x00);
    }
}

void SPI_Transmit_Data(const uint8_t* data, uint32_t size){
    // Согласование начало приема с ведущим
    // Отправим 0x45, ожидаем 0x23
    while (SPI_Transmit_Receive(0x45)!= 0x23);

    for (uint32_t i=0; i<size; i++){
        SPI_Transmit_Receive(data[i]);
    }
}

uint8_t SPI_Transmit_Receive(uint8_t data){
    while (!(SPI1->SR & SPI_SR_TXE));
    SPI1->DR = data;

    while (!(SPI1->SR & SPI_SR_RXNE));
    data = SPI1->DR;

    return data;
}
