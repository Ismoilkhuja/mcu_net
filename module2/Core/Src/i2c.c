#include "i2c.h"




void I2C1_Init()
{
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;

    NVIC_EnableIRQ(I2C1_EV_IRQn);
    // Freq 16MHz
    //I2C1->CR2 = 16;
    // Interrupt
    I2C1->CR2 |= I2C_CR2_ITBUFEN | I2C_CR2_ITEVTEN;

    //I2C1->CCR = 80;

    //I2C1->TRISE = 17;

    // Address
    I2C1->OAR1 = 0xA0;

    I2C1->CR1 |= I2C_CR1_PE | I2C_CR1_ACK;
}
