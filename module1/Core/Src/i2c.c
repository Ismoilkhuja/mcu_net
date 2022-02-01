#include "i2c.h"

void I2C1_Init()
{
    // Тактирование I2C1
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
    // Частота APB1 идущий на I2C1
    I2C1->CR2 &= ~I2C_CR2_FREQ;
    I2C1->CR2 |= 16;
    // Прерывание по буферам TxE, RxNE и флагам
    I2C1->CR2 |= I2C_CR2_ITBUFEN | I2C_CR2_ITEVTEN;
    // Адрес I2C1 в режиме Slave
    I2C1->OAR1 = 0xD0;
    // Настройка частоты I2C1 на Standart mode 100кГц
    I2C1->CCR = 80;
    // Максимальное время нарастания фронта I2C на тактах APB шины
    I2C1->TRISE = 17;
    // Включить I2C1
    I2C1->CR1 |= I2C_CR1_PE | I2C_CR1_ACK;

    NVIC_EnableIRQ(I2C1_EV_IRQn);
}

void I2C1_Send_Byte(uint8_t data){

    I2C1->CR1 = I2C_CR1_PE | I2C_CR1_ACK;

    while (!(I2C1->SR1 & I2C_SR1_ADDR));
    (void) I2C1->SR1;
    (void) I2C1->SR2;

    while (!(I2C1->SR1 & I2C_SR1_TXE));
    I2C1->DR = data;

    while (!(I2C1->SR1 & I2C_SR1_AF));
    I2C1->SR1 &= ~I2C_SR1_AF;
    I2C1->CR1 = I2C_CR1_PE;
}
