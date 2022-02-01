#include "main.h"
#include <stdio.h>
#include "usart.h"
#include "spi.h"
#include "i2c.h"
#include "delay.h"
#include "crc8.h"

enum {
    SPI_Receive_Transmit,
    USART_Transmit_Receive,
}state;

uint8_t SPI_Rx_Buffer[13] = {0};

void GPIO_Init();

void SetSysClockTo16();

int main()
{
    SetSysClockTo16();
    SysTick_Config(SystemCoreClock/1000);

    GPIO_Init();
    USART1_Init();
    SPI1_Init();
//    I2C1_Init();

    __enable_irq();

    uint8_t ack_byte;
    state=SPI_Receive_Transmit;
    printf("Hello from STM32F103C8T6 running on %d Hz!\n", (int)SystemCoreClock);
    while (1) {

        switch (state) {
        case SPI_Receive_Transmit:
            SPI_Receive_Data(SPI_Rx_Buffer, sizeof (SPI_Rx_Buffer));
            if (!(crc_8(SPI_Rx_Buffer, sizeof (SPI_Rx_Buffer)))){

                ack_byte = 0xff;
                SPI_Transmit_Data(&ack_byte, 1);    // Подтверждение
                state = USART_Transmit_Receive;
                printf("%x\n", 0x10);
            }else{
                ack_byte = 0x01;
                SPI_Transmit_Data(&ack_byte, 1);
                state = SPI_Receive_Transmit;
            }
            break;

        case USART_Transmit_Receive:
            USART1_Transmit_Buffer(SPI_Rx_Buffer, sizeof (SPI_Rx_Buffer));
            ack_byte = USART1_Receive();
            printf("%x\n", ack_byte);
            if (ack_byte == 0xa2){
                state = SPI_Receive_Transmit;
            }else{
                state = USART_Transmit_Receive;
            }
            break;

        default:
            break;
        }
    }
    return 0;
}


void GPIO_Init(){

    // тактирование порта GPIOA
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_AFIOEN | RCC_APB2ENR_IOPBEN;

    // коннигурация включения SW-DP, трассировка SWO
    AFIO->MAPR &= AFIO_MAPR_SWJ_CFG;
    AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_1;


    //----------USART1----------

    // Выводы PA9 - Tx конфигурирую на альтернативную функцию
    GPIOA->CRH &= ~GPIO_CRH_CNF9 & ~GPIO_CRH_MODE9;
    GPIOA->CRH |= GPIO_CRH_CNF9_1 | GPIO_CRH_MODE9_0;

    // Вывод PA10 - Rx на вход
    GPIOA->CRH &= ~GPIO_CRH_CNF10 & ~GPIO_CRH_MODE10;
    GPIOA->CRH |= GPIO_CRH_CNF10_0;

    //----------SPI1----------

    // PA5 - SPI1_SCK, Input floating
    GPIOA->CRL &= ~(GPIO_CRL_MODE5 | GPIO_CRL_CNF5);
    GPIOA->CRL |= GPIO_CRL_CNF5_0;

    // PA6 - SPI1_MISO, Alternate function push-pull
    GPIOA->CRL &= ~(GPIO_CRL_MODE6 | GPIO_CRL_CNF6);
    GPIOA->CRL |= GPIO_CRL_MODE6_0 | GPIO_CRL_CNF6_1;

    // PA7 - SPI1_MOSI, Input floating
    GPIOA->CRL &= ~(GPIO_CRL_MODE7 | GPIO_CRL_CNF7);
    GPIOA->CRL |= GPIO_CRL_CNF7_0;

    //----------I2C1----------

    // PB6 - I2C1_SCL, PB7 - I2C1_SDA, Alternate function open-drain
    GPIOB->CRL |= 0xff << GPIO_CRL_MODE6_Pos;
}

void SetSysClockTo16(){
    // Конфигурация PLL
    RCC->CFGR &= ~RCC_CFGR_PLLMULL & ~RCC_CFGR_PLLSRC;
    RCC->CFGR |= RCC_CFGR_PLLMULL4;

    // Включение PLL
    RCC->CR |= RCC_CR_PLLON;

    // Ожидание запуска PLL
    while (!(RCC->CR & RCC_CR_PLLRDY)){}

    // PLL в качестве SysClock
    RCC->CFGR &= ~RCC_CFGR_SW;
    RCC->CFGR |= RCC_CFGR_SW_PLL;

    // Ожидание переключения SysClock на PLL
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL){}

    // Обновление SystemCoreClock
    SystemCoreClockUpdate();
}
