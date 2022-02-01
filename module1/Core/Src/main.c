#include "main.h"
#include <stdio.h>
#include <string.h>
#include "usart.h"
#include "spi.h"
#include "i2c.h"
#include "delay.h"
#include "crc8.h"

#define message_length 13

enum {
    none,
    USART1_Received,
    SPI1_Transmitted,
    I2C1_Received
} state;

uint8_t buffer[13]={0},
second_buffer[13]={0},
index=0,
crc=0;

void GPIO_Init();

int main()
{
    uint8_t ack_byte=0xff;

    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;

    SysTick_Config(SystemCoreClock/1000);
    GPIO_Init();
    USART_Init();
    SPI1_Init();
    I2C1_Init();

    __enable_irq();

    state = none;
    while (1) {

        switch (state) {
        case USART1_Received:
            index =0;
            state = none;
            if (!(crc_8(buffer, sizeof (buffer)))){
                // CRC совпадает
                USART_Send(0x00);
                do {
                    SPI_Transmit_Data(buffer, sizeof (buffer));
                    SPI_Receive_Data(&ack_byte, 1);
                } while (ack_byte!=0xff);
//                USART_Send(0x34);
            } else{
                // CRC не совпадает
                USART_Send(0x01);
                USART1->CR1 |= USART_CR1_RXNEIE;
            }
            break;

        case I2C1_Received:
            index=0;
            state=none;
            if (!(crc_8(second_buffer, sizeof (second_buffer)))){
//                USART_Send(0x76);
                // CRC совпадает
                I2C1_Send_Byte(0x00);
                USART_Send_Buffer(second_buffer, sizeof (second_buffer));
                USART1->CR1 |= USART_CR1_RXNEIE;
                I2C1->CR2 |= I2C_CR2_ITBUFEN | I2C_CR2_ITEVTEN;
                I2C1->CR1 = I2C_CR1_PE | I2C_CR1_ACK;  // Включение периферии I2C1
            }else{
                // CRC не совпадает
                I2C1_Send_Byte(0x01);
                I2C1->CR1 = I2C_CR1_PE | I2C_CR1_ACK;  // Включение периферии I2C1
                I2C1->CR2 |= I2C_CR2_ITBUFEN | I2C_CR2_ITEVTEN;
            }
            break;

        default:
            break;
        }
    }

    return 0;
}


void GPIO_Init(){
    // Тактирование GPIOA, GPIOB
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN;


    //----------I2C1----------
    // Пины PB6 - I2C1_SCL, PB7 - I2C1_SDA, альтернативные функции
    GPIOB->MODER &= ~(GPIO_MODER_MODE6 | GPIO_MODER_MODE7);
    GPIOB->MODER |= GPIO_MODER_MODE6_1 | GPIO_MODER_MODE7_1;

    // Output open drain
    GPIOB->OTYPER |= GPIO_OTYPER_OT6 | GPIO_OTYPER_OT7;
    // Very high speed
    GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR6 | GPIO_OSPEEDER_OSPEEDR7;
    // Pull-up
    GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPD6 | GPIO_PUPDR_PUPD7);
    GPIOB->PUPDR |= GPIO_PUPDR_PUPD6_0 | GPIO_PUPDR_PUPD7_0;

    // AF04 - 0b0100
    GPIOB->AFR[0] &= ~GPIO_AFRL_AFRL6 & ~GPIO_AFRL_AFRL7;
    GPIOB->AFR[0] |= GPIO_AFRL_AFRL6_2 | GPIO_AFRL_AFRL7_2;


    // PA0 на вход
    GPIOA->MODER &= ~GPIO_MODER_MODE0;
    // PA0 pull-up
    GPIOA->PUPDR |= GPIO_PUPDR_PUPD0_0;
}

void I2C1_EV_IRQHandler(){
    if (I2C1->SR1 & I2C_SR1_RXNE){
        second_buffer[index++] = I2C1->DR;
        if (index >= message_length){
            state = I2C1_Received;
            I2C1->CR1 &= ~I2C_CR1_ACK;
        }
    }

    if (I2C1->SR1 & I2C_SR1_ADDR){
        (void) I2C1->SR1;
        (void) I2C1->SR2;
    }

    if (I2C1->SR1 & I2C_SR1_STOPF){
        (void) I2C1->SR1;
        I2C1->CR1 = I2C_CR1_PE;
        I2C1->CR2 &= ~(I2C_CR2_ITBUFEN | I2C_CR2_ITEVTEN);
    }

}

void USART1_IRQHandler(){
    if (USART1->SR & USART_SR_RXNE){
        buffer[index++] = USART1->DR;
        if (index >= message_length){
            USART1->CR1 &= ~USART_CR1_RXNEIE;
            state = USART1_Received;
        }
    }
}
