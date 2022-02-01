#include "main.h"
#include "avr/interrupt.h"
#include "i2c.h"
#include "usart.h"
#include "util/twi.h"
#include "crc-8.h"

enum {
    USART_Receive_Transmit,
    TWI_Transmit_Receive
} state;

uint8_t USART_Rx_Buffer[13]={0};
uint8_t cnt=0;

int main()
{
    uint8_t crc;
    uint8_t ack_byte;
    USART0_Init();
    TWI_Init();
    sei();

    state = USART_Receive_Transmit;
    DDRB |= 1<< LED_PIN;

    while (1) {

        switch (state) {

        case USART_Receive_Transmit:

            USART_Receive_Buffer(USART_Rx_Buffer, sizeof (USART_Rx_Buffer));

            if (!(crc=crc_8(USART_Rx_Buffer, sizeof(USART_Rx_Buffer)))){
                USART0_Transmit(0xa2);     //Подтверждение совпадения
                state = TWI_Transmit_Receive;
            }else{
                USART_Receive();
                USART0_Transmit(0x78);     //Подтверждение не совпадения
                state = USART_Receive_Transmit;
            }
            break;

        case TWI_Transmit_Receive:
            do {
                TWI_Transmit(SLA, USART_Rx_Buffer, sizeof(USART_Rx_Buffer));
                while (TWI_Busy());
                TWI_Receive(SLA, &ack_byte, sizeof (ack_byte));
            } while (ack_byte);
            state = USART_Receive_Transmit;
            break;

        default:
            break;
        }
    }
    return 0;
}
